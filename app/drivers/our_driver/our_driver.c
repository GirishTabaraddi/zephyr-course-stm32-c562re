#include <zephyr/drivers/sensor.h>
#include <zephyr/logging/log.h>

#define DT_DRV_COMPAT our_driver

LOG_MODULE_REGISTER(our_driver, LOG_LEVEL_INF);

static int channel_get_my_imp(const struct device *dev, enum sensor_channel chan, struct sensor_value *val)
{
    LOG_INF("Hello from channel_get_my_imp : %d", chan);

    return 0;
}

static DEVICE_API(sensor, api_iomico_demo) = {
    .channel_get = channel_get_my_imp,
};

static int init(const struct device* dev)
{
    LOG_INF("Device Initialized");
    return 0;
}

#define DEV_INST(inst) DEVICE_DT_INST_DEFINE(inst, init, NULL, NULL, NULL, POST_KERNEL, 80, &api_iomico_demo);

DT_INST_FOREACH_STATUS_OKAY(DEV_INST);