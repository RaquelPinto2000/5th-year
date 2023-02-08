/*
 * Paulo Pedreiras, 2022/10
 * Zephyr: Simple thread creation example (2)
 * 
 * One of the tasks is periodc, the other two are activated via a semaphore. Data communicated via sharem memory 
 *
 * Base documentation:
 *      https://developer.nordicsemi.com/nRF_Connect_SDK/doc/latest/zephyr/reference/kernel/index.html
 * 
 */


#include <zephyr.h>
#include <device.h>
#include <drivers/gpio.h>
#include <drivers/adc.h>
#include <sys/printk.h>
#include <sys/__assert.h>
#include <string.h>
#include <timing/timing.h>
#include <stdlib.h>
#include <stdio.h>

/**
 * @brief ADC includes, definitions, and configurations
 * Taken from the ADC demo.
 * */
#include <hal/nrf_saadc.h>
#define ADC_RESOLUTION 10
#define ADC_GAIN ADC_GAIN_1_4
#define ADC_REFERENCE ADC_REF_VDD_1_4
#define ADC_ACQUISITION_TIME ADC_ACQ_TIME(ADC_ACQ_TIME_MICROSECONDS, 40)
#define ADC_CHANNEL_ID 1  
#define ADC_CHANNEL_INPUT NRF_SAADC_INPUT_AIN1 
#define BUFFER_SIZE 1
#define ADC_NODE DT_NODELABEL(adc)  
const struct device *adc_dev = NULL;
#define TIMER_INTERVAL_MSEC 1000 /* Interval between ADC samples */

/* ADC channel configuration */
static const struct adc_channel_cfg my_channel_cfg = {
	.gain = ADC_GAIN,
	.reference = ADC_REFERENCE,
	.acquisition_time = ADC_ACQUISITION_TIME,
	.channel_id = ADC_CHANNEL_ID,
	.input_positive = ADC_CHANNEL_INPUT
};

/**
 * @brief Configuration of the DevKit LEDs.
 * Based Zephyr's Blinky demo.* 
 */
#define LED0_NODE DT_NODELABEL(led0)
#define LED1_NODE DT_NODELABEL(led1)
#define LED2_NODE DT_NODELABEL(led2)
#define LED3_NODE DT_NODELABEL(led3)
static const struct gpio_dt_spec l0s = GPIO_DT_SPEC_GET(LED0_NODE, gpios);
static const struct gpio_dt_spec l1s = GPIO_DT_SPEC_GET(LED1_NODE, gpios);
static const struct gpio_dt_spec l2s = GPIO_DT_SPEC_GET(LED2_NODE, gpios);
static const struct gpio_dt_spec l3s = GPIO_DT_SPEC_GET(LED3_NODE, gpios);

/**
 * @brief Configuration of the test button (Button 1 on DevKit).
 * Based on Zephyr's Button demo
 * 
 */
#define TEST_BUT_NODE DT_NODELABEL(button0)
static const struct gpio_dt_spec tbs = GPIO_DT_SPEC_GET(TEST_BUT_NODE, gpios);
static struct gpio_callback button_cb_data;

/**
 * @brief Threads declaration and configurations.
 * Based on the RT
 * 
 */
/* Size of stack area used by each thread (can be thread specific, if necessary)*/
#define STACK_SIZE 1024

/* Thread scheduling priority */
#define sensor_thread_priority 1
#define processing_thread_prioriry 1
#define output_thread_priotity 1
#define test_thread_priotity 2

/* Therad periodicity (in ms)*/
#define SAMP_PERIOD_MS 1000

/* Create thread stack space */
K_THREAD_STACK_DEFINE(sensor_thread_stack, STACK_SIZE);
K_THREAD_STACK_DEFINE(processing_thread_stack, STACK_SIZE);
K_THREAD_STACK_DEFINE(output_thread_stack, STACK_SIZE);
K_THREAD_STACK_DEFINE(test_thread_stack, STACK_SIZE);

  
/* Create variables for thread data */
struct k_thread sensor_thread_d;
struct k_thread processing_thread_d;
struct k_thread output_thread_d;
struct k_thread test_thread_d;

/* Create task IDs */
k_tid_t sensor_thread_tid;
k_tid_t processing_thread_tid;
k_tid_t output_thread_tid;
k_tid_t test_thread_tid;

/* Global vars (ADC Demo) */
static uint16_t adc_sample_buffer[BUFFER_SIZE];

/**
 * @brief Declaration of shared memory strutures and semaphores for its managment.
 * Based on the provided RT demo. 
 */
/* Global Vars (shared memory between tasks sensor_task/processing_task and processing_task/output_task, resp)*/
uint16_t sensorValue = 0;
uint16_t filteredValue = 0;

/* Semaphores for task sync */
struct k_sem sem_sensorValue;   // controls access to global var 'sensorValue', sync between sensor and processing tasks
struct k_sem sem_filteredValue; // controls access to global var 'filteredValue', sync between processing and output tasks
struct k_sem sem_output_leds;   // controls access to LEDs for output, sync between output and test tasks
struct k_sem sem_test_leds;     // used for trigering test task when test button is pressed

/* Thread code prototypes, based on the provided RT demo */
/**
 * @brief Task responsible for reading sensor values and making them available to the other tasks.
 * Periodic task with period given by SAMP_PERIOD_MS.
 * Saves read value in GV sensorValue.
 */
void sensor_thread_code(void *argA, void *argB, void *argC);

/**
 * @brief Task responsible for applying a moving average filter to the sensor values.
 * Moving average windows is 10 samples.
 * Saves read value in GV filteredValue.
 */
void processing_thread_code(void *argA, void *argB, void *argC);

/**
 * @brief Ligths the DevKit LEDs in diferent configurations based on the GV filteredValue.
 * 
 * Knowing that filteredValue = 0 => Distance = 0m e filteredValue = 1023 => Distance = 30m+ linearly, then:
 * [  0, 341[  -> [ 0, 10[m -> LEDs 1, 2, 3, e 4
 * [341, 682[  -> [10, 20[m -> LEDs 1, 2, e 3
 * [682, 1023[ -> [20, 30[m -> LEDs 1 e 2
 * 1023 -> 30m+ -> LEDs 1
 */
void output_thread_code(void *argA, void *argB, void *argC);

/**
 * @brief After being trigered, blinks all LEDs during 5seconds with a toggle period of 500ms (not RT periods).
 */
void test_thread_code(void *argA, void *argB, void *argC);

/* Initialization Functions */
int adc_init(void); // based on ADC demo initialization
void led_init(void); // based on Zephyr's Blinky demo initialization
void button_init(void); // based on Zephyr's Button demo initialization

/* Helper Functions */
/**
 * @brief Takes one sample from ADC and saves it to global variable adc_sample_buffer.
 * Based on ADC example code.
 * @return int error code (0 if no error).
 */
static int adc_sample(void); // From ADC Demo

/**
 * @brief Test button onClick handler.
 * Triggers test task using sem_test_leds. 
 * Based on Zephyr's Button demo. 
 */
void on_test_button_click(void);

/* Main function */
void main(void) {
    
    /* Welcome message */
    printf("\n\r SOTR Assignment 3 - Zephyr RTOS for Embedded RT Applications\n\r");
    
    // init all components
    led_init();
    adc_init();
    button_init();
    
    /* Create and init semaphores */
    k_sem_init(&sem_sensorValue, 0, 1);
    k_sem_init(&sem_filteredValue, 0, 1);    
    k_sem_init(&sem_test_leds, 0, 1);   
    k_sem_init(&sem_output_leds, 0, 1); 
    // release output_leds semaphore
    k_sem_give(&sem_output_leds);

    
    /* Create tasks */
    sensor_thread_tid = k_thread_create(&sensor_thread_d, sensor_thread_stack,
        K_THREAD_STACK_SIZEOF(sensor_thread_stack), sensor_thread_code,
        NULL, NULL, NULL, sensor_thread_priority, 0, K_NO_WAIT);

    // Processing thread argA=10 (numSamples)
    // int numSamples = 10;
    processing_thread_tid = k_thread_create(&processing_thread_d, processing_thread_stack,
        K_THREAD_STACK_SIZEOF(processing_thread_stack), processing_thread_code,
        NULL, NULL, NULL, processing_thread_prioriry, 0, K_NO_WAIT);

    output_thread_tid = k_thread_create(&output_thread_d, output_thread_stack,
        K_THREAD_STACK_SIZEOF(output_thread_stack), output_thread_code,
        NULL, NULL, NULL, output_thread_priotity, 0, K_NO_WAIT);

    // Test Led thread:
    // testTimeMs = 5000 (time of testing, ms)
    // sleepTimeMs = 500 (sleep time between toggles, ms)
    test_thread_tid = k_thread_create(&test_thread_d, test_thread_stack,
        K_THREAD_STACK_SIZEOF(test_thread_stack), test_thread_code,
        NULL, NULL, NULL, output_thread_priotity, 0, K_NO_WAIT);

    return;

} 

/* Sensor task (periodic) implementation */
void sensor_thread_code(void *argA , void *argB, void *argC)
{
    /* Timing variables to control task periodicity */
    int64_t fin_time=0, release_time=0;
    
    printk("Sensor Thread Init (Periodic)\n");

    /* Compute next release instant */
    release_time = k_uptime_get() + SAMP_PERIOD_MS;
    int err;

    /* Thread loop */
    while(1) {      

        /* Get one sample, checks for errors and prints the values */
        err=adc_sample();
        if(err) {
            printk("adc_sample() failed with error code %d\n\r",err);
        }
        else {
            if(adc_sample_buffer[0] > 1023) {
                printk("adc reading out of range\n\r");
            }
            else {
                sensorValue = adc_sample_buffer[0];
            }
        }

        printk("Sensor Thread set sensorValue to: %d \n",sensorValue);  

        k_sem_give(&sem_sensorValue);

        /* Wait for next release instant */ 
        fin_time = k_uptime_get();
        if( fin_time < release_time) {
            k_msleep(release_time - fin_time);
            release_time += SAMP_PERIOD_MS;
        }
    }

}
/* Processing task implementation */
void processing_thread_code(void *argA , void *argB, void *argC)
{
    int nSamples = 10;
    int arr[10] = {0};
    int arrHead = 0;
    int sum = 0;

    printk("Processing Thread init (sporadic, waits on a semaphore by Sensor Task)\n");
    while(1) {
        k_sem_take(&sem_sensorValue,  K_FOREVER);
        //printk("Processing Thread read sensorValue value: %d\n",sensorValue);

        sum -= arr[arrHead];
        sum += sensorValue;
        arr[arrHead] = sensorValue;

        //arrHead = ++arrHead%nSamples;
        arrHead = (arrHead+1)%nSamples;

        filteredValue = sum/nSamples;
        
        printk("Processing Thread set filteredValue value to: %d \n",filteredValue);  
        k_sem_give(&sem_filteredValue);
  }
}

/* Output task implementation */
void output_thread_code(void *argA , void *argB, void *argC)
{
    printk("Output Thread init (sporadic, waits on a semaphore by processing task)\n");

    while(1) {
        k_sem_take(&sem_filteredValue, K_FOREVER);  
        k_sem_take(&sem_output_leds, K_FOREVER);    
        printk("Output Thread read filteredValue: %d\n",filteredValue);
        
        // reset pin outputs
        gpio_pin_set_dt(&l0s, 1);
        gpio_pin_set_dt(&l1s, 0);
        gpio_pin_set_dt(&l2s, 0);
        gpio_pin_set_dt(&l3s, 0);

        
        if(filteredValue<1023) {
            gpio_pin_set_dt(&l1s, 1);
        }

        if(filteredValue<682) {
            gpio_pin_set_dt(&l2s, 1);
        }

        if(filteredValue<341) {
            gpio_pin_set_dt(&l3s, 1);
        }
        k_sem_give(&sem_output_leds);
    }
}

/**
 * LED Test Task
 * Waits on sem_test_leds semaphore, after awaken blinks all LEDs for N seconds
*/
void test_thread_code(void *testTimeMs , void *sleepTimeMs, void *argC) {
    printk("Test Thread init (sporadic, waits on button 1 press)\n");
    int testTime = 5000;//*((int*) testTimeMs);
    int sleepTime =  500;//*((int*) sleepTimeMs);
    int nCycles = testTime/sleepTime;
    while (1)
    {
        k_sem_take(&sem_test_leds, K_FOREVER);
        printk("@test_thread_code received button press\n");

        k_sem_take(&sem_output_leds, K_FOREVER);
        //printk("@test_thread_code took control of output leds\n");
        // reset pin outputs
        gpio_pin_set_dt(&l0s, 0);
        gpio_pin_set_dt(&l1s, 0);
        gpio_pin_set_dt(&l2s, 0);
        gpio_pin_set_dt(&l3s, 0);

        for(int i=0; i<nCycles;i++) {
            //printk("@test_thread_code cycle nr: %d\n", nCycles);
            gpio_pin_toggle_dt(&l0s);
            gpio_pin_toggle_dt(&l1s);
            gpio_pin_toggle_dt(&l2s);
            gpio_pin_toggle_dt(&l3s);

            k_msleep(sleepTime);
        }
        k_sem_give(&sem_output_leds);
    }
}

void on_test_button_click(void) {
    // trigger test task.
    k_sem_give(&sem_test_leds);
}

/**
 * ADC Initialization routine.
 * Based on ADC example initialization.
*/
int adc_init(void) {
    int err=0;
    /* ADC setup: bind and initialize */
    adc_dev = device_get_binding(DT_LABEL(ADC_NODE));
	if (!adc_dev) {
        printk("ADC device_get_binding() failed\n");
    } 
    err = adc_channel_setup(adc_dev, &my_channel_cfg);
    if (err) {
        printk("adc_channel_setup() failed with error code %d\n", err);
    }
    
    /* It is recommended to calibrate the SAADC at least once before use, and whenever the ambient temperature has changed by more than 10 °C */
    NRF_SAADC->TASKS_CALIBRATEOFFSET = 1;

    return err;
}

/**
 * LEDs initialization routine.
 * Based on LED example initialization.
*/
void led_init(void) {
    int ret;
    /* Check if devices are ready */
	if (
        !device_is_ready(l0s.port) || 
        !device_is_ready(l1s.port) || 
        !device_is_ready(l2s.port) ||
        !device_is_ready(l3s.port) 
        ) 
    {
        printk("Error: LEDs not ready");
		return ;
	}

    /* Configure the GPIO pins for output */
	ret = gpio_pin_configure_dt(&l0s, GPIO_OUTPUT_ACTIVE);
	if (ret < 0) {
        printk("Error in LED 0 init");
		return;
	}
    ret = gpio_pin_configure_dt(&l1s, GPIO_OUTPUT_ACTIVE);
	if (ret < 0) {
        printk("Error in LED 1 init");
		return;
	}
    ret = gpio_pin_configure_dt(&l2s, GPIO_OUTPUT_ACTIVE);
	if (ret < 0) {
        printk("Error in LED 2 init");
		return;
	}
    ret = gpio_pin_configure_dt(&l3s, GPIO_OUTPUT_ACTIVE);
	if (ret < 0) {
        printk("Error in LED 3 init");
		return;
	}
}

/**
 * Button initialization routine.
 * Based on Button example initialization.
*/
void button_init(void) {
    int ret;

    if (!device_is_ready(tbs.port)) {
		printk("Error: button device %s is not ready\n",
		       tbs.port->name);
		return;
	}

	ret = gpio_pin_configure_dt(&tbs, GPIO_INPUT);
	if (ret != 0) {
		printk("Error %d: failed to configure %s pin %d\n",
		       ret, tbs.port->name, tbs.pin);
		return;
	}

	ret = gpio_pin_interrupt_configure_dt(&tbs,
					      GPIO_INT_EDGE_TO_ACTIVE);

	if (ret != 0) {
		printk("Error %d: failed to configure interrupt on %s pin %d\n",
			ret, tbs.port->name, tbs.pin);
		return;
	}

	gpio_init_callback(&button_cb_data, (void*) on_test_button_click, BIT(tbs.pin));
	gpio_add_callback(tbs.port, &button_cb_data);
	printk("Set up button at %s pin %d\n", tbs.port->name, tbs.pin);
}

/**
 * Takes one sample from ADC and saves it to global variable adc_sample_buffer.
 * Based on ADC example code.
 */
static int adc_sample(void)
{
	int ret;
	const struct adc_sequence sequence = {
		.channels = BIT(ADC_CHANNEL_ID),
		.buffer = adc_sample_buffer,
		.buffer_size = sizeof(adc_sample_buffer),
		.resolution = ADC_RESOLUTION,
	};

	if (adc_dev == NULL) {
            printk("adc_sample(): error, must bind to adc first \n\r");
            return -1;
	}

	ret = adc_read(adc_dev, &sequence);
	if (ret) {
            printk("adc_read() failed with code %d\n", ret);
	}	

	return ret;
}