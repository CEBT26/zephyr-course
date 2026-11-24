/**
 * Our driver header
 */

#ifndef OUR_DRIVER_H
#define OUR_DRIVER_H

#include <zephyr/device.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

struct our_driver_data{
    bool led_state;
 };

int our_driver_set_state(const struct device *dev, bool state);

#ifdef __cplusplus
}
#endif

#endif