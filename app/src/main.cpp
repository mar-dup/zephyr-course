#include <sys/syslimits.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>

#define SLEEP_TIME_MS 5000

int main(void)
{
    /* states for the LEDs */
    /* MAIN LOOP */
    while (1)
    {
        printf("Hello World! %s\n", CONFIG_BOARD_TARGET);

        k_msleep(SLEEP_TIME_MS);
    }

    /* should not be reached */
    return 0;
}
