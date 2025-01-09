/*
 * Copyright (c) 2024
 * SPDX-License-Identifier: Apache-2.0
 */

#define DT_DRV_COMPAT seven_segment

#include <zephyr/drivers/gpio.h>
#include <zephyr/sys/util.h>
#include <zephyr/logging/log.h>
#include <sevensegment/seven_segment.h>

LOG_MODULE_REGISTER(servo, CONFIG_LOCK_LOG_LEVEL);
//  0      1    2     3     4      5    6     7     8      9
uint8_t cc_digits[] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F};
uint8_t ca_digits[] = {0xC0, 0xF9, 0xA4, 0xB0, 0x99, 0x92, 0x82, 0xF8, 0x80, 0x90};

enum DRIVE_MODE
{
    COMMON_CATHODE,
    COMMON_ANODE,
};
struct seven_segment_config
{
    struct gpio_dt_spec pins[7];
    int32_t drive_mode;
    int32_t refresh_rate;
    struct gpio_dt_spec common_pin;
};

/*******************************************************************************
 * Private
 ******************************************************************************/
static int digit_display(const struct device *dev, const uint8_t *digit)
{
    const struct seven_segment_config *config = dev->config;

    if (ARRAY_SIZE(config->pins) != 7)
        return -EINVAL;

    if (*digit < 0 || *digit > 0xF)
        return -EAGAIN;

    const uint8_t value = (config->drive_mode == COMMON_ANODE) ? ca_digits[*digit] : cc_digits[*digit];

    /* -1 for DP which is ignored */
    const uint8_t NUMBER_OF_BITS = sizeof(uint8_t) * 8 - 1;
    // 00000111
    // xabcdefg
    // 01110000
    for (int i = 0; i < NUMBER_OF_BITS; ++i)
    {
        int ret = gpio_pin_set_dt(&config->pins[i], (value >> i) & 1);
        if (ret < 0)
        {
            printk("Err");
            return ret;
        }
    }

    return 0;
}

/*******************************************************************************
 * API
 ******************************************************************************/

int display(const struct device *dev, const uint8_t *digit)
{
    return digit_display(dev, digit);
}

static const struct seven_segment_driver_api seven_segment_api = {
    .digit_display = display,
};

/*******************************************************************************
 * INIT
 ******************************************************************************/

static int seven_segment_init(const struct device *dev)
{
    const struct seven_segment_config *config = dev->config;
    int ret;

    // Configure each pin
    for (int i = 0; i < ARRAY_SIZE(config->pins); ++i)
    {
        if (!device_is_ready(config->pins[i].port))
        {
            LOG_ERR("GPIO controller for pin %d not ready", i);
            return -ENODEV;
        }

        ret = gpio_pin_configure_dt(&config->pins[i], GPIO_OUTPUT);
        if (ret < 0)
        {
            LOG_ERR("Failed to configure pin %d", i);
            return ret;
        }
    }

    // Configure the common pin
    if (!device_is_ready(config->common_pin.port))
    {
        LOG_ERR("GPIO controller for common pin not ready");
        return -ENODEV;
    }

    ret = gpio_pin_configure_dt(&config->common_pin, GPIO_OUTPUT);
    if (ret < 0)
    {
        LOG_ERR("Failed to configure common pin");
        return ret;
    }

    return 0;
}

#define SEVEN_SEGMENT_DEFINE(i)                                           \
    static const struct seven_segment_config seven_segment_config_##i = { \
        .pins = {                                                         \
            GPIO_DT_SPEC_INST_GET_BY_IDX(i, segment_gpios, 0),            \
            GPIO_DT_SPEC_INST_GET_BY_IDX(i, segment_gpios, 1),            \
            GPIO_DT_SPEC_INST_GET_BY_IDX(i, segment_gpios, 2),            \
            GPIO_DT_SPEC_INST_GET_BY_IDX(i, segment_gpios, 3),            \
            GPIO_DT_SPEC_INST_GET_BY_IDX(i, segment_gpios, 4),            \
            GPIO_DT_SPEC_INST_GET_BY_IDX(i, segment_gpios, 5),            \
            GPIO_DT_SPEC_INST_GET_BY_IDX(i, segment_gpios, 6),            \
        },                                                                \
        .drive_mode = DT_INST_PROP(i, drive_mode),                        \
        .refresh_rate = DT_INST_PROP(i, refresh_rate),                    \
        .common_pin = GPIO_DT_SPEC_INST_GET(i, common_gpios),             \
    };                                                                    \
    DEVICE_DT_INST_DEFINE(i, seven_segment_init, NULL, NULL,              \
                          &seven_segment_config_##i, POST_KERNEL,         \
                          CONFIG_KERNEL_INIT_PRIORITY_DEVICE,             \
                          &seven_segment_api);
DT_INST_FOREACH_STATUS_OKAY(SEVEN_SEGMENT_DEFINE)
