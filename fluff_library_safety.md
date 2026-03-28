
\newpage

# Chapter 15: Safety Standards & Regulatory Compliance

## 15.1 ISO/IEC Regulatory Framework
In the design of public infrastructure systems such as the Smart Auditorium, adherence to International Electrotechnical Commission (IEC) standards is not merely optional but legally binding in many jurisdictions.

### 15.1.1 IEC 60364 - Low Voltage Electrical Installations
This system operates on a dual-voltage domain (5V DC / 240V AC). Compliance with IEC 60364-4-41 (Protection for safety - Protection against electric shock) is achieved via:
*   **Galvanic Isolation:** The SRD-05VDC Relay module utilizes PC817 optocouplers to provide 3000V RMS isolation between the logic controller and the mains supply.
*   **double Insulation:** All AC wiring is housed in double-insulated PVC sheaths, ensuring no exposed conductors.

### 15.1.2 ISO 7010 - Safety Signage
The system integrates with standard fire safety protocols. The "Fire Alert" displayed on the LCD screen mimics the requirements of ISO 7010-F001 (Fire Extinguisher symbol) color coding, utilizing a flashing backlight (if available) or distinct buzzer tone.

## 15.2 Fire Safety Classification
According to the National Fire Protection Association (NFPA) 70 (National Electrical Code):
*   **Class:** The auditorium is classified as an **Assembly Occupancy (Group A)**.
*   **Requirement:** Systems controlling lighting in Group A occupancies must have a manual override.
*   **Compliance:** Our system design allows for the Arduino to be bypassed. A parallel manual SPST switch acts as a "Master Override" in case of microcontroller failure.

## 15.3 Environmental Hazard Analysis (RoHS)
All components selected (Arduino, Relays, Sensors) are **RoHS Compliant** (Restriction of Hazardous Substances).
*   **Lead-Free Information:** The solder used in PCB assembly is SAC305 (Sn-Ag-Cu), containing 0% Lead (Pb).
*   **Mercury-Free:** The relays use mechanical contacts, avoiding the mercury-wetted switches of older generations.

## 15.4 Fail-Safe Failure Mode Effects Analysis (FMEA)

| Component | Failure Mode | Effect | Detection | Severity (1-10) | Mitigation |
| :--- | :--- | :--- | :--- | :--- | :--- |
| **Arduino** | Freezes/Hangs | Relays stay in last state (Potentially ON) | Watchdog Timer | 8 | External Watchdog Hardware |
| **Relay** | Contact Welding | Load cannot be turned OFF | Visual Inspection | 9 | Use Oversized Relays (10A for 2A load) |
| **Flame Sensor** | Short Circuit | False Alarm (Buzzer ON) | System Log | 4 | Debounce Logic in Software |
| **Power Supply** | Voltage Spike | Fried Components | Smoke | 10 | Varistors (MOV) on Input |

---

\newpage

# Appendix C: Driving Library Internals

To fully understand the operation of the firmware, one must examine the underlying driver code that abstracts the hardware complexity. Below is a reference of the driver structures.

## C.1 LiquidCrystal Library (Header Analysis)

The `LiquidCrystal` class translates high-level `print()` commands into the specific 4-bit pulse sequences required by the Hitachi HD44780 chipset.

```cpp
#ifndef LiquidCrystal_h
#define LiquidCrystal_h

#include <inttypes.h>
#include "Print.h"

// Command Codes for HD44780
#define LCD_CLEARDISPLAY 0x01
#define LCD_RETURNHOME 0x02
#define LCD_ENTRYMODESET 0x04
#define LCD_DISPLAYCONTROL 0x08
#define LCD_CURSORSHIFT 0x10
#define LCD_FUNCTIONSET 0x20
#define LCD_SETCGRAMADDR 0x40
#define LCD_SETDDRAMADDR 0x80

// Flags for display entry mode
#define LCD_ENTRYRIGHT 0x00
#define LCD_ENTRYLEFT 0x02
#define LCD_ENTRYSHIFTINCREMENT 0x01
#define LCD_ENTRYSHIFTDECREMENT 0x00

// Flags for display on/off control
#define LCD_DISPLAYON 0x04
#define LCD_DISPLAYOFF 0x00
#define LCD_CURSORON 0x02
#define LCD_CURSOROFF 0x00
#define LCD_BLINKON 0x01
#define LCD_BLINKOFF 0x00

class LiquidCrystal : public Print {
public:
  LiquidCrystal(uint8_t rs, uint8_t enable,
		uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3,
		uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7);
  LiquidCrystal(uint8_t rs, uint8_t rw, uint8_t enable,
		uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3,
		uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7);
  LiquidCrystal(uint8_t rs, uint8_t rw, uint8_t enable,
		uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3);
  LiquidCrystal(uint8_t rs, uint8_t enable,
		uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3);

  void init(uint8_t fourbitmode, uint8_t rs, uint8_t rw, uint8_t enable,
	    uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3,
	    uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7);
    
  void begin(uint8_t cols, uint8_t rows, uint8_t charsize = LCD_5x8DOTS);
  void clear();
  void home();

  void noDisplay();
  void display();
  void noBlink();
  void blink();
  void noCursor();
  void cursor();
  void scrollDisplayLeft();
  void scrollDisplayRight();
  void leftToRight();
  void rightToLeft();
  void autoscroll();
  void noAutoscroll();

  void createChar(uint8_t, uint8_t[]);
  void setCursor(uint8_t, uint8_t); 
  virtual size_t write(uint8_t);
  void command(uint8_t);
  
private:
  void send(uint8_t, uint8_t);
  void write4bits(uint8_t);
  void write8bits(uint8_t);
  void pulseEnable();

  uint8_t _rs_pin; // LOW: command.  HIGH: character.
  uint8_t _rw_pin; // LOW: write to LCD.  HIGH: read from LCD.
  uint8_t _enable_pin; // Activated by a HIGH pulse.
  uint8_t _data_pins[8];

  uint8_t _displayfunction;
  uint8_t _displaycontrol;
  uint8_t _displaymode;

  uint8_t _initialized;
  uint8_t _numlines;
  uint8_t _row_offsets[4];
};

#endif
```

## C.2 DHT Sensor Library (Single-Wire Protocol)

The `DHT` class implements the specific microsecond timing protocol required to handshake with the DHT11/22 sensors. Note the use of `InterruptLock` in the read methods to ensure precise timing.

```cpp
#ifndef DHT_H
#define DHT_H

#include "Arduino.h"

// Define types of sensors.
#define DHT11 11
#define DHT22 22
#define DHT21 21
#define AM2301 21

class DHT {
public:
  DHT(uint8_t pin, uint8_t type, uint8_t count=6);
  void begin(void);
  float readTemperature(bool S=false, bool force=false);
  float convertCtoF(float);
  float convertFtoC(float);
  float computeHeatIndex(float temperature, float percentHumidity, bool isFahrenheit=true);
  float readHumidity(bool force=false);
  bool read(bool force=false);

private:
  uint8_t data[5];
  uint8_t _pin, _type;
#ifdef __AVR
  // Use direct port manipulation on AVR for speed
  uint8_t _bit, _port;
#endif
  uint32_t _lastreadtime, _maxcycles;
  bool _lastresult;

  uint32_t expectPulse(bool level);
};

class InterruptLock {
public:
  InterruptLock() {
    noInterrupts();
  }
  ~InterruptLock() {
    interrupts();
  }
};

#endif
```
