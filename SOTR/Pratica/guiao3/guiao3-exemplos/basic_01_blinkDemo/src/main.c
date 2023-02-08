/*
 * Paulo Pedreiras, 2022/09
 * Simple Digital Output example, based on Nordic blinky
 * 
 * Toggles periodically LED1, which is internally connected to P0.13 and labeled led0
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
 * 		Note that the GPIO must be enabled in prj.conf (CONFIG_GPIO=y)
 * 
 */

/* Includes. zephyr.h is always necessary. Add the othert ones according with the resourtces used
 *   In this case it is used the gpio module 
 */ 
#include <zephyr.h>
#include <drivers/gpio.h>

/* Define the sleep time, in ms (roughly half the period) */
#define SLEEP_TIME_MS   100

/* Get led 0 node ID. Refer to the DTS file to find "led0" node label
 *   (could also be obtaind via alias, see below). 
 */
#define LED0_NODE DT_NODELABEL(led0)
//#define LED0_NODE DT_ALIAS(led0) /* Same action, via alias */

/*
 * Get the device pointer, pin number, and configuration flags. A build error on this line means your board is unsupported.
 * See the sample documentation for information on how to fix this.
 */
static const struct gpio_dt_spec led = GPIO_DT_SPEC_GET(LED0_NODE, gpios);

/* 
 * The main function
 */
void main(void)
{
	int ret;

	/* Check if device is ready */
	if (!device_is_ready(led.port)) {
		return;
	}

	/* Configure the GPIO pin for output */
	ret = gpio_pin_configure_dt(&led, GPIO_OUTPUT_ACTIVE);
	if (ret < 0) {
		return;
	}

	/* 
 	 * The main loop
 	 */ 
	while (1) {
		/* Simply toggle the led */
		ret = gpio_pin_toggle_dt(&led);
		if (ret < 0) {
			return;
		}
		k_msleep(SLEEP_TIME_MS);
	}
}
