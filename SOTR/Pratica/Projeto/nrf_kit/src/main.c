#include <zephyr.h>
#include <zephyr/device.h>

#include <string.h>

#include "imageProcessing/imageProcessing.h"
#include "cab/cab.h"
#include "cab_msg.h"

#include "uart.c"
#include "imageGenAuto.h"

/**
 * @brief Threads declaration and configurations.
 * Based on the RT
 * 
 */

/* Real Time Tasks periods */
#define SERIAL_COM_PERIOD_MS 1000
#define GUIDELINE_SEARCH_PERIOD_MS 1000
#define NEAR_OBJS_PERIOD_MS 1000
#define OBJ_COUNT_PERIOD_MS 1000
#define OUTPUT_PERIOD_MS 1000

/* Size of stack area used by each thread (can be thread specific, if necessary)*/
#define STACK_SIZE 1024

/* Thread scheduling priority */
// Cooperative Threads due to their importance
#define near_objects_thread_priotity -1 
#define output_thread_priotity -1
// Time slicing threads
#define serial_com_thread_priority 1
#define guideline_thread_prioriry 2
#define object_count_thread_priotity 3

/* Therad periodicity (in ms)*/

/* Create thread stack space */
K_THREAD_STACK_DEFINE(serial_com_thread_stack, STACK_SIZE);
K_THREAD_STACK_DEFINE(guideline_thread_stack, STACK_SIZE);
K_THREAD_STACK_DEFINE(near_objects_thread_stack, STACK_SIZE);
K_THREAD_STACK_DEFINE(object_count_thread_stack, STACK_SIZE);
K_THREAD_STACK_DEFINE(output_thread_stack, STACK_SIZE);
  
/* Create variables for thread data */
struct k_thread serial_com_thread_d;
struct k_thread guideline_thread_d;
struct k_thread near_objects_thread_d;
struct k_thread object_count_thread_d;
struct k_thread output_thread_d;

/* Create task IDs */
k_tid_t serial_com_thread_tid;
k_tid_t guideline_thread_tid;
k_tid_t near_objects_thread_tid;
k_tid_t object_count_thread_tid;
k_tid_t output_thread_tid;

/* Thread code prototypes, based on the provided RT demo */
/** 
 * @brief Task responsible for comunication with external device for image exchange.
 * Uses the serial port.
 * The image is stored in cab_image.
 * Periodic task with period given by SERIAL_COM_PERIOD_MS. 
 */
void serial_com_thread_code(void *argA, void *argB, void *argC);

/**
 * @brief Task responsible for applying the guideline finding algorithm to the image.
 */
void guideline_thread_code(void *argA, void *argB, void *argC);

/**
 * @brief Task responsible for applying the near objects algorithm to the image.
 */
void near_objects_thread_code(void *argA, void *argB, void *argC);

/**
 * @brief Task responsible for applying the object_count finding algorithm to the image.
 */
void object_count_thread_code(void *argA, void *argB, void *argC);

/**
 * @brief
 */
void output_thread_code(void *argA, void *argB, void *argC);


/* CABs */
CAB cab_img, cab_guideline, cab_near_objs, cab_obj_count;

int main(int argc, char const *argv[])
{
    /* init CABs */
    cab_img = cab_create("cab image", 4, sizeof(cab_msg_image), NULL);
    cab_guideline = cab_create("cab guideline", 2, sizeof(cab_msg_guideline), NULL);
    cab_near_objs = cab_create("cab near objects", 2, sizeof(cab_msg_near_objects), NULL);
    cab_obj_count = cab_create("ab obj count", 2, sizeof(cab_msg_num_objects), NULL);

    /* Init Uart*/
    //init_uart();

    /* Create tasks */
    serial_com_thread_tid = k_thread_create(&serial_com_thread_d, serial_com_thread_stack,
        K_THREAD_STACK_SIZEOF(serial_com_thread_stack), serial_com_thread_code,
        NULL, NULL, NULL, serial_com_thread_priority, 0, K_NO_WAIT);

    guideline_thread_tid = k_thread_create(&guideline_thread_d, guideline_thread_stack,
        K_THREAD_STACK_SIZEOF(guideline_thread_stack), guideline_thread_code,
        NULL, NULL, NULL, guideline_thread_prioriry, 0, K_NO_WAIT);
    

    near_objects_thread_tid = k_thread_create(&near_objects_thread_d, near_objects_thread_stack,
        K_THREAD_STACK_SIZEOF(near_objects_thread_stack), near_objects_thread_code,
        NULL, NULL, NULL, near_objects_thread_priotity, 0, K_NO_WAIT);

    object_count_thread_tid = k_thread_create(&object_count_thread_d, object_count_thread_stack,
        K_THREAD_STACK_SIZEOF(object_count_thread_stack), object_count_thread_code,
        NULL, NULL, NULL, object_count_thread_priotity, 0, K_NO_WAIT);

    output_thread_tid = k_thread_create(&output_thread_d, output_thread_stack,
        K_THREAD_STACK_SIZEOF(output_thread_stack), output_thread_code,
        NULL, NULL, NULL, output_thread_priotity, 0, K_NO_WAIT);


    return 0;
}

void serial_com_thread_code(void *argA, void *argB, void *argC){
    /* Timing variables to control task periodicity */
    int64_t fin_time=0, release_time=0;
    
    printk("Serial COmununication Thread Init (Periodic)\n");

    /* Compute next release instant */
    release_time = k_uptime_get() + SERIAL_COM_PERIOD_MS;
    //int err;
    int i = 0;
    char* image_data;
    cab_msg_image* msg_buffer;
    while (1)
    {
        switch (i)
        {
        case 0:
            image_data = gen_start_image();
            break;
        case 1:
            image_data = gen_first_image();
            break;
        
        case 2:
            image_data = gen_second_image();
            i=-1; // reset idx
            break;
        default:
            return;
        }
        msg_buffer = (cab_msg_image*) cab_reserve(cab_img);
        bytecpy(msg_buffer, image_data, sizeof(cab_msg_image));
        cab_putmes(cab_img, (char*) msg_buffer);
        k_free(image_data);
        i++;
        

        /* Wait for next release instant */ 
        fin_time = k_uptime_get();
        if( fin_time < release_time) {
            k_msleep(release_time - fin_time);
            release_time += SERIAL_COM_PERIOD_MS;
        }
    }
}

void guideline_thread_code(void *argA, void *argB, void *argC){
    /* Timing variables to control task periodicity */
    int64_t fin_time=0, release_time=0;
    
    printk("Guideline Search Thread Init (Periodic)\n");

    /* Compute next release instant */
    release_time = k_uptime_get() + GUIDELINE_SEARCH_PERIOD_MS;
    //int err;
    while (1)
    {
        
        if(cab_hasmes(cab_img)){
            //printf("Guideline Search -> Has Data\n");

            cab_msg_image* image_data = (cab_msg_image*) cab_getmes(cab_img);
            cab_msg_guideline* outMsg = (cab_msg_guideline*) cab_reserve(cab_guideline);

            guidelineSearch(image_data->image_data, &(outMsg->pos), &(outMsg->angle));

            cab_unget(cab_img, (char*) image_data);
            cab_putmes(cab_guideline, (char*) outMsg);
        }     
        
        /* Wait for next release instant */ 
        fin_time = k_uptime_get();
        if( fin_time < release_time) {
            k_msleep(release_time - fin_time);
            release_time += GUIDELINE_SEARCH_PERIOD_MS;
        }
    }
}

void near_objects_thread_code(void *argA, void *argB, void *argC){
    /* Timing variables to control task periodicity */
    int64_t fin_time=0, release_time=0;
    
    printk("Near Objects Search Thread Init (Periodic)\n");

    /* Compute next release instant */
    release_time = k_uptime_get() + NEAR_OBJS_PERIOD_MS;
    //int err;

    while (1)
    {
        if(cab_hasmes(cab_img)){
            cab_msg_image* image_msg = (cab_msg_image*) cab_getmes(cab_img);
            cab_msg_near_objects* outMsg = (cab_msg_near_objects*) cab_reserve(cab_near_objs);
            
            
            outMsg->near_objects = nearObstSearch(image_msg->image_data);

            cab_unget(cab_img, (char*) image_msg);
            cab_putmes(cab_near_objs, (char*) outMsg);
        }

        /* Wait for next release instant */ 
        fin_time = k_uptime_get();
        if( fin_time < release_time) {
            k_msleep(release_time - fin_time);
            release_time += NEAR_OBJS_PERIOD_MS;
        }
    }    
}

void object_count_thread_code(void *argA, void *argB, void *argC){
    /* Timing variables to control task periodicity */
    int64_t fin_time=0, release_time=0;
    
    printk("Object Count Thread Init (Periodic)\n");

    /* Compute next release instant */
    release_time = k_uptime_get() + OBJ_COUNT_PERIOD_MS;
    //int err;

    while (1)
    {
        if(cab_hasmes(cab_img)){
            cab_msg_image* image_msg = (cab_msg_image*) cab_getmes(cab_img);
            cab_msg_num_objects* outMsg = (cab_msg_num_objects*) cab_reserve(cab_obj_count);

            outMsg->num_objects = obstCount(image_msg->image_data);
            

            cab_unget(cab_img, (char*) image_msg);
            cab_putmes(cab_obj_count, (char*) outMsg);
        }

        /* Wait for next release instant */ 
        fin_time = k_uptime_get();
        if( fin_time < release_time) {
            k_msleep(release_time - fin_time);
            release_time += OBJ_COUNT_PERIOD_MS;
        }
    }
    
}

void output_thread_code(void *argA, void *argB, void *argC){
    /* Timing variables to control task periodicity */
    int64_t fin_time=0, release_time=0;
    
    printk("Output Thread Init (Periodic)\n");

    /* Compute next release instant */
    release_time = k_uptime_get() + OUTPUT_PERIOD_MS;
    //int err = 0;

    cab_msg_guideline* guideline_msg;
    cab_msg_near_objects* near_objs_msg;
    cab_msg_num_objects* num_objs_msg;

    while (1)
    {
        if(cab_hasmes(cab_guideline)){
            guideline_msg = (cab_msg_guideline*) cab_getmes(cab_guideline);
            printf("Output -> Guideline Data(%p): Angle=%.2f  Pos=%d\n", guideline_msg, guideline_msg->angle, guideline_msg->pos);
            cab_unget(cab_guideline, (char*) guideline_msg);
        }

        if(cab_hasmes(cab_near_objs)){
            near_objs_msg = (cab_msg_near_objects*) cab_getmes(cab_near_objs);
            printf("Output -> Near Objects(%p): %d\n", near_objs_msg, near_objs_msg->near_objects);
            cab_unget(cab_near_objs, (char*) near_objs_msg);
        }

        if(cab_hasmes(cab_obj_count)){
            num_objs_msg = (cab_msg_num_objects*) cab_getmes(cab_obj_count);
            printf("Output -> No. Objects(%p): %d\n", num_objs_msg, num_objs_msg->num_objects);
            cab_unget(cab_obj_count, (char*) num_objs_msg);
        }

        /* Wait for next release instant */ 
        fin_time = k_uptime_get();
        if( fin_time < release_time) {
            k_msleep(release_time - fin_time);
            release_time += OUTPUT_PERIOD_MS;
        }
    }
    
}