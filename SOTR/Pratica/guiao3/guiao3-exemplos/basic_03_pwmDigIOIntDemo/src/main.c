/*
 * Paulo Pedreiras, 2022/09
 * Simple Digital I/O, PWM and External Interrupt example
 * 
 * Button 1 generates an Int and:
 *    i) toggles led1.
 *    ii) cycles among PWM_NLEVELS different Duty-Cycles on led 0 (associated with led0)
 * 
 * Base documentation:
 *        
 *      HW info:
 *          https://infocenter.nordicsemi.com/topic/struct_nrf52/struct/nrf52840.html
 *          Section: nRF52840 Product Specification -> Peripherals -> GPIO / GPIOTE
 * 
 *          Board specific HW info can be found in the nRF52840_DK_User_Guide_20201203. I/O pins available at pg 27
 *
 *          Note that Leds are active-low. I.e., they turn on when a logic zero is output at the corresponding port, and vice-versa.
 *          This behaviour can be reverted by adding the "GPIO_ACTIVE_LOW" flag - gpio_pin_config( ...., GPIO_OUTPUT_ACTIVE | GPIO_ACTIVE_LOW)
 
 *
 *      SW API info:
 *          https://developer.nordicsemi.com/nRF_Connect_SDK/doc/latest/zephyr/reference/peripherals/gpio.html 
 * 
 * 
 * 	Notes:
 * 		Note that the GPIO and PWM must be enabled in prj.conf 
 * 		CONFIG_GPIO=y
 * 		CONFIG_PWM=y
 * 		
 * 		Check prj.conf for additional contents that allwos to see log messages 
 * 		and also printf()/printk() output
 * 
 */

/* Includes. zephyr.h is always necessary. Add the othert ones according with the resourtces used
  In this case are used the gpio and pwm modules 
 */ 
#include <zephyr.h>
#include <drivers/gpio.h>
#include <drivers/pwm.h>

/* Set the PWM period. Note that there is HW that retricts the maximum value that can be used 
 * When assigning should check for errors.
*/
#define PWM_PERIOD 10000000 /* Value specified in ns */ 
#define PWM_NLEVELS 5 /* Number of PWM duty-cycle levels*/

/* Use a "big" sleep time to reduce CPU load (button detection int activated, not polled) */
#define SLEEP_TIME_MS   60*1000 

/* Get node IDs fro LED1, Button 1 and pwm0. Note that 
* LED1 is labeld led0 and Button 1 is labeled button0 in DTS file). */ 
#define LED0_NODE DT_NODELABEL(led0)
#define LED1_NODE DT_NODELABEL(led1)
#define SW0_NODE DT_NODELABEL(button0)
#define PWM0_NODE DT_NODELABEL(pwm_led0)

/* Now get the corresponding device pointer, pin number, configuration flags, ... */
static const struct gpio_dt_spec led0 = GPIO_DT_SPEC_GET(LED0_NODE, gpios);
static const struct gpio_dt_spec led1 = GPIO_DT_SPEC_GET(LED1_NODE, gpios);
static const struct gpio_dt_spec button = GPIO_DT_SPEC_GET(SW0_NODE, gpios);
static const struct pwm_dt_spec pwm_led0 = PWM_DT_SPEC_GET(DT_ALIAS(pwm_led0));


/* Define a callback function. It is like an ISR that is called when the button is pressed */
void button_pressed(const struct device *dev, struct gpio_callback *cb, uint32_t pins)
{
	static int div = 1;
    /* Toggle led1 */
	gpio_pin_toggle_dt(&led1);

	/* Adjut the brightness of led0 (associated with pwm) 
	 * 5 levels of intensity, which are actually dividers that set the duty-cycle
	 */
	div++;
	if(div > PWM_NLEVELS)
		div = 1;
	
	printk("PWM divider set to %d\n\r", div);
	
	pwm_set_dt(&pwm_led0, PWM_PERIOD, PWM_PERIOD/((unsigned int)div)); /* args are period and Ton */

}

/* Define a variable of type static struct gpio_callback, which will latter be used to install the callback
*  It defines e.g. which pin triggers the callback
*/
static struct gpio_callback button_cb_data;


/* 
 * The main function
 */
void main(void)
{
	int ret;

	/* Check if devices are ready */
	if (!device_is_ready(led1.port)) {
		printk("Error: led1 device %s is not ready\n", led1.port->name);
		return;
	}

	if (!device_is_ready(button.port)) {
		printk("Error: button device %s is not ready\n", button.port->name);
		return;
	}

	if (!device_is_ready(pwm_led0.dev)) {
		printk("Error: PWM device %s is not ready\n", pwm_led0.dev->name);
		return;
	}

	/* Configure the GPIO pins - led for output and button for input
	 * Use internal pull-up to avoid the need for an external resitor (button)
	 */
	ret = gpio_pin_configure_dt(&led1, GPIO_OUTPUT_ACTIVE);
	if (ret < 0) {
		printk("Error: gpio_pin_configure_dt failed for led1, error:%d", ret);
		return;
	}

	ret = gpio_pin_configure_dt(&button, GPIO_INPUT | GPIO_PULL_UP);
	if (ret < 0) {
		printk("Error: gpio_pin_configure_dt failed for button, error:%d", ret);
		return;
	}

	/* Configure the interrupt on the button's pin */
	ret = gpio_pin_interrupt_configure_dt(&button, GPIO_INT_EDGE_TO_ACTIVE );
	if (ret < 0) {
		printk("Error: gpio_pin_interrupt_configure_dt failed for button, error:%d", ret);
		return;
	}

	printk("All devices initialzed sucesfully!\n\r");

	/* Initialize the static struct gpio_callback variable   */
    gpio_init_callback(&button_cb_data, button_pressed, BIT(button.pin)); 	
	
	/* Add the callback function by calling gpio_add_callback()   */
	 gpio_add_callback(button.port, &button_cb_data);

	/* Set the initial pwm period and pulse duration, both in ns*/
	 pwm_set_dt(&pwm_led0, PWM_PERIOD, PWM_PERIOD/(5U));

	/* 
 	 * The main loop
 	 */ 
	while (1) {
		/* Just sleep. Led onoff is carrued by int+callback */
		k_msleep(SLEEP_TIME_MS);
	}
}
