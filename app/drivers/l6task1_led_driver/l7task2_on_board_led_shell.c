#include <zephyr/kernel.h>
#include <zephyr/shell/shell.h>
#include <zephyr/device.h>
#include <zephyr/drivers/sensor.h>
#include <stdlib.h> /* Required for strtoul */
#include "custom_api_led_sensor.h"

/* Helper function to dynamically grab our driver instance */
static const struct device *get_led_sensor_device(void)
{
    return DEVICE_DT_GET_ANY(l6task1_on_board_led);
}

/* Subcommand: info */
static int cmd_info(const struct shell *sh, size_t argc, char **argv)
{
    const struct device *dev = get_led_sensor_device();

    if (dev == NULL) {
        shell_error(sh, "Error: No LED sensor device found in devicetree.");
        return -ENODEV;
    }

    shell_print(sh, "--- Sensor Device Info ---");
    shell_print(sh, "Device Name: %s", dev->name);
    shell_print(sh, "State:       %s", device_is_ready(dev) ? "READY" : "NOT READY");

    return 0;
}

/* Subcommand: fetch */
static int cmd_fetch(const struct shell *sh, size_t argc, char **argv)
{
    const struct device *dev = get_led_sensor_device();

    if (!device_is_ready(dev)) {
        shell_error(sh, "Error: Device is not ready");
        return -ENODEV;
    }

    int err = sensor_sample_fetch(dev);
    if (err) {
        shell_error(sh, "Failed to fetch sample (Error %d)", err);
    } else {
        shell_print(sh, "Sample fetched successfully! (LED turned ON)");
    }
    return err;
}

/* Subcommand: read */
static int cmd_read(const struct shell *sh, size_t argc, char **argv)
{
    const struct device *dev = get_led_sensor_device();
    struct sensor_value val;

    if (!device_is_ready(dev)) {
        shell_error(sh, "Error: Device is not ready");
        return -ENODEV;
    }

    int err = sensor_channel_get(dev, SENSOR_CHAN_ALL, &val);
    if (err) {
        shell_error(sh, "Failed to read channel (Error %d)", err);
    } else {
        shell_print(sh, "Channel read successfully! (LED turned OFF)");
        shell_print(sh, "Sensor Value: %d.%06d", val.val1, val.val2);
    }
    return err;
}

/* Subcommand: set <value> */
static int cmd_set(const struct shell *sh, size_t argc, char **argv)
{
    const struct device *dev = get_led_sensor_device();
    char *endptr;
    unsigned long new_param;

    if (!device_is_ready(dev)) {
        shell_error(sh, "Error: Device is not ready");
        return -ENODEV;
    }

    /* Convert string argument to an unsigned long integer */
    new_param = strtoul(argv[1], &endptr, 10);

    /* Validation 1: Ensure the argument was entirely numeric */
    if (*endptr != '\0') {
        shell_error(sh, "Error: Invalid argument '%s'. Must be a positive integer.", argv[1]);
        return -EINVAL;
    }

    /* Validation 2: Ensure the argument falls within a specific range (e.g., 0 to 10000) */
    if (new_param > 10000) {
        shell_error(sh, "Error: Value %lu is out of range. Must be <= 10000.", new_param);
        return -EINVAL;
    }

    /* Call the custom extension API */
    on_board_led_sensor_set_param(dev, (unsigned int)new_param);
    shell_print(sh, "Successfully set custom parameter to: %u", (unsigned int)new_param);

    return 0;
}

/* Define the dictionary of subcommands */
SHELL_STATIC_SUBCMD_SET_CREATE(sub_sensorroot,
    SHELL_CMD(info,   NULL, "Prints the device name and ready state", cmd_info),
    SHELL_CMD(fetch,  NULL, "Calls sensor_sample_fetch()",            cmd_fetch),
    SHELL_CMD(read,   NULL, "Calls sensor_channel_get()",             cmd_read),
    
    /* 
     * SHELL_CMD_ARG Parameters:
     * 1. Command name (set)
     * 2. Subcommands (NULL)
     * 3. Help string
     * 4. Handler function (cmd_set)
     * 5. Mandatory arguments (2 -> argv[0]="set", argv[1]="<value>")
     * 6. Optional arguments (0)
     */
    SHELL_CMD_ARG(set, NULL, "Sets custom param: set <value>", cmd_set, 2, 0),
    
    SHELL_SUBCMD_SET_END
);

/* Register the root command */
SHELL_CMD_REGISTER(sensorroot, &sub_sensorroot, "LED Sensor Driver Commands", NULL);