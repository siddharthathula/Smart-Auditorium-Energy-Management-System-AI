
\newpage

# Chapter 3: Component Datasheets & Summary

## 3.1 Overview of Hardware Selection
The hardware selection process prioritized three key metrics: **Cost-Efficiency, Reliability, and Modular Interoperability**. The core logic is handled by the ATmega328P (Arduino Uno), chosen for its robust 5V tolerance which simplifies interfacing with the 5V relay coils.

![System Block Diagram](/home/siddhartha/.gemini/antigravity/brain/a2639d23-a2e2-4a90-bc18-571ca9761109/system_block_diagram_1769675364651.png)

## 3.2 Microcontroller: Arduino Uno R3
*   **Microchip:** ATmega328P based on AVR RISC Architecture.
*   **Operating Voltage:** 5V.
*   **Input Voltage (recommended):** 7-12V via DC Jack.
*   **Digital I/O Pins:** 14 (of which 6 provide PWM output).
*   **Analog Input Pins:** 6.
*   **Flash Memory:** 32 KB.
*   **Clock Speed:** 16 MHz.
*   **Justification:** The 16MHz clock is sufficient for the bi-directional counting logic which requires polling intervals of ~200ms. The presence of hardware interrupts (INT0, INT1) allows for potential future upgrades to interrupt-based sensing.

## 3.3 Environmental Sensor: DHT11
*   **Type:** Resistive Humidity & NTC Temperature Sensor.
*   **Temperature Range:** 0°C to 50°C (Accuracy ±2°C).
*   **Humidity Range:** 20% to 90% RH (Accuracy ±5%).
*   **Sampling Rate:** 1 Hz (One reading every second).
*   **Protocol:** Single-wire proprietary digital signal.
*   **Application:** Used to trigger the Exhaust Fan relay @ 30°C threshold.

## 3.4 Obstacle Sensors: IR Proximity Module (Active Low)
*   **Components:** IR Emitter LED + Photodiode Receiver + LM393 Comparator.
*   **Detection Range:** 2cm - 30cm (Adjustable via Potentiometer).
*   **Output:** Digital LOW when obstacle detected; HIGH when clear.
*   **Logic:**
    *   *Sensor A (Entry):* Triggers "Entry" state.
    *   *Sensor B (Exit):* Triggers "Exit" state.
*   **Physics:** Based on the principle of reflection. When a person walks through the door, IR light reflects off their body back to the photodiode, dropping the output voltage to 0V.

![IR Calibration Diagram](/home/siddhartha/.gemini/antigravity/brain/a2639d23-a2e2-4a90-bc18-571ca9761109/ir_sensor_counting_setup_1769675525928.png)

## 3.5 IoT Module: ESP32-CAM
*   **Processor:** Dual-core 32-bit LX6 ESP32.
*   **Camera:** OV2640 (2 Megapixel).
*   **WiFi:** 802.11 b/g/n.
*   **RAM:** 520KB Internal + 4MB External PSRAM (Crucial for video buffering).
*   **Role:** Acts as a standalone IP Camera server. It does *not* control the relays; it purely observes.

![ESP32-CAM Wiring](/home/siddhartha/.gemini/antigravity/brain/a2639d23-a2e2-4a90-bc18-571ca9761109/esp32_cam_wiring_1769675849939.png)

---

\newpage

# Chapter 4: System Architecture & Circuit Design

## 4.1 Electrical Wiring & Relay Interface
The interface between the low-voltage logic (5V) and high-voltage loads (240V AC) is mediated by a 4-Channel Relay Module.
*   **Isolation:** The module features optocouplers (PC817) that electrically isolate the Arduino from AC noise and spikes.
*   **Active Low Logic:** The relay coils energize when the IN pin connects to GND.
    *   `digitalWrite(RELAY, LOW)` -> **ON**
    *   `digitalWrite(RELAY, HIGH)` -> **OFF**

![Relay Schematic](/home/siddhartha/.gemini/antigravity/brain/a2639d23-a2e2-4a90-bc18-571ca9761109/circuit_schematic_relay_1769675434981.png)

### 4.1.1 Pin Mapping Table
| Component | Arduino Pin | Description |
| :--- | :--- | :--- |
| **Relay 1** | A0 | Controls Main Fan Bank (2 Fans) |
| **Relay 2** | A1 | Controls Aux Fan (1 Fan) |
| **Relay 3** | A2 | Controls Lighting (All LEDs) |
| **Relay 4** | A3 | Controls Exhaust System |
| **IR Sensor 1** | D8 | Entry Detection |
| **IR Sensor 2** | A4 | Exit Detection |
| **Piezzo Buzzer** | D10 | Emergency Alarm |
| **Flame Sensor** | D9 | Fire Interrupt (Active Low) |
| **DHT11** | D13 | Env Data |

## 4.2 Power Distribution Strategy
A critical failure point in embedded systems is shared power rails. This system utilizes a **Dual-Rail Topology**.
1.  **Logic Rail (5V):** The Arduino and Sensors are powered by a regulated 5V 2A SMPS adapter.
2.  **Load Rail (240V AC):** The Fans and Lights draw power directly from the mains, switched via the Common (COM) and Normally Open (NO) relay terminals.
3.  **Isolation:** The ESP32-CAM, due to its high current spikes (300mA+ during WiFi TX), acts better if powered by a separate LDO regulator or a high-quality USB source to effectively prevent Brownout Resets.

## 4.3 Grounding Protocols
To prevent "Ghost Triggering" of relays:
*   **Common Ground:** The Relay Module GND is tied to Arduino GND.
*   **Flyback Protection:** The Relay Module includes built-in flyback diodes to clamp the back-EMF generated when the coil de-energizes.
