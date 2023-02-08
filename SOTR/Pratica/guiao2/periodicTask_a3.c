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
#include <inttypes.h>

#include <sys/mman.h> // For mlockall

// Xenomai API (former Native API)
#include <alchemy/task.h>
#include <alchemy/timer.h>
#include <alchemy/queue.h>

#define MS_2_NS(ms)(ms*1000*1000) 	/* Convert ms to ns */

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
	FILE* resultFile;
};

struct sensorQueueMsgStruct {
	uint16_t value;
};
struct resultQueueMsgStruct {
	uint16_t value;
};

RT_TASK sensor_task_desc, processing_task_desc, storage_task_desc, test_task_desc; // Task decriptor

RT_QUEUE sensor_queue, result_queue; // Queues Descriptors

FILE* tsFile;
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
	if(argc != 4) {
	  printf("Usage: %s SENSOR_DATA_FILE(in) SENSOR_DATA_FILTERED_FILE(out) TIMESTAMPS_FILE(out, csv format)\n\r", argv[0]);
	  return -1; 
	}

	// Open file with sensor values
	if ((sensorTaskArgs.sensorFile = fopen(argv[1], "r")) == NULL){
       printf("Error! opening file: %s\n", argv[1]);
       // Program exits if the file pointer returns NULL.
       exit(1);
   	}

	// Open file for result values
	if ((storageTaskArgs.resultFile = fopen(argv[2], "w")) == NULL){
       printf("Error! opening file: %s\n", argv[2]);
       // Program exits if the file pointer returns NULL.
       exit(1);
   	}
	
	if ((tsFile = fopen(argv[3], "w")) == NULL){
       printf("Error! opening file: %s\n", argv[3]);
       // Program exits if the file pointer returns NULL.
       exit(1);
   	}
	fprintf(tsFile, "task, ts, action, value\n");
	/* Lock memory to prevent paging */
	mlockall(MCL_CURRENT|MCL_FUTURE); 


	/* Create Xenomai Queues */
	err = rt_queue_create(&sensor_queue, "sensor_queue", sizeof(struct sensorQueueMsgStruct)*5, 5, Q_FIFO);
	if(err) {
		printf("Error creating Sensor Queue (error code = %d)\n",err);
		return err;
	} else 
		printf("Sensor Queue created successfully\n");
	
	err = rt_queue_create(&result_queue, "result_queue", sizeof(struct sensorQueueMsgStruct)*5, 5, Q_FIFO);
	if(err) {
		printf("Error creating Result Queue (error code = %d)\n",err);
		return err;
	} else 
		printf("Result Queue created successfully\n");
		
	sensorTaskArgs.queue = &sensor_queue;
	processingTaskArgs.sensorQueue = &sensor_queue;
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


	/* wait for termination signal */	
	wait_for_ctrl_c();

	/* CLEAN UP */
	
	fclose(sensorTaskArgs.sensorFile);
	fclose(storageTaskArgs.resultFile);

	rt_queue_delete(&sensor_queue);
	rt_queue_delete(&result_queue);
	

	return 0;
		
}

/* ***********************************
* Task body implementation
* *************************************/
void sensor_task(void *args) {
	RT_TASK *curtask;
	RT_TASK_INFO curtaskinfo;
	struct sensorTaskArgsStruct *taskArgs;
	struct sensorQueueMsgStruct *sensorMsg;

	unsigned long overruns;
	int err;

	/* Get task information */
	curtask=rt_task_self();
	rt_task_inquire(curtask,&curtaskinfo);
	taskArgs=(struct sensorTaskArgsStruct *)args;
	printf("Task %s init, period:%llu , PID: %d\n", curtaskinfo.name, taskArgs->taskPeriod_ns, gettid());

	/* Set task as periodic */
	err=rt_task_set_periodic(NULL, TM_NOW, taskArgs->taskPeriod_ns);

	for(;;) {
		err=rt_task_wait_period(&overruns);
		fprintf(tsFile, "%s, %lld, activation\n", curtaskinfo.name, rt_timer_read());

		// read value from file
		if(fscanf(taskArgs->sensorFile, "%" SCNd16 , &(sensorMsg->value)) == 1){
			//printf("Sensor read value: %d\n", sensorMsg->value);
			//printf("strctu size: %ld\n", sizeof(*sensorMsg));
			// add value to queue
			fprintf(tsFile, "%s,%lld, write to queue, %d\n", curtaskinfo.name, rt_timer_read(), sensorMsg->value);
			rt_queue_write(taskArgs->queue, sensorMsg, sizeof(*sensorMsg), Q_NORMAL);
		}
		else {
			printf("Sensor reached EOF\n");
			break;
		}
		fprintf(tsFile, "%s, %lld, back to sleep\n", curtaskinfo.name, rt_timer_read());
	}
	fprintf(tsFile, "%s, %lld, done\n", curtaskinfo.name, rt_timer_read());

	printf("Sensor task has finished!\n");
	return;
}

void processing_task(void *args) {
	RT_TASK *curtask;
	RT_TASK_INFO curtaskinfo;
	struct processingTaskArgsStruct *taskArgs;
	struct sensorQueueMsgStruct *sensorMsg;
	//struct resultQueueMsgStruct *resultMsg;

	//int arr[5]={0};
	int arrHead = 0;
	uint16_t arrSum = 0, arrAvg = 0; 
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
		fprintf(tsFile,"%s, %lld, received value, %d\n", curtaskinfo.name, rt_timer_read(), sensorMsg->value);

		if(msgSize == -ETIMEDOUT || msgSize<=0) {
			break;
		}

		//printf("@processing_task msgSize: %d, msgValue: %d\n", msgSize, sensorMsg->value);
		
		arrSum -= taskArgs->arr[arrHead];
		taskArgs->arr[arrHead] = sensorMsg->value;
		arrSum += taskArgs->arr[arrHead];
		arrHead = (arrHead+1)%PROC_SAMPLE_NR;
		arrAvg = arrSum/PROC_SAMPLE_NR;

		//resultMsg->value = arrSum/PROC_SAMPLE_NR;
		// passing arrAvg directly as using resultMsg is resulting in seg fault
		// given that resultMsg is just a wrapper around uint16, sizeof(*resultMsg) == sizeof(arrAvg) 
		rt_queue_write(taskArgs->resultQueue, &arrAvg, sizeof(arrAvg), Q_NORMAL);
		fprintf(tsFile, "%s, %lld, write to result queue, %d\n", curtaskinfo.name, rt_timer_read(), arrAvg);
	}
	printf("Processing task has finished!\n");
	return;
}

void storage_task(void *args) {
	RT_TASK *curtask;
	RT_TASK_INFO curtaskinfo;
	struct storageTaskArgsStruct *taskArgs;
	struct resultQueueMsgStruct *queueMsg;

	int msgSize;
	int err;

	/* Get task information */
	curtask=rt_task_self();
	rt_task_inquire(curtask,&curtaskinfo);
	taskArgs=(struct storageTaskArgsStruct *)args;
	printf("Task %s init, PID: %d\n", curtaskinfo.name, gettid());
	

	for(;;) {
		//err=rt_task_wait_period(&overruns);
		msgSize = rt_queue_read(taskArgs->resultQueue, queueMsg, sizeof(*queueMsg), ACK_PERIOD_NS*PROC_SAMPLE_NR);
		fprintf(tsFile, "%s, %lld, received value, %d\n", curtaskinfo.name, rt_timer_read(), queueMsg->value);

		if(msgSize == -ETIMEDOUT) {
			break;
		}
		
		//printf("@storage_task msgSize: %d, msgValue: %d\n", msgSize, queueMsg->value);

		//todo: store values from result queue
		fprintf(taskArgs->resultFile, "%d\n", queueMsg->value);
		fprintf(tsFile, "%s, %lld, write to file, %d\n", curtaskinfo.name, rt_timer_read(), queueMsg->value);

	}
	fflush(taskArgs->resultFile);
	printf("Storage Task has finished!\n");
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
