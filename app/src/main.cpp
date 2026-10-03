#include <sys/syslimits.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include "zephyr/drivers/sensor.h"
//#include "zephyr/device.h"

#define SLEEP_TIME_MS 2000

LOG_MODULE_REGISTER(main, LOG_LEVEL_INF);

/* MAIN ********************************************************************* */
int main(void)
{
    /* get device */
    const struct device * driver = DEVICE_DT_GET(DT_NODELABEL(our_driver0));
    struct sensor_value val;

    int ret = 0xff;


    printf("Hello World! %s\n", CONFIG_BOARD_TARGET);
    /* MAIN LOOP */
    while (1)
    {
        /* turn on  - sleep */
        LOG_INF("turn LED on");
        ret = sensor_sample_fetch(driver);
        LOG_INF("ret: %d\n", ret);
        k_msleep(SLEEP_TIME_MS);

        /* turn off - sleep */
        LOG_INF("turn LED off");
        ret = sensor_channel_get(driver, SENSOR_CHAN_AMBIENT_TEMP, &val);
        LOG_INF("ret: %d\n", ret);
        k_msleep(SLEEP_TIME_MS + 1000);

    }

    /* should not be reached */
    return 0;
}
