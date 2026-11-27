#include <zephyr/shell/shell.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/device.h>
#include <zephyr/drivers/sensor.h>

#define SENSOR_NODE DT_NODELABEL(our_driver0)

static const struct device *sensor_dev =
    DEVICE_DT_GET(SENSOR_NODE);

bool led_state1 = true;

#define LED_NODE DT_ALIAS(led0)

static const struct gpio_dt_spec led = GPIO_DT_SPEC_GET(LED_NODE, gpios);

static int cmd_channel_fetch_handler(const struct shell *sh, size_t argc, char **argv)
{
    ARG_UNUSED(argc);
    ARG_UNUSED(argv);

    int ret = sensor_sample_fetch(sensor_dev);

    if (ret < 0) {
        shell_error(sh, "Fetch failed: %d", ret);
        return ret;
    }

    shell_info(sh, "Fetch completed successfully");

    return 0;
}

static int cmd_channel_get_handler(const struct shell *sh, size_t argc, char **argv)
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
}

/* Create subcommands array for "demo" */
SHELL_STATIC_SUBCMD_SET_CREATE(sub_sensor,
SHELL_CMD_ARG(fetch, NULL, "Show sensor information.", cmd_channel_fetch_handler, 1, 0),
SHELL_CMD_ARG(read, NULL, "Get channel.", cmd_channel_get_handler, 2, 0),
SHELL_CMD_ARG(info, NULL, "Show device information.", sensor_info, 1,0),
SHELL_SUBCMD_SET_END
);


SHELL_CMD_REGISTER(sensor, &sub_sensor,"Custom driver test commands", NULL);