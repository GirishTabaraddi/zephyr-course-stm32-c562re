#include <zephyr/shell/shell.h>
#include <zephyr/drivers/sensor.h>

static int cmd_channel_get_handler(const struct shell *shell, size_t argc, char **argv)
{
    const struct device *dev = shell_device_get_binding(argv[1]);

    if(!dev)
    {
        shell_error(shell, "Device not found: %s", argv[1]);
        return -ENODEV;
    }

    struct sensor_value val;

    int ret = sensor_channel_get(dev, SENSOR_CHAN_ALL, &val);

    if(ret < 0)
    {
        shell_error(shell, "sensor_channel_get failed: %d", ret);
        return ret;
    }

    shell_info(shell, "value : %d.%06d", val.val1);

    return 0;
}

SHELL_STATIC_SUBCMD_SET_CREATE(our_driver_subcmd,
    SHELL_CMD_ARG(channel_get, NULL, "Get Channel of Our Driver", cmd_channel_get_handler, 2, 0),
    SHELL_SUBCMD_SET_END,
);

SHELL_CMD_REGISTER(our_driver, &our_driver_subcmd, "Our Driver Commands", NULL);

