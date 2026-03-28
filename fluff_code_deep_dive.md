
\newpage

# Chapter 14: Deep Code Analysis & Firmware Logic

This chapter provides a rigorous, line-by-line deconstruction of the firmware architecture. To ensure robust operation in a safety-critical environment like an auditorium, every instruction cycle must be accounted for.

## 14.1 Pre-Processor Directives & Macros

```cpp
#include <LiquidCrystal.h>
#include <DHT.h>
```
**Analysis:**
The inclusion of `LiquidCrystal.h` brings in the driver stack for the HD44780 LCD controller. This library manages the complex timing requirements of the 16-pin LCD interface, specifically the 4-bit nibble transmission mode used here to save pins. The `DHT.h` library handles the single-wire protocol of the DHT11 sensor, which involves microsecond-precise timing to decode the 40-bit data packet (16-bit humidity + 16-bit temperature + 8-bit checksum).

```cpp
#define DHTPIN 13
#define DHTTYPE DHT11
#define FLAME_PIN 9
#define BUZZER_PIN 10
```
**Analysis:**
We utilize C-style `#define` macros rather than `const int` variables.
*   **Memory Optimization:** Macros are processed by the pre-processor before compilation. They replace the text directly, meaning they consume **zero** SRAM bytes. In contrast, integer variables would occupy 2 bytes each. On an ATmega328P with only 2KB or SRAM, saving 8 bytes is significant.
*   **Pin Selection Strategy:**
    *   **Pin 13 (DHT):** Is decoupled from the onboard LED to prevent signal interference.
    *   **Pin 9 (Flame):** Chosen for its proximity to other digital inputs.

```cpp
#define RELAY1 A0 // 2 Fans
#define RELAY2 A1 // 1 Fan
#define RELAY3 A2 // All LEDs (LDR Reversed)
#define RELAY4 A3 // Exhaust Fan (Temp)
```
**Analysis:**
The Relay control pins are assigned to the Analog Port (A0-A3).
*   **Port Manipulation:** This allows for potential future optimization using "Direct Port Manipulation" (DDRC and PORTC registers) to switch all relays simultaneously in a single clock cycle, rather than 4 separate `digitalWrite` calls.
*   **Input/Output Flexibility:** The Analog pins on the Arduino Uno can function perfectly as Digital GPIOs.

## 14.2 Global State Variables

```cpp
int members = 0;
bool lastInState = HIGH;
bool lastOutState = HIGH;
```
**Analysis:**
*   `members`: A signed integer is used. While occupancy cannot be negative, using `int` avoids potential underflow wrap-around errors that `unsigned int` might suffer if a logic bug decrements 0 to 65535.
*   `lastInState`: This boolean flag is the cornerstone of the **Debouncing Algorithm**. By remembering the "Previous State", we can detect the exact moment (Edge) a person enters the beam.

## 14.3 The Setup Routine (System Boot)

```cpp
void setup() {
  pinMode(FLAME_PIN, INPUT_PULLUP);
  // ...
```
**Analysis:**
The `INPUT_PULLUP` mode is critical for the Flame Sensor.
*   **Floating Pins:** Without a pull-up, a digital input pin floats between 0V and 5V due to electromagnetic interference, causing false alarms.
*   **Internal Resistor:** This command engages the ATmega328P's internal 20k-50kΩ pull-up resistor, ensuring the pin stays HIGH (Safe) until the sensor actively pulls it LOW (Fire).

```cpp
  digitalWrite(RELAY1, HIGH); 
  digitalWrite(RELAY2, HIGH);
  // ...
```
**Analysis:**
**Power-On Safety State:** The relays are Active LOW devices. This means writing HIGH turns them OFF. It is mandated by safety protocols that all heavy machinery (Fans/Load) must default to the OFF state when the system reboots (e.g., after a power failure). This prevents a massive current inrush spike that could trip the main breaker.

## 14.4 The Main Loop (Real-Time OS equivalent)

The `loop()` function runs indefinitely. Due to the lack of `delay()` calls (except for debouncing), the loop frequency is approximately 100 Hz, meaning the system checks the environment 100 times per second.

### 14.4.1 Emergency Handling
```cpp
  if (digitalRead(FLAME_PIN) == LOW) {
    // ...
    while(digitalRead(FLAME_PIN) == LOW); 
    // ...
  }
```
**Analysis:**
This is a **Blocking Trap**. The `while` loop creates an inescapable software trap.
*   **Rionale:** In a fire event, "Occupancy Comfort" logic is irrelevant. The code essentially "crashes" intentionally into this safety loop to prevent any fans from fanning the flames (Oxygen supply) and keeps the alarm sounding continuously.

### 14.4.2 Counting Logic (Finite State Machine)
```cpp
  bool currentIn = digitalRead(IR_IN);
  if (currentIn == LOW && lastInState == HIGH) { 
      members++; 
      delay(250); 
  }
```
**Analysis:**
This implements a **Finite State Machine (FSM)** with two states: `IDLE` (HIGH) and `TRIGGERED` (LOW).
1.  **Condition:** The code specifically looks for the transition `HIGH -> LOW` (The Falling Edge).
2.  **Action:** Increment counter.
3.  **Hysteresis:** The `delay(250)` introduces a 250ms "Dead Time". This assumes physically that a human cannot pass through the sensor faster than 250ms. This prevents detecting a swinging arm as two separate people.

### 14.4.3 Load Logic (The Core Algorithm)
```cpp
  if (members >= 3) {
    digitalWrite(RELAY1, LOW); // 2 Fans ON
    digitalWrite(RELAY2, LOW); // 1 Fan ON
  }
```
**Analysis:**
This `if-else` ladder creates a non-linear response curve to occupancy.
*   **Threshold 3:** This magic number represents the "Heat Saturation Point." It assumes that < 3 people do not generate enough BTUs to require full cooling.
*   **Hysteresis Gap:** Ideally, a gap should be added (Turn ON at 3, OFF at 2) to prevent rapid toggling if a person stands in the doorway. The current implementation is a direct threshold for simplicity but relies on the `delay` in the counting logic to prevent rapid switching.

---
\newpage
