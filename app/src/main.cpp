#include <zephyr/drivers/gpio.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include <zephyr/device.h>
//#include "../drivers/our_driver/our_driver.h"
#include "our_driver.h"

#define SLEEP_TIME_MS 1000

/* The devicetree node identifier for the "led0" alias. */
//#define LED_NODE DT_ALIAS(led0)

//static const struct gpio_dt_spec led = GPIO_DT_SPEC_GET(LED_NODE, gpios);

LOG_MODULE_REGISTER(main, LOG_LEVEL_INF);

int main(void)
{
    //Identidy the node created in dts file with the name our_driver0
    const struct device* driver = DEVICE_DT_GET(DT_NODELABEL(our_driver0));
    //Measured value container
    struct sensor_value val;

    if (!device_is_ready(driver)) {
        return 0;
    }

    while (1)
    {

        // Driver Extension API: Turn ON LED
        our_driver_set_state(driver, true);

        // sensor_channel_get_impl()
        sensor_channel_get(driver, SENSOR_CHAN_AMBIENT_TEMP, &val);
        
        LOG_INF("LED state: %d", val.val1);

        k_msleep(CONFIG_BLINK_SLEEP_TIME_MS);

        // Driver Extension API: Turn OFF LED
        our_driver_set_state(driver, false);

        // Read LED state
        sensor_channel_get(driver,
                           SENSOR_CHAN_AMBIENT_TEMP,
                           &val);

        LOG_INF("LED state: %d", val.val1);

        k_msleep(CONFIG_BLINK_SLEEP_TIME_MS);
    }
    return 0;
}