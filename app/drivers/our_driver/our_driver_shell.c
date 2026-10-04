#include "zephyr/drivers/sensor.h"
#include <zephyr/shell/shell.h>
#include <sys/errno.h>

/* HANDLER ****************************************************************** */
//static int cmd_channel_get_handler(
static int cmd_print_demo(
    const struct shell *sh,
    int argc,
    char** argv)
{
    shell_info(sh, "hello from print_demo");
    shell_info(sh, "    arg nums: %d", argc);
    if (argc >= 1) {
        shell_info(sh, "    arg 1   : %s", argv[0]);
    }
    shell_info(sh, "---------------------");

    shell_print(sh, "shell_print");
    shell_info(sh, "shell_info");
    shell_warn(sh, "shell_warn");
    shell_error(sh, "shell_error");
    shell_fprintf(sh, SHELL_INFO, "shell_fprintf with SHELL_INFO\n");
    shell_print(sh, "----------------------------------------");

    return 0;
}

/*
 *  Lesson 07 Task 1 functions
 */
/* Turn LED ON */
static int cmd_fetch(
    const struct shell *sh,
    int argc,
    char** argv)
{
    shell_info(sh, "FETCH function");

    /* we do not need to check the ARGC, this is done for us */

//  if (argc != 2) {
//      shell_error(sh, "Missing argument");

//      return EINVAL;
//  }

    const struct device *dev = shell_device_get_binding(argv[1]);
    if (NULL == dev)
    {
        shell_error(sh, "could not find %s", argv[1]);

        return EINVAL;
    }

    int ret = sensor_sample_fetch(dev);
    if (ret != 0)
    {
        shell_error(sh, "Got error code \"%d\" of sensor_sample_fetch", ret);
        return EFAULT;
    }

    return 0;
}

/* Turn LED OFF */
static int cmd_read(
    const struct shell *sh,
    int argc,
    char** argv)
{
    shell_info(sh, "READ function");

    const struct device *dev = shell_device_get_binding(argv[1]);
    if (NULL == dev)
    {
        shell_error(sh, "could not find %s", argv[1]);

        return EINVAL;
    }

    struct sensor_value val;
    int ret = sensor_channel_get(dev, SENSOR_CHAN_AMBIENT_TEMP, &val);

    if (ret != 0)
    {
        shell_error(sh, "Got error code \"%d\" of sensor_sample_fetch", ret);
        return EFAULT;
    }

    return 0;
}

/* print device name and ready state */
static int cmd_info(
    const struct shell *sh,
    int argc,
    char** argv)
{
    shell_info(sh, "INFO function");

    const struct device *dev = shell_device_get_binding(argv[1]);
    if (NULL == dev)
    {
        shell_error(sh, "could not find %s", argv[1]);

        return EINVAL;
    }

    shell_info(sh, "Device: \"%s\"", dev->name);
    shell_info(sh, "State : \"%s\"",
        dev->state->initialized? "Initialized" : "is not initialized");


    return 0;
}

/* REGISTER COMMANDS ******************************************************** */
SHELL_STATIC_SUBCMD_SET_CREATE(our_driver_subcmd,
        SHELL_CMD_ARG(\
            print_demo,\
            NULL,\
            "Show different prints",\
            cmd_print_demo,\
            1,\
            0),
        SHELL_CMD_ARG(\
            fetch,\
            NULL,\
            "Use our_drivers fetch function (turn LED ON)",\
            cmd_fetch,\
            2,\
            0),
        SHELL_CMD_ARG(\
            read,\
            NULL,\
            "Use our_drivers channel get function (turn LED OFF)",\
            cmd_read,\
            2,\
            0),
        SHELL_CMD_ARG(\
            info,\
            NULL,\
            "Get Info about the given device",\
            cmd_info,\
            2,\
            0),
        SHELL_SUBCMD_SET_END,
);

SHELL_CMD_REGISTER(\
    sensor,\
    &our_driver_subcmd,\
    "sensor shell",\
    NULL);
