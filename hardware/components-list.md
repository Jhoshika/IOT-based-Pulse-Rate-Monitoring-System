# Components List – IoT-Based Pulse Monitoring System

This document lists all the hardware components used in the project along with their quantities and descriptions.

| Component Name                   | Quantity  | Description                                                                 |
| -------------------------------- | --------- | --------------------------------------------------------------------------- |
| Arduino Uno                      | 1         | Main microcontroller board used to read pulse sensor data and control LCD.  |
| ESP8266 WiFi Module              | 1         | Provides WiFi connectivity and sends BPM data to ThingSpeak cloud platform. |
| Pulse Sensor (Heartbeat Sensor)  | 1         | Detects heartbeat pulses and outputs analog signal to Arduino.              |
| LCD Display 16x2 (I2C, PCF8574)  | 1         | Displays BPM readings and system status locally.                            |
| LED (Blink Indicator)            | 1         | Blinks with each detected heartbeat for visual feedback.                    |
| Buzzer                           | 1         | Provides audible alert when BPM is abnormal.                                |
| Breadboard                       | 1         | Used for prototyping and connecting all components neatly.                  |
| Resistors (e.g., 10kΩ)           | As needed | Used for pull-up or signal conditioning if required.                        |
| Connecting Wires / Jumper Cables | As needed | For making connections between Arduino, ESP8266, sensor, and LCD.           |
| External 5V Power Supply         | 1         | Provides stable power to Arduino Uno, ESP8266, and other components.        |

---

## Notes:

* **Arduino Uno** is the central controller handling signal processing, BPM calculation, and communication with the ESP8266.
* **Pulse Sensor** connects to an analog pin (A0) of Arduino. Ensure proper placement on fingertip/earlobe for accurate readings.
* **ESP8266 Module** uses AT commands via `SoftwareSerial` to connect to WiFi and upload data to ThingSpeak.
* **LCD Display (I2C)** reduces pin usage (only SDA + SCL) and shows BPM + status ("Normal" or "Abnormal").
* **LED Indicator** provides immediate visual cue for each detected heartbeat.
* **Buzzer** can be triggered when BPM goes out of the safe range.
* **External Power Supply** is recommended, since powering ESP8266 + LCD + Arduino from USB alone may cause resets.

