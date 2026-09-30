#include <sys/syslimits.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>

#define SLEEP_TIME_MS 1000

/* The devicetree node identifier for the "led0" alias. */
#define LED_NODE DT_ALIAS(led0)
#define LED_APP  DT_ALIAS(app_led)

#if 0
static const struct gpio_dt_spec led     = GPIO_DT_SPEC_GET(LED_NODE, gpios);
#endif
static const struct gpio_dt_spec led_app = GPIO_DT_SPEC_GET(LED_APP,  gpios);

LOG_MODULE_REGISTER(main, LOG_LEVEL_INF);

int main(void)
{
    /* states for the LEDs */
#if 0
    bool led_state     = true;
#endif
    bool led_app_state = true;

    /* check if gpio is ready */
#if 0
    if (!gpio_is_ready_dt(&led))
    {
        return 0;
    }
#endif
    if (!gpio_is_ready_dt(&led_app))
    {
        return 0;
    }

    /* configure LED pins */
#if 0
    if (gpio_pin_configure_dt(&led, GPIO_OUTPUT_ACTIVE) < 0)
    {
        return 0;
    }
#endif
    if (gpio_pin_configure_dt(&led_app, GPIO_OUTPUT_ACTIVE) < 0)
    {
        return 0;
    }

    /* MAIN LOOP */
    while (1)
    {
/* disabled to not interfere with Lesson 4 LED */
#if 0
        /* Lesson 2 LED */
        if (gpio_pin_toggle_dt(&led) < 0)
        {
            return 0;
        }

        led_state = !led_state;
        LOG_INF("LED state: %s", led_state ? "ON" : "OFF");
        k_msleep(CONFIG_BLINK_SLEEP_TIME_MS);
#endif

        /* Lesson 4 LED */
        if (gpio_pin_toggle_dt(&led_app) < 0)
        {
           return 0;
        }

        led_app_state = !led_app_state;
        LOG_INF("LED APP state: %s", led_app_state ? "ON" : "OFF");
        k_msleep(CONFIG_APP_HEARTBEAT_PERIOD_MS);
    }

    /* should not be reached */
    return 0;
}
