#include <zephyr/drivers/gpio.h>
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include <zephyr/drivers/sensor.h>

#include "custom_api_led_sensor.h"

// // #define SLEEP_TIME_MS 500

// /* The devicetree node identifier for the "led0" alias. */
// // #define LED_NODE DT_NODELABEL(red_led)

// #define LED_NODE DT_PATH(leds, led_2)

// // #define APP_LED DT_ALIAS(app_led)

// // #define CONFIG_APP_HEARTBEAT_PERIOD_MS 

// static const struct gpio_dt_spec led = GPIO_DT_SPEC_GET(LED_NODE, gpios);

LOG_MODULE_REGISTER(main, LOG_LEVEL_INF);

// Enable this namespace for l4 and l5 tasks.

// namespace{
//     void test(){
//         const struct device* driver = DEVICE_DT_GET(DT_NODELABEL(our_driver0));
//         // const struct device* driver = DEVICE_DT_GET(DT_INST(0, our_driver));
//         struct sensor_value val;
//         auto ret = sensor_channel_get(driver, SENSOR_CHAN_AMBIENT_TEMP, &val);
//         LOG_INF("sensor_channel_get returned: %d", ret);
//     }
// }

// int main(void)
// {
//     test();
//     bool led_state = true;

//     if (!gpio_is_ready_dt(&led)) return 0;

//     if (gpio_pin_configure_dt(&led, GPIO_OUTPUT_ACTIVE) < 0) return 0;

//     while (1) {
//         if (gpio_pin_toggle_dt(&led) < 0) return 0;

//         led_state = !led_state;
//         LOG_INF("LED state: %s", led_state ? "ON" : "OFF");
//         k_msleep(CONFIG_APP_HEARTBEAT_PERIOD_MS);
//     }
//     return 0;
// }

// // Enable this below int main for l6task1 and l6task2.
// int main(void)
// {
//     const struct device *led_sensor = DEVICE_DT_GET_ANY(l6task1_on_board_led);

//     struct sensor_value val;

//     if(!device_is_ready(led_sensor)) {
//         LOG_ERR("LED sensor device is not ready");
//         return -ENODEV;
//     }

//     while(1)
//     {
//         sensor_sample_fetch(led_sensor);
//         k_msleep(1000);

//         on_board_led_sensor_set_param(led_sensor, k_uptime_get_32());

//         sensor_channel_get(led_sensor, SENSOR_CHAN_ALL, &val);
//         k_msleep(1000);

//         // LOG_INF("Sensor value: %d.%06d", val.val1, val.val2);
//     }
//     return 0;
// }

// Enable this below int main for l7task1.
int main(void)
{
    const struct device *led_sensor = DEVICE_DT_GET_ANY(l6task1_on_board_led);

    if(!device_is_ready(led_sensor)) {
        LOG_ERR("LED sensor device is not ready");
        return -ENODEV;
    }

    LOG_INF("LED Sensor initialized. Awaiting shell commands...");
    LOG_INF("Type 'sensorroot fetch' to turn ON, 'sensorroot read' to turn OFF.");

    /* 
     * The main thread now just sleeps forever. 
     * The Zephyr shell runs in its own background thread, 
     * allowing you to trigger the sensor driver manually.
     */
    while(1)
    {
        k_sleep(K_FOREVER);
    }
    
    return 0;
}