#pragma once

#include "zephyr/device.h"
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

void printInternal(const struct device *dev);

uint32_t getTimesOn(const struct device *dev);

void resetTimesOn(const struct device *dev);

int setCounter(const struct device *dev, int64_t count);

#ifdef __cplusplus
}
#endif