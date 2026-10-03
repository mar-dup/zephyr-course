#include "zephyr/device.h"
#include "zephyr/logging/log_core.h"
#include <zephyr/drivers/sensor.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>

#define DT_DRV_COMPAT our_driver

LOG_MODULE_REGISTER(our_driver, LOG_LEVEL_INF);

/* DRIVER STATIC DATA ******************************************************* */
#define LED_APP  DT_ALIAS(app_led)
static const struct gpio_dt_spec led_app = GPIO_DT_SPEC_GET(LED_APP,  gpios);

static bool led_app_state = false;

/* DRIVER API FUNCTIONS ***************************************************** */
/* LED ON */
static int sample_fetch_my_impl(
   const struct device * dev,
   enum sensor_channel chan)
{
   LOG_INF("hello from sample fetch (LED ON), channel %d", chan);

   /* turn LED on when it is off */
   if (!led_app_state)
   {
      gpio_pin_toggle_dt(&led_app);
      led_app_state = !led_app_state;
   }

   return 0;
}
/* turn LED off */
static int channel_get_my_impl(
    const struct device *dev,
    enum sensor_channel chan,
    struct sensor_value *val)
{
   LOG_INF("hello from channel get (LED OFF), channel %d", chan);

   if (led_app_state)
   {
      gpio_pin_toggle_dt(&led_app);
      led_app_state = !led_app_state;
   }

   return 0;
}

/* REGISTER API ************************************************************* */
static DEVICE_API(sensor, api_awesome_soft) = {
   .channel_get  = channel_get_my_impl,         /* LED OFF */
   .sample_fetch = sample_fetch_my_impl,        /* LED ON */
};

/* INIT FUNCTION ************************************************************ */
static int init(
    const struct device *dev)
{
   if (!gpio_is_ready_dt(&led_app))
   {
      return 0;
   }

   if (gpio_pin_configure_dt(&led_app, GPIO_OUTPUT_ACTIVE) < 0)
   {
      return 0;
   }

   LOG_INF("Device initialized");

   return 0;
}

DEVICE_DT_INST_DEFINE(     \
   0,                      \
   init,                   \
   NULL,                   \
   NULL,                   \
   NULL,                   \
   POST_KERNEL,            \
   80,                     \
   &api_awesome_soft);