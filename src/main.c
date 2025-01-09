#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <sevensegment/seven_segment.h>

int main(void)
{
    const struct device *seven_seg_dev = DEVICE_DT_GET(DT_NODELABEL(seven_segment));

    if (!device_is_ready(seven_seg_dev))
    {
        printk("7-segment display device not ready\n");
        return -ENODEV;
    }

    while (1)
    {
        // Display digits 0-9 in a loop
        for (uint8_t digit = 0; digit < 10; digit++)
        {
            int ret = display(seven_seg_dev, &digit);
            if (ret < 0)
            {
                printk("Failed to display digit: %d\n", ret);
            }
            else
            {
                printk("Digit %d displayed successfully\n", digit);
            }
            k_sleep(K_MSEC(2000)); // Pause for 500ms
        }
    }

    return 0;
}
