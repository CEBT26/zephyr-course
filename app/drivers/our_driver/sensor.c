#include <zephyr/shell/shell.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/device.h>
#include <zephyr/drivers/sensor.h>

#define SENSOR_NODE DT_NODELABEL(our_driver0)

static const struct device *sensor_dev =
    DEVICE_DT_GET(SENSOR_NODE);

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

static int cmd_channel_get_handler(const struct shell *sh,
                                   size_t argc, char **argv)
{
    ARG_UNUSED(argc);
    ARG_UNUSED(argv);

    struct sensor_value val;

    int ret = sensor_channel_get(sensor_dev,
                                 SENSOR_CHAN_AMBIENT_TEMP,
                                 &val);

    if (ret < 0) {
        shell_error(sh, "Channel get failed: %d", ret);
        return ret;
    }

    shell_info(sh, "Sensor value: val1=%d, val2=%d",
               val.val1, val.val2);

    return 0;
}

static int cmd_sensor_info_handler(const struct shell *sh, size_t argc, char **argv)
{

    ARG_UNUSED(argc);
    ARG_UNUSED(argv);

    shell_info(sh, "Sensor device name: %s", sensor_dev->name);
    shell_info(sh, "Device is ready: %s",
               device_is_ready(sensor_dev) ? "YES" : "NO");
    return 0;
}

/* Create subcommands array for "demo" */
SHELL_STATIC_SUBCMD_SET_CREATE(sub_sensor,
SHELL_CMD_ARG(fetch, NULL, "Show sensor information.", cmd_channel_fetch_handler, 1, 0),
SHELL_CMD_ARG(get, NULL, "Get channel.", cmd_channel_get_handler, 1, 0),
SHELL_CMD_ARG(info, NULL, "Show device information.", cmd_sensor_info_handler, 1,0),
SHELL_SUBCMD_SET_END
);


SHELL_CMD_REGISTER(sensor, &sub_sensor,"Custom driver test commands", NULL);