# Libraries Used in IoT-Based Pulse Monitoring System

This document lists all the libraries used in the project, along with a brief description and installation instructions. These libraries are required for the Arduino Uno + ESP8266 code to function properly.

## 1. SoftwareSerial

**Purpose:** Allows serial communication on other digital pins of the Arduino Uno, enabling communication with the ESP8266 WiFi module while still using the default Serial for debugging.
**Installation:** Pre-installed with Arduino IDE.
**Include Statement:**

```cpp
#include <SoftwareSerial.h>
```


## 2. LiquidCrystal_I2C

**Purpose:** Used to interface with I2C-based 16x2 LCD displays. Handles text display for BPM and system status.
**Installation:** Install via Arduino Library Manager (`LiquidCrystal I2C` by Frank de Brabander or similar).
**Include Statement:**

```cpp
#include <LiquidCrystal_I2C.h>
```

## 3. Wire

**Purpose:** Provides I2C communication support required by the LCD module.
**Installation:** Pre-installed with Arduino IDE.
**Include Statement:**

```cpp
#include <Wire.h>
```


## 4. stdlib (Standard C Library)

**Purpose:** Provides basic functions like `abs()`, `rand()`, and memory management utilities. Used internally by the Arduino framework.
**Installation:** Comes by default with Arduino toolchain.
**Include Statement:**

```cpp
#include <stdlib.h>
```

## Notes

* Ensure all libraries are updated to their latest versions via Arduino Library Manager.
* The **ESP8266 AT firmware** must be flashed and functional on the ESP8266 module for proper serial communication.
* `Wire.h` is indirectly required by `LiquidCrystal_I2C` and included automatically, but explicitly adding it is a good practice.