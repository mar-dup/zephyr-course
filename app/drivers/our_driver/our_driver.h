#pragma once

#include "zephyr/device.h"

#ifdef __cplusplus
extern "C" {
#endif

void printInternal(const struct device *dev);

uint32_t getTimesOn(const struct device *dev);

void resetTimesOn(const struct device *dev);

#ifdef __cplusplus
}
#endif