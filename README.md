# greencue
Arduino project daylight responsive smart lighting and energy tracking system
# Smart Room Activity & Energy Cue System

An Arduino-based IoT prototype designed to optimize room energy consumption through automated occupancy detection (PIR) and daylight harvesting (LDR), featuring a manual override push button and real-time energy tracking via an I2C LCD display.

## Board & Runtime
* **Microcontroller:** Arduino Uno (ATmega328P)
* **Environment:** Arduino IDE 2.x / Wokwi Web Simulator

## Dependencies
* `Wire.h` (Built-in Arduino Wire library)
* `LiquidCrystal_I2C` (by Frank de Brabander or Marco Schwartz — installable via the Arduino Library Manager)

## Component List & Wiring Summary
* **PIR Motion Sensor:** Connected to Digital Pin 2
* **LDR Photoresistor Module:** Connected to Analog Pin A0 (with a pull-down resistor forming a voltage divider)
* **Push Button (Manual Override):** Connected to Digital Pin 3 (`INPUT_PULLUP`)
* **Red LED (Active Light):** Connected to Digital Pin 13 (via 220Ω resistor)
* **Green LED (Eco-Mode Light):** Connected to Digital Pin 12 (via 220Ω resistor)
* **I2C LCD 16x2 Display:** SDA $\rightarrow$ A4, SCL $\rightarrow$ A5

## Setup, Run & Demonstration Instructions
1. Clone this repository or download the `.ino` source file.
2. Open the file inside the Arduino IDE or import it into the [Wokwi Simulator](https://wokwi.com/).
3. Ensure the `LiquidCrystal_I2C` library is installed in your IDE libraries.
4. Compile and upload the sketch to your Arduino Uno board or run the virtual simulation to test the three operating states:
   * **Dark Occupancy:** Triggers Red LED and energy accumulation.
   * **Daylight Eco-Mode:** Triggers Green LED for daylight harvesting.
   * **Manual Override:** Pressing the push button forces the active state.
