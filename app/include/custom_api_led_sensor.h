#ifndef CUSTOM_API_LED_SENSOR_H
#define CUSTOM_API_LED_SENSOR_H

#include <zephyr/device.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Ensure this exact name matches the definition in your driver's .c file */
void on_board_led_sensor_set_param(const struct device *dev, unsigned int new_param);

#ifdef __cplusplus
}
#endif

#endif // CUSTOM_API_LED_SENSOR_H