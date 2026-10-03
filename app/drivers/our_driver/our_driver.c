#include "zephyr/device.h"
#include "zephyr/logging/log_core.h"
#include <stdint.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>

#include "our_driver.h"

#define DT_DRV_COMPAT our_driver

LOG_MODULE_REGISTER(our_driver, LOG_LEVEL_INF);

/* DRIVER STATIC DATA ******************************************************* */
#define LED_APP  DT_ALIAS(app_led)
static const struct gpio_dt_spec led_app = GPIO_DT_SPEC_GET(LED_APP,  gpios);

static bool led_app_state = false;

struct our_driver_data
{
   uint32_t on_state_counter;
   uint32_t some_other_counter;
};

static struct our_driver_data internal_data = {0};

/* DRIVER API FUNCTIONS ***************************************************** */
/* LED ON */
static int sample_fetch_my_impl(
   const struct device * dev,
   enum sensor_channel chan)
{
   LOG_INF("hello from sample fetch (LED ON), channel %d", chan);

   struct our_driver_data *data = dev->data;

   /* turn LED on when it is off */
   if (!led_app_state)
   {
      gpio_pin_toggle_dt(&led_app);
      led_app_state = !led_app_state;
      data->on_state_counter++;
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

/* CUSTOM API *************************************************************** */
void printInternal(const struct device *dev)
{
   struct our_driver_data *data = dev->data;

   printf("LED has been %u times ON.\n", data->on_state_counter);
}

uint32_t getTimesOn(const struct device *dev)
{
   struct our_driver_data *data = dev->data;

   return data->on_state_counter;
}

void resetTimesOn(const struct device *dev)
{
   struct our_driver_data *data = dev->data;

   data->on_state_counter = 0;
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

DEVICE_DT_INST_DEFINE(\
   0,                      \
   init,                   \
   NULL,                   \
   &internal_data,         \
   NULL,                   \
   POST_KERNEL,            \
   80,                     \
   &api_awesome_soft);
