#include <zephyr/kernel.h>
#include <zephyr/shell/shell.h>
#include <zephyr/device.h>
#include <zephyr/drivers/sensor.h>

/* Helper function to dynamically grab our driver instance */
static const struct device *get_led_sensor_device(void)
{
    /* Finds the first devicetree node with compatible = "l6task1,on-board-led" */
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

    bool ready = device_is_ready(dev);
    
    shell_print(sh, "--- Sensor Device Info ---");
    shell_print(sh, "Device Name: %s", dev->name);
    shell_print(sh, "State:       %s", ready ? "READY" : "NOT READY");

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

    /* Call the driver's sample_fetch API */
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

    /* Call the driver's channel_get API */
    int err = sensor_channel_get(dev, SENSOR_CHAN_ALL, &val);
    
    if (err) {
        shell_error(sh, "Failed to read channel (Error %d)", err);
    } else {
        shell_print(sh, "Channel read successfully! (LED turned OFF)");
        shell_print(sh, "Sensor Value: %d.%06d", val.val1, val.val2);
    }

    return err;
}

/* Define the dictionary of subcommands */
SHELL_STATIC_SUBCMD_SET_CREATE(sub_sensorroot,
    SHELL_CMD(info,   NULL, "Prints the device name and ready state", cmd_info),
    SHELL_CMD(fetch,  NULL, "Calls sensor_sample_fetch()",            cmd_fetch),
    SHELL_CMD(read,   NULL, "Calls sensor_channel_get()",             cmd_read),
    SHELL_SUBCMD_SET_END /* Must terminate the array */
);

/* Register the root command "sensorroot" to the Zephyr Shell */
SHELL_CMD_REGISTER(sensorroot, &sub_sensorroot, "LED Sensor Driver Commands", NULL);