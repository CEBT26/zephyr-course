#include <zephyr/shell/shell.h>
#include <zephyr/drivers/gpio.h>

//bool led_state1 = true;

#define LED_NODE DT_ALIAS(led0)

static const struct gpio_dt_spec led = GPIO_DT_SPEC_GET(LED_NODE, gpios);

/*static int sensor_sample_fetch(const struct shell *sh, size_t argc, char **argv)
{
    shell_info(sh, "Fetching sample since shell: toggling LED");
    return 0;
}

static int sensor_channel_get(const struct shell *sh, size_t argc, char **argv)
{
    shell_info(sh, "Hello since shell channel: %s", led_state1 ? "ON" : "OFF");
    led_state1 = led_state1 ? 1 : 0;
    led_state1 = !led_state1;
    return 0;
}

static int sensor_info(const struct shell *sh, size_t argc, char **argv)
{
    shell_info(sh, "LED since shell, GPIO: %s, Pin: %d",led.port->name, led.pin);
    return 0;
}*/

static int sensor_set(const struct shell *sh, size_t argc, char **arg)
{
    //Identidy the node created in dts file with the name our_driver0
    const struct device* driver = DEVICE_DT_GET(DT_NODELABEL(our_driver0));

    if (!device_is_ready(driver)) {
        shell_error(sh, "Device not ready");
        return 0;
    }

    gpio_pin_set_dt(&led, 1);
    shell_info(sh, "LED ON");
    return 0;
}

static int sensor_unset(const struct shell *sh, size_t argc, char **arg)
{
    //Identidy the node created in dts file with the name our_driver0
    const struct device* driver = DEVICE_DT_GET(DT_NODELABEL(our_driver0));

    if (!device_is_ready(driver)) {
        shell_error(sh, "Device not ready");
        return 0;
    }

    gpio_pin_set_dt(&led, 0);
    shell_info(sh, "LED OFF");
    return 0;
}

/* Create subcommands array for "demo" */
SHELL_STATIC_SUBCMD_SET_CREATE(sub_sensor,
//SHELL_CMD_ARG(fetch, NULL, "Show sensor information.", sensor_sample_fetch, 1, 0),
//SHELL_CMD_ARG(read, NULL, "Show channel .", sensor_channel_get, 1, 0),
//SHELL_CMD_ARG(info, NULL, "Show device information.", sensor_info, 1,0),
SHELL_CMD_ARG(set, NULL, "Set led state.", sensor_set, 1,0),
SHELL_CMD_ARG(unset, NULL, "Unset led state.", sensor_unset, 1,0),
SHELL_SUBCMD_SET_END
);


SHELL_CMD_REGISTER(sensor, &sub_sensor,"Custom driver test commands", NULL);