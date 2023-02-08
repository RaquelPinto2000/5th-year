/*
 * Paulo Pedreiras, 2022/10
 * 
 * Inits the console services and echoes any typed char
 * Returns are converted in Return + Newline 
 * 
 * Adapted from: ncs/zephyr/samples/subsys/console/echo
 *
 * Set prj.conf to:
 *   CONFIG_RTT_CONSOLE=n
 *   CONFIG_UART_CONSOLE=y
 *   CONFIG_CONSOLE_SUBSYS=y
 *   CONFIG_CONSOLE_GETCHAR=y
 *   CONFIG_CONSOLE_GETCHAR_BUFSIZE=64
 *   CONFIG_CONSOLE_PUTCHAR_BUFSIZE=512
 */

#include <zephyr.h>
#include <device.h>
#include <drivers/gpio.h>
#include <string.h>

#include <console/console.h>

static const char prompt[] = "Character echo started ...\r\n";


void main(void)
{    
    uint8_t c;

    /* Welcome message */
    printk("\n\r Console test. Characters typed by the user are echoed.  \n\r");

    /* Init console service */
    console_init();
    
    /* Output a string*/ 
    console_write(NULL, prompt, sizeof(prompt) - 1);

    /* Wait for chars and echoes them. Newline is added after return */ 
    while (1) {
             c = console_getchar();

            console_putchar(c);
            if (c == '\r') {
                    console_putchar('\n');
            }
    }
 
}
