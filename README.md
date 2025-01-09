# **Seven-Segment Display Driver as an External Module for Zephyr**

This repository provides a **working example** of a **seven-segment display driver** implemented as an **external module** for the **Zephyr RTOS**. The project demonstrates how to integrate an **out-of-tree driver** into Zephyr using its external module system.

---

## **Repository Structure**
```
├── boards
│   └── apollo3_evb.overlay      # Device Tree overlay for the Apollo3 board
├── CMakeLists.txt               # Main CMake configuration
├── modules
│   ├── sevenseg                 # Seven-segment driver module
│   │   ├── CMakeLists.txt        # Module-level CMake configuration
│   │   ├── drivers               # Driver source files
│   │   │   ├── CMakeLists.txt     # Driver build setup
│   │   │   ├── Kconfig            # Driver configuration options
│   │   │   └── sevensegment       # Driver implementation
│   │   │       ├── CMakeLists.txt
│   │   │       ├── Kconfig
│   │   │       └── seven_segment.c # Seven-segment driver code
│   │   ├── dts                    # Device Tree files
│   │   │   └── bindings
│   │   │       └── sevensegment
│   │   │           └── seven-segment.yml # Device Tree binding for seven-segment display
│   │   ├── include
│   │   │   ├── CMakeLists.txt
│   │   │   └── sevensegment
│   │   │       └── seven_segment.h # Header file for the driver API
│   │   ├── Kconfig                 # Module-level configuration
│   │   └── west.yml                 # West manifest file
│   └── zephyr
│       └── module.yml               # Zephyr module registration
├── prj.conf                         # Zephyr project configuration
└── src
    └── main.c                        # Application demonstrating the seven-segment driver
```

---

## **Features**
 Implements an **out-of-tree driver** for a seven-segment display
 Uses **Zephyr's external module system** for integration
 Demonstrates **Device Tree usage** for hardware configuration
 Provides a simple **application (`main.c`)** to interact with the display

---

## **Getting Started**
### **1. Clone the Repository**
```sh
git clone <repository-url>
cd <repository-name>
```

### **2. Initialize `west` (Zephyr’s meta-tool)**
Ensure you have **Zephyr installed** and initialize the project:
```sh
west init -l .
west update
```

### **3. Build and Flash the Application**
```sh
west build -b apollo3_evb
west flash
```

### **4. Running the Application**
Once flashed, the application should:
- Initialize the seven-segment display
- Display test patterns or numbers on the display

Monitor output logs using:
```sh
west log
```

---

## **How It Works**
### **Device Tree Integration**
The seven-segment display is **defined in the Device Tree (`apollo3_evb.overlay`)**:
```dts
/ {
    seven_segment: seven_segment {
        compatible = "seven-segment";
        segment-gpios =
                <&gpio0_31 10 GPIO_ACTIVE_HIGH>,
                <&gpio0_31 28 GPIO_ACTIVE_HIGH>,
                <&gpio0_31 16 GPIO_ACTIVE_HIGH>,
                <&gpio0_31 18 GPIO_ACTIVE_HIGH>,
                <&gpio0_31 19 GPIO_ACTIVE_HIGH>,
                <&gpio0_31 30 GPIO_ACTIVE_HIGH>,
                <&gpio0_31 15 GPIO_ACTIVE_HIGH>;
        drive-mode = <0>; /* 0: Common Cathode, 1: Common Anode */
        common-gpios = <&gpio0_31 5 GPIO_ACTIVE_HIGH>;
        refresh-rate = <0>;
        status="okay";
    };
};

```

### **Application Code (`main.c`)**
The application **initializes** the seven-segment display and **displays a sequence**:
```c
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
```

---

## **Customization**
To modify the **GPIO pins**, edit `boards/apollo3_evb.overlay`.
To add **new features**, modify `seven_segment.c` and `seven_segment.h`.

---

## **License**
This project is licensed under the **Apache 2.0 License**.

---
