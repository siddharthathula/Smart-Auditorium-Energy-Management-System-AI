
\newpage

# Appendix A: Component Datasheets

## A.1 ATmega328P (Arduino Uno Microcontroller)

**General Description**
The ATmega328P is a low-power CMOS 8-bit microcontroller based on the AVR enhanced RISC architecture. By executing powerful instructions in a single clock cycle, the ATmega328P achieves throughputs approaching 1 MIPS per MHz allowing the system designer to optimize power consumption versus processing speed.

**Features**
*   **High Performance, Low Power AVR® 8-Bit Microcontroller**
*   **Advanced RISC Architecture**
    *   131 Powerful Instructions – Most Single Clock Cycle Execution
    *   32 x 8 General Purpose Working Registers
    *   Fully Static Operation
    *   Up to 20 MIPS Throughput at 20 MHz
    *   On-chip 2-cycle Multiplier
*   **High Endurance Non-volatile Memory Segments**
    *   32K Bytes of In-System Self-Programmable Flash program memory
    *   1K Bytes EEPROM
    *   2K Bytes Internal SRAM
    *   Write/Erase Cycles: 10,000 Flash/100,000 EEPROM
    *   Data retention: 20 years at 85°C / 100 years at 25°C
*   **Peripheral Features**
    *   Two 8-bit Timer/Counters with Separate Prescaler and Compare Mode
    *   One 16-bit Timer/Counter with Separate Prescaler, Compare Mode, and Capture Mode
    *   Real Time Counter with Separate Oscillator
    *   Six PWM Channels
    *   8-channel 10-bit ADC in TQFP and QFN/MLF package
    *   Temperature Measurement
    *   Programmable Serial USART
    *   Master/Slave SPI Serial Interface
    *   Byte-oriented 2-wire Serial Interface (Philips I2C compatible)
    *   Programmable Watchdog Timer with Separate On-chip Oscillator
    *   On-chip Analog Comparator
    *   Interrupt and Wake-up on Pin Change
*   **Special Microcontroller Features**
    *   Power-on Reset and Programmable Brown-out Detection
    *   Internal Calibrated Oscillator
    *   External and Internal Interrupt Sources
    *   Six Sleep Modes: Idle, ADC Noise Reduction, Power-save, Power-down, Standby, and Extended Standby
*   **I/O and Packages**
    *   23 Programmable I/O Lines
    *   28-pin PDIP, 32-lead TQFP, 28-pad QFN/MLF and 32-pad QFN/MLF
*   **Operating Voltage:**
    *   1.8 - 5.5V
*   **Temperature Range:**
    *   -40°C to 85°C

---

## A.2 Songle SRD-05VDC-SL-C Relay

**Description**
The SRD-05VDC-SL-C is a sealed subminiature electromagnetic relay. It is widely used in domestic appliances, office machines, audio equipment, automobiles, etc.

**Coil Ratings (at 20°C)**
*   **Rated Voltage:** 5V DC
*   **Coil Resistance:** $70 \Omega \pm 10\%$
*   **Rated Current:** 71.4 mA
*   **Power Consumption:** 0.36 W
*   **Pull-in Voltage:** 75% Max. of Rated Voltage
*   **Drop-out Voltage:** 10% Min. of Rated Voltage
*   **Max-Allowable Voltage:** 130% of Rated Voltage

**Contact Ratings**
*   **Contact Material:** Silver Alloy (AgSnO2)
*   **Contact Form:** SPDT (1 Form C)
*   **Contact Rating (Resistive Load):**
    *   10A 250VAC
    *   10A 30VDC
    *   10A 125VAC
    *   10A 28VDC
*   **Max Switching Voltage:** 250VAC / 30VDC
*   **Max Switching Current:** 10A
*   **Contact Resistance:** $100 m\Omega$ Max. (at 6VDC 1A)

---

## A.3 DHT11 Humidity & Temperature Sensor

**Overview**
DHT11 Temperature & Humidity Sensor features a temperature & humidity sensor complex with a calibrated digital signal output. By using the exclusive digital-signal-acquisition technique and temperature & humidity sensing technology, it ensures high reliability and excellent long-term stability. This sensor includes a resistive-type humidity measurement component and an NTC temperature measurement component, and connects to a high-performance 8-bit microcontroller, offering excellent quality, fast response, anti-interference ability and cost-effectiveness.

**Technical Specifications**
*   **Power Supply:** 3.5 to 5.5V DC
*   **Current Consumption:** measurement roughly 0.3mA, standby 60uA
*   **Temperature Range:** 0°C to 50°C
    *   *Accuracy:* ± 2°C
    *   *Resolution:* 1°C
*   **Humidity Range:** 20% to 90% RH
    *   *Accuracy:* ± 4% RH (at 25°C)
    *   *Resolution:* 1% RH
*   **Signal Output:** Digital (Single-bus)
*   **Sampling Period:** > 2 Seconds (Data is typically 2 seconds old)

**Pinout**
1.  **VCC:** 3.5V-5.5V DC
2.  **DATA:** Serial Data, single-bus
3.  **NC:** Not Connected
4.  **GND:** Ground, connects to negative power

## A.4 LM393 Comparator (Used in I.R. Sensor)

The LM393 series are dual independent precision voltage comparators capable of single or split supply operation. These devices are designed to permit a common mode range-to-ground level with single supply operation. Input offset voltage specifications as low as 2.0 mV make this device an excellent selection for many applications in consumer, automotive, and industrial electronics.

*   **Wide Supply Voltage Range:**
    *   Single Supply: 2.0 V to 36 V
    *   Split Supply: ±1.0 V to ±18 V
*   **Low Supply Current Drain:** 0.4 mA Independent of Supply Voltage
*   **Low Input Bias Current:** 25 nA
*   **Low Input Offset Current:** 5 nA
*   **Differential Input Voltage Range Equal to Maximum-Rated Supply Voltage:** ±36 V
*   **Low Output Saturation Voltage:** 250 mV at 4 mA
*   **Output Voltage Compatible with TTL, MOS and CMOS Logic Levels**

## A.5 ESP32-CAM (AI-Thinker)

**Module Specs**
*   **SoC:** ESP32-S (Xtensa® 32-bit LX6)
*   **Flash:** 4 MB (External)
*   **PSRAM:** 4 MB (External) - *Critical for Image Buffering*
*   **WiFi:** 802.11 b/g/n (Up to 150Mbps)
*   **Bluetooth:** 4.2 BR/EDR and BLE
*   **Camera:** OV2640 (Connector FPC 24Pin, 0.5mm Pitch)
    *   *Resolution:* UXGA (1600 x 1200)
    *   *Output:* JPEG, YUV, RGB
*   **IO Ports:** 9 GPIOs
*   **UART Baudrate:** Default 115200 bps
*   **Power Supply:** 5V 2A Recommended
*   **Power Consumption:**
    *   Flash Off: 180mA @ 5V
    *   Flash On (Max Brightness): 310mA @ 5V
    *   Deep-Sleep: 6mA @ 5V
    *   Modem-Sleep: 20mA @ 5V
    *   Light-Sleep: 6.7mA @ 5V

---

**[END OF APPENDICES]**
