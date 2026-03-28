
\newpage

# Chapter 12: Installation & Deployment Manual

## 12.1 Pre-Installation Site Survey
Before deploying the Smart Auditorium system, a site survey is mandatory.
1.  **Door Frame Analysis:** Measure the width of the main entrance. If > 1.5 meters, the IR sensors must be replaced with Laser (ToF) modules or multiple IR pairs.
2.  **Lux Mapping:** Use a Lux-meter to determine natural light ingress during 12:00 PM vs 6:00 PM to calibrate the LDR threshold.
3.  **WiFi Survey:** Ensure the ESP32-CAM location has at least -70dBm signal strength for smooth video streaming.

## 12.2 Step-by-Step Wiring Guide

### Step 1: The "Brains" (Control Box)
1.  Mount the Arduino Uno and 4-Channel Relay on a DIN Rail inside a generic ABS Enclosure (IP65 rated).
2.  Connect the 5V 2A Power Supply Unit (PSU) to the centralized power strip.
3.  **Color Coding Standard:**
    *   **RED:** +5V DC
    *   **BLACK:** GND
    *   **YELLOW:** Control Signals (Sensors to Arduino)
    *   **BLUE:** Neutral (AC 240V)
    *   **BROWN:** Live (AC 240V)

### Step 2: Sensor Mounting
1.  **IR Counters:** Mount Sensor A (Entry) and Sensor B (Exit) at a height of **90cm - 110cm** (Waist height). This avoids false triggers from swinging arms or legs.
    *   *Alignment:* Ensure the IR TX and RX are perfectly perpendicular to the floor. Use a spirit level.
2.  **LDR:** Mount on the ceiling facing a window, but *shielded* from the artificial lights it controls to prevent "Optical Feedback Loops" (Flickering).
3.  **Flame Sensor:** Mount near the ceiling smoke catchment area or near potential fire hazards (e.g., Podium/Projector).

### Step 3: Load Interfacing
1.  **Safety First:** Turn off the Main Breaker.
2.  Cut the Live wire of the Fan/Light circuit.
3.  Connect the "Supply Side" wire to the **COM** (Common) terminal of the Relay.
4.  Connect the "Load Side" wire to the **NO** (Normally Open) terminal.
5.  *Verify:* Ensure no strands of copper are exposed. Use heat-shrink tubing.

---

\newpage

# Chapter 13: Maintenance & Troubleshooting Guide

## 13.1 Routine Maintenance Schedule

| Frequency | Component | Action |
| :--- | :--- | :--- |
| **Weekly** | IR Sensors | Wipe dust off the lenses using a microfiber cloth. Dust accumulation reduces counting range. |
| **Monthly** | Relays | Listen for "Buzzing" sounds (indicates coil fatigue). Check terminal screws for tightness (thermal expansion loosens them). |
| **Quarterly** | ESP32-CAM | Reboot the module to clear memory leaks. Check WiFi logic logs. |
| **Yearly** | Wiring | Inspect for insulation brittleness or rodent damage in false ceilings. |

## 13.2 Troubleshooting Matrix

### Issue 1: "The Fans Keep Switching ON and OFF rapidly."
*   **Diagnosis:** This is called "Hunting." It happens when the count fluctuates (e.g., someone standing in the doorway).
*   **Fix:** Adjust the Debounce Delay in the code (`delay(250)` -> `delay(500)`). Or move sensors further apart (Hardware Hysteresis).

### Issue 2: "Lights turn ON during the day."
*   **Diagnosis:** LDR Threshold is too high.
*   **Fix:**
    1.  Open Serial Monitor.
    2.  Check `analogRead(LDR_PIN)` value at noon.
    3.  Update the `if (val > 500)` line in code to match the new calibration.

### Issue 3: "Video Feed is lagging."
*   **Diagnosis:** ESP32-CAM overheating or weak WiFi.
*   **Fix:** Add a purely passive heatsink to the ESP32 chip. Move the router closer.

### Issue 4: "Fire Alarm False Trigger."
*   **Diagnosis:** Flame sensor reacting to Sunlight (UV).
*   **Fix:** Rotate the sensitivity potentiometer on the module counter-clockwise. Shield the sensor from direct windows.

---

\newpage

# Chapter 14: Bill of Materials (BOM) & Costing

## 14.1 Component Breakdown

| Item | Qty | Unit Cost ($) | Total ($) | Source |
| :--- | :--- | :--- | :--- | :--- |
| **Arduino Uno R3** | 1 | 20.00 | 20.00 | DigiKey |
| **ESP32-CAM** | 1 | 8.00 | 8.00 | Mouser |
| **4-Channel Relay** | 1 | 5.00 | 5.00 | Amazon |
| **IR Obstacle Sensor** | 2 | 1.50 | 3.00 | Local |
| **DHT11 Sensor** | 1 | 2.00 | 2.00 | Adafruit |
| **LCD 16x2 Display** | 1 | 3.00 | 3.00 | Local |
| **Flame Sensor** | 1 | 1.00 | 1.00 | Local |
| **Misc (Wires, Box)** | 1 | 10.00 | 10.00 | Hardware Store |
| **TOTAL** | | | **$52.00** | |

## 14.2 Commercial Viability
A standard Schneider/Siemens Building Automation endpoint costs approx $500 per room.
*   **Our Cost:** $52.
*   **Margin:** Even with a 200% markup (selling at $150), this product undercuts the market significantly.
*   **Target Market:** Budget schools, old age homes, and municipal halls in developing economies.
