#include <zephyr/shell/shell.h>
#include <zephyr/drivers/gpio.h>

bool led_state1 = true;

#define LED_NODE DT_ALIAS(led0)

static const struct gpio_dt_spec led = GPIO_DT_SPEC_GET(LED_NODE, gpios);

static int sensor_sample_fetch(const struct shell *sh, size_t argc, char **argv)
{
    shell_info(sh, "Fetching sample: toggling LED");
    return 0;
}

static int sensor_channel_get(const struct shell *sh, size_t argc, char **argv)
{
    shell_info(sh, "Hello since channel: %s", led_state1 ? "ON" : "OFF");
    led_state1 = led_state1 ? 1 : 0;
    led_state1 = !led_state1;
    return 0;
}

static int sensor_info(const struct shell *sh, size_t argc, char **argv)
{
    shell_info(sh, "LED, GPIO: %s, Pin: %d",led.port->name, led.pin);
    return 0;
}

/* Create subcommands array for "demo" */
SHELL_STATIC_SUBCMD_SET_CREATE(sub_sensor,
SHELL_CMD(fetch, NULL, "Show sensor information.", sensor_sample_fetch),
SHELL_CMD(read, NULL, "Show channel .", sensor_channel_get),
SHELL_CMD(info, NULL, "Show device information.", sensor_info),
SHELL_SUBCMD_SET_END
);


SHELL_CMD_REGISTER(sensor, &sub_sensor,"Custom driver test commands", NULL);