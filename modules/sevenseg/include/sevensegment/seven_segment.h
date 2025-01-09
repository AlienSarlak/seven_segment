/*
 * Copyright (c) 2024
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#ifndef APP_INCLUDE_APP_DRIVERS_7SEG_H_
#define APP_INCLUDE_APP_DRIVERS_7SEG_H_

#include <zephyr/device.h>
#include <zephyr/toolchain.h>

/** @cond INTERNAL_HIDDEN */

typedef int (*digit_display_t)(const struct device *dev, const uint8_t *digit);

struct seven_segment_driver_api
{
    digit_display_t digit_display;
};

/** @endcond */

/**
 * @brief Display a digit on the 7-segment display.
 *
 * @param dev 7-segment device instance.
 * @param digit Pointer to the digit to be displayed.
 *
 * @retval 0 On success.
 * @retval -errno Negative errno in case of failure.
 */
int display(const struct device *dev, const uint8_t *digit);

// static inline int z_impl_display(const struct device *dev, const uint8_t *digit)
// {
// }

#endif /* APP_INCLUDE_APP_DRIVERS_7SEG_H_ */
