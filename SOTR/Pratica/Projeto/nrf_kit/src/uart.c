/*
 * Copyright (c) 2012-2014 Wind River Systems, Inc.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr.h>
#include <zephyr/device.h>
#include <zephyr/drivers/uart.h>

#include <string.h>

#include "imageProcessing/imageProcessing.h"
#include "cab/cab.h"

#define UART_NODE DT_NODELABEL(uart0)    /* UART Node label, see dts */

#define UART_MSG_SIZE 6//(IMGWIDTH*IMGWIDTH) 
#define RXBUF_SIZE UART_MSG_SIZE		/* RX buffer size */
#define TXBUF_SIZE 60                   /* TX buffer size */
#define RX_TIMEOUT 1000                  /* Inactivity period after the instant when last char was received that triggers an rx event (in us) */

/* Struct for UART configuration (if using default values is not needed) */
const struct uart_config uart_cfg = {
		.baudrate = 115200,
		.parity = UART_CFG_PARITY_NONE,
		.stop_bits = UART_CFG_STOP_BITS_1,
		.data_bits = UART_CFG_DATA_BITS_8,
		.flow_ctrl = UART_CFG_FLOW_CTRL_NONE
};

/* UAR related variables */
const struct device *uart_dev;          /* Pointer to device struct */ 
static uint8_t rx_buf[RXBUF_SIZE];      /* RX buffer, to store received data */
static uint8_t rx_chars[RXBUF_SIZE];    /* chars actually received  */
volatile int uart_rxbuf_nchar=0;        /* Number of chars currnetly on the rx buffer */

/* UART callback function prototype */
static void uart_cb(const struct device *dev, struct uart_event *evt, void *user_data);


/**
 * @brief UART definitions.
 * Based on Zephyr Os Example:
 * https://github.com/zephyrproject-rtos/zephyr/tree/main/samples/drivers/uart/echo_bot
 */
/* change this to any other UART peripheral if desired */


void init_uart(void)
{	
	// UART 
	
	/* Local vars */    
    int err=0; /* Generic error variable */
    uint8_t welcome_mesg[] = "UART demo: Type a few chars in a row and then pause for a little while ...\n\r"; 

	/* Bind to UART */
    uart_dev= device_get_binding(DT_LABEL(UART_NODE));
    if (uart_dev == NULL) {
        printk("device_get_binding() error for device %s!\n\r", DT_LABEL(UART_NODE));
        return;
    }
    else {
        printk("UART binding successful\n\r");
    }

    /* Configure UART */
    err = uart_configure(uart_dev, &uart_cfg);
    if (err == -ENOSYS) { /* If invalid configuration */
        printk("uart_configure() error. Invalid configuration\n\r");
        return; 
    }

    /* Register callback */
    err = uart_callback_set(uart_dev, uart_cb, NULL);
    if (err) {
        printk("uart_callback_set() error. Error code:%d\n\r",err);
        return;
    }
		
    /* Enable data reception */
    err =  uart_rx_enable(uart_dev ,rx_buf,sizeof(rx_buf),RX_TIMEOUT);
    if (err) {
        printk("uart_rx_enable() error. Error code:%d\n\r",err);
        return;
    }

	/* Send a welcome message */ 
    /* Last arg is timeout. Only relevant if flow controll is used */
    err = uart_tx(uart_dev, welcome_mesg, sizeof(welcome_mesg), SYS_FOREVER_MS);
    if (err) {
        printk("uart_tx() error. Error code:%d\n\r",err);
        return;
    }
}

void uart_main_loop() {
	int err = 0;
	uint8_t rep_mesg[TXBUF_SIZE];

	/* Main loop */
    while(1) {
        k_msleep(100);
        
        /* Print string received so far. */
        /* Very basic implementation, just for showing the use of the API */
        /* E.g. it does not prevent race conditions with the callback!!!!*/
        if(uart_rxbuf_nchar >= 1) {
            rx_chars[uart_rxbuf_nchar] = 0; /* Terminate the string */
            uart_rxbuf_nchar = 0;           /* Reset counter */

            //sprintf(rep_mesg,"Hello",rx_chars);            
            
            err = uart_tx(uart_dev, rep_mesg, strlen(rep_mesg), SYS_FOREVER_MS);
            if (err) {
                printk("uart_tx() error. Error code:%d\n\r",err);
                return;
            }
        }
    }
}


/* UART callback implementation */
/* Note that callback functions are executed in the scope of interrupt handlers. */
/* They run asynchronously after hardware/software interrupts and have a higher priority than all threads */
/* Should be kept as short and simple as possible. Heavier processing should be deferred to a task with suitable priority*/
static void uart_cb(const struct device *dev, struct uart_event *evt, void *user_data)
{
    int err;

    switch (evt->type) {
	
        case UART_TX_DONE:
		    //printk("UART_TX_DONE event \n\r");
            break;

    	case UART_TX_ABORTED:
	    	printk("UART_TX_ABORTED event \n\r");
		    break;
            
	    case UART_RX_RDY:
		    //printk("UART_RX_RDY event \n");
            printk("%c\n", rx_buf[evt->data.rx.offset]);
            /* Just copy data to a buffer. Usually it is preferable to use e.g. a FIFO to communicate with a task that shall process the messages*/
            memcpy(&rx_chars[uart_rxbuf_nchar],&(rx_buf[evt->data.rx.offset]),evt->data.rx.len); 
            uart_rxbuf_nchar++;           
		    break;

	    case UART_RX_BUF_REQUEST:
		    //printk("UART_RX_BUF_REQUEST event \n\r");
		    break;

	    case UART_RX_BUF_RELEASED:
		    //printk("UART_RX_BUF_RELEASED event \n\r");
		    break;
		
	    case UART_RX_DISABLED: 
            /* When the RX_BUFF becomes full RX is is disabled automaticaly.  */
            /* It must be re-enabled manually for continuous reception */
            //printk("UART_RX_DISABLED event \n\r");
		    err =  uart_rx_enable(uart_dev ,rx_buf,sizeof(rx_buf),RX_TIMEOUT);
            if (err) {
                printk("uart_rx_enable() error. Error code:%d\n\r",err);
                return;                
            }
		    break;

	    case UART_RX_STOPPED:
		    //printk("UART_RX_STOPPED event \n\r");
		    break;
		
	    default:
            //printk("UART: unknown event \n\r");
		    break;
    }

}