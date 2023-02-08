/* ************************************************************
* Xenomai - creates a periodic task
*	
* Paulo Pedreiras
* 	Out/2020: Upgraded from Xenomai V2.5 to V3.1    
* 
************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <math.h>

#include <sys/mman.h> // For mlockall

// Xenomai API (former Native API)
#include <alchemy/task.h>
#include <alchemy/timer.h>

#define MS_2_NS(ms)(ms*1000*1000) /* Convert ms to ns */

/* *******************
 * Task attributes 
 * *******************/ 
#define TASK_MODE 0  	// No flags
#define TASK_STKSZ 1000 	// Default stack size

#define TASK_A_PERIOD_NS MS_2_NS(1000)
#define ACK_PERIOD_NS MS_2_NS(100) 	/* Sensor task period */
#define PROC_SAMPLE_NR 5 			/* number of sample the filter should consider */

/* *****************************************************
 * Define task structure for setting input arguments
 * *****************************************************/

struct sensorTaskArgsStruct {
	RTIME taskPeriod_ns;
	RT_QUEUE* queue;
	FILE* sensorFile;
};
struct processingTaskArgsStruct {
	RT_QUEUE* sensorQueue;
	RT_QUEUE* resultQueue;
	int arr[PROC_SAMPLE_NR];
};
struct storageTaskArgsStruct {
	RT_QUEUE* resultQueue;
};

struct sensorQueueMsgStruct {
	uint16_t value;
};
struct resultQueueMsgStruct {
	uint16_t value;
};

struct AverageMsgStruct {
	int arrAverage [10000]; //qual e o tamanho do array?
};


RT_TASK sensor_task_desc, processing_task_desc, storage_task_desc, test_task_desc; // Task decriptor
RT_QUEUE sensor_queue, result_queue; // Queues Descriptors


/* *********************
* Function prototypes
* **********************/
void catch_signal(int sig); 	/* Catches CTRL + C to allow a controlled termination of the application */
void wait_for_ctrl_c(void);

void sensor_task(void *args); 		/* sensor task body */
void processing_task(void *args); 	/* processing task body */
void storage_task(void *args); 	/* storage task body */

/* ******************
* Main function
* *******************/ 
int main(int argc, char *argv[]) {
	int err; 
	struct sensorTaskArgsStruct sensorTaskArgs;
	struct processingTaskArgsStruct processingTaskArgs = {
		NULL,
		NULL,
		{0}
	};
	struct storageTaskArgsStruct storageTaskArgs;

	int tp=45;
	
	/* Process input args */
	if(argc != 2) {
	  printf("Usage: %s SENSOR_FILE \n\r", argv[0]);
	  return -1; 
	}

	// Open file with sensor values
	if ((sensorTaskArgs.sensorFile = fopen(argv[1], "r")) == NULL){
       printf("Error! opening file: %s\n", argv[1]);
       // Program exits if the file pointer returns NULL.
       exit(1);
   	}


	/* Lock memory to prevent paging */
	mlockall(MCL_CURRENT|MCL_FUTURE); 


	/* Create Xenomai Queues */
	err = rt_queue_create(&sensor_queue, "sensor_queue", sizeof(struct sensorQueueMsgStruct)*5, 5, Q_FIFO);
	if(err) {
		printf("Error creating Sensor Queue (error code = %d)\n",err);
		return err;
	} else 
		printf("Sensor Queue created successfully\n");
	sensorTaskArgs.queue = &sensor_queue;
	processingTaskArgs.sensorQueue = &sensor_queue;

	err = rt_queue_create(&result_queue, "result_queue", sizeof(struct sensorQueueMsgStruct)*5, 5, Q_FIFO);
	if(err) {
		printf("Error creating Result Queue (error code = %d)\n",err);
		return err;
	} else 
		printf("Result Queue created successfully\n");
	processingTaskArgs.resultQueue = &result_queue;
	storageTaskArgs.resultQueue = &result_queue;	

	/* Create RT task */
	/* Args: descriptor, name, stack size, priority [0..99] and mode (flags for CPU, FPU, joinable ...) */	
	/* Sensor Task */
	err=rt_task_create(&sensor_task_desc, "Sensor Task", TASK_STKSZ, tp, TASK_MODE);
	if(err) {
		printf("Error creating Sensor Task (error code = %d)\n",err);
		return err;
	} else 
		printf("Sensor Task created successfully\n");
	
	/* Processing Task */
	err=rt_task_create(&processing_task_desc, "Processing Task", TASK_STKSZ, tp, TASK_MODE);
	if(err) {
		printf("Error creating Processing Task (error code = %d)\n",err);
		return err;
	} else 
		printf("Processing Task created successfully\n");

	/* Storage Task */
	err=rt_task_create(&storage_task_desc, "Storage Task", TASK_STKSZ, tp, TASK_MODE);
	if(err) {
		printf("Error creating Storage Task (error code = %d)\n",err);
		return err;
	} else 
		printf("Storage Task created successfully\n");

	/* Test Task */
	err=rt_task_create(&test_task_desc, "Test Sensor Task", TASK_STKSZ, tp, TASK_MODE);
	if(err) {
		printf("Error creating Test Sensor Task (error code = %d)\n",err);
		return err;
	} else 
		printf("Test Sensor Task created successfully\n");

	/* Start RT task */
	/* Args: task decriptor, address of function/implementation and argument*/
	/* Sensor Task */
	//rt_task_set_affinity(&task_a_desc, &cpuset); 	
	sensorTaskArgs.taskPeriod_ns = ACK_PERIOD_NS;	
    rt_task_start(&sensor_task_desc, &sensor_task, (void *)&sensorTaskArgs);

	/* Procesing Task */
	rt_task_start(&processing_task_desc, &processing_task, (void *)&processingTaskArgs);

	/* Storage Task */
	rt_task_start(&storage_task_desc, &storage_task, (void *)&storageTaskArgs);


	/* Test Task */
    //rt_task_start(&test_task_desc, &test_task, (void *)&processingTaskArgs);

	/* wait for termination signal */	
	wait_for_ctrl_c();

	return 0;
		
}

/* ***********************************
* Task body implementation
* *************************************/
void sensor_task(void *args) {
	RT_TASK *curtask;
	RT_TASK_INFO curtaskinfo;
	struct sensorTaskArgsStruct *taskArgs; //mensagem do sensor
	struct sensorQueueMsgStruct *sensorMsg; // mensagem do sensor enviada pela fila

	unsigned long overruns;
	int err;
	
	/* Get task information */
	curtask=rt_task_self();
	rt_task_inquire(curtask,&curtaskinfo);
	taskArgs=(struct taskArgsStruct *)args;
	printf("Task %s init, period:%llu , PID: %d\n", curtaskinfo.name, taskArgs->taskPeriod_ns, gettid());
	
	/* Set task as periodic */
	err=rt_task_set_periodic(NULL, TM_NOW, taskArgs->taskPeriod_ns);
	for(;;) {
		err=rt_task_wait_period(&overruns); //espera
		//se conseguiu ler do sensor
		if (fscanf(taskArgs->sensorFile, "%" SCNd16 , &(sensorMsg->value)) == 1){ 
			//vai escrever o valor do sensor na fila
			rt_queue_write(taskArgs->queue, sensorMsg, sizeof(*sensorMsg), Q_NORMAL);
		}else{
			// se nao conseguiu ler do sensor
			printf("Sensor reached EOF\n");
			break;
		}		
	}
	printf("Sensor task has finished!\n");
	return;
}

/*Processing task: this task takes every sensor reading generated
by the sensor task and applies a filter. The filter output is then
sent to the storage task (tarefa de armazenamento), again via a message queue.*/
void processing_task(void *args) {
	RT_TASK *curtask;
	RT_TASK_INFO curtaskinfo;
	struct processingTaskArgsStruct *taskArgs;
	struct sensorQueueMsgStruct *sensorMsg;
	struct resultQueueMsgStruct *resultMsg;

	//int* larr[PROC_SAMPLE_NR] = taskArgs->arr;
	//int arr[5];
	int arrHead = 0;
	int16_t arrSum = 0; 
	int msgSize;
	int err;

	/* Get task information */
	curtask=rt_task_self();
	rt_task_inquire(curtask,&curtaskinfo);
	taskArgs=(struct processingTaskArgsStruct *)args;
	printf("Task %s init, PID: %d\n", curtaskinfo.name, gettid());

	for(;;) {		
		// read value from sensor queue
		msgSize = rt_queue_read(taskArgs->sensorQueue, sensorMsg, sizeof(*sensorMsg), ACK_PERIOD_NS*PROC_SAMPLE_NR);
		if(msgSize == -ETIMEDOUT || msgSize<=0) {
			break;
		}

		//printf("@processing_task msgSize: %d, msgValue: %d\n", msgSize, sensorMsg->value);

		//arr[arr0Head] = sensorMsg->value;
		arrSum -= taskArgs->arr[arrHead];
		taskArgs->arr[arrHead] = sensorMsg->value;
		arrSum += taskArgs->arr[arrHead];
		arrHead = (arrHead+1)%PROC_SAMPLE_NR;


		resultMsg->value = arrSum/PROC_SAMPLE_NR;
		rt_queue_write(taskArgs->resultQueue, resultMsg, sizeof(*resultMsg), Q_NORMAL);
	}
	printf("Processing task has finished!\n");
	return;
}


void storage_task(void *args) {
	RT_TASK *curtask;
	RT_TASK_INFO curtaskinfo;
	struct storageTaskArgsStruct *taskArgs;
	struct resultQueueMsgStruct *queueMsg;
	struct AverageMsgStruct *Average;
	
	int msgSize;
	int err;

	/* Get task information */
	curtask=rt_task_self();
	rt_task_inquire(curtask,&curtaskinfo);
	taskArgs=(struct storageTaskArgsStruct *)args;
	printf("Task %s init, PID: %d\n", curtaskinfo.name, gettid());
	
	int j=0;
	for(;;) {
		//err=rt_task_wait_period(&overruns);
		msgSize = rt_queue_read(taskArgs->resultQueue, queueMsg, sizeof(*queueMsg), ACK_PERIOD_NS*PROC_SAMPLE_NR);
		if(msgSize == -ETIMEDOUT) {
			break;
		}
		
		printf("@storage_task msgSize: %d, msgValue: %d\n", msgSize, queueMsg->value);

		//todo: store values from result queue
		
		int aux = 0;
		int length =  sizeof(Average)/sizeof(Average->arrAverage[0]);
		for(int i =0 ;i<length;i++){
			aux += Average->arrAverage[i];

		}
		Average->arrAverage[j] = (1/length) * (aux+ *taskArgs->resultQueue);
		j++;

	}
	printf("Storage Sensor task has finished!\n");
	return;
}


/* **************************************************************************
 *  Catch control+c to allow a controlled termination
 * **************************************************************************/
 void catch_signal(int sig)
 {
	 return;
 }

void wait_for_ctrl_c(void) {
	signal(SIGTERM, catch_signal); //catch_signal is called if SIGTERM received
	signal(SIGINT, catch_signal);  //catch_signal is called if SIGINT received

	// Wait for CTRL+C or sigterm
	pause();
	
	// Will terminate
	printf("Terminating ...\n");
}
