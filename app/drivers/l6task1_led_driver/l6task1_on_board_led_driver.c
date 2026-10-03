#include <zephyr/drivers/sensor.h>
#include <zephyr/logging/log.h>
#include <zephyr/kernel.h>
#include <zephyr/drivers/gpio.h>

#include "custom_api_led_sensor.h"

#define DT_DRV_COMPAT l6task1_on_board_led

LOG_MODULE_REGISTER(l6task1_on_board_led, LOG_LEVEL_INF);

struct led_driver_config {
    const struct gpio_dt_spec led;
};

struct led_driver_data {
    unsigned int custom_param;
};

void on_board_led_sensor_set_param(const struct device *dev, unsigned int new_param)
{
    struct led_driver_data *data = dev->data;
    
    data->custom_param = new_param;
    LOG_INF("Custom parameter dynamically updated to: %d", new_param);
}

static int on_board_led_sample_fetch(const struct device *dev, enum sensor_channel chan)
{
    const struct led_driver_config *config = dev->config;
    struct led_driver_data *data = dev->data;

    LOG_INF("Fetching... Current custom_param in RAM is: %d", data->custom_param);
    gpio_pin_set_dt(&config->led, 1);

    return 0;
}

static int on_board_led_channel_get(const struct device *dev, enum sensor_channel chan, struct sensor_value *val)
{
    const struct led_driver_config *config = dev->config;

    LOG_INF("Getting channel.... turning off the LED");
    gpio_pin_set_dt(&config->led, 0);

    if(val != NULL){
        val->val1 = 0;
        val->val2 = 0;
    }

    return 0;
}

static const struct sensor_driver_api on_board_led_api = {
    .sample_fetch = on_board_led_sample_fetch,
    .channel_get = on_board_led_channel_get,
};

static int on_board_led_init(const struct device *dev)
{
    const struct led_driver_config *config = dev->config;

    if (!gpio_is_ready_dt(&config->led)) {
        LOG_ERR("LED GPIO is not ready");
        return -ENODEV;
    }

    return gpio_pin_configure_dt(&config->led, GPIO_OUTPUT_INACTIVE);
}

/* 3. Instantiate the device */
#define ON_BOARD_LED_INIT(inst)                                        \
    static const struct led_driver_config led_config_##inst = {        \
        .led = GPIO_DT_SPEC_INST_GET(inst, gpios),                     \
    };                                                                 \
    static struct led_driver_data led_data_##inst = {                  \
        .custom_param = 0,                                             \
    };                                                                 \
    SENSOR_DEVICE_DT_INST_DEFINE(inst,                                 \
                                 on_board_led_init,                    \
                                 NULL,                                 \
                                 &led_data_##inst,                     \
                                 &led_config_##inst,                   \
                                 POST_KERNEL,                          \
                                 CONFIG_SENSOR_INIT_PRIORITY,          \
                                 &on_board_led_api);

DT_INST_FOREACH_STATUS_OKAY(ON_BOARD_LED_INIT)