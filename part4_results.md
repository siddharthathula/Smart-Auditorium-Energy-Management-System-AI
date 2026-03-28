
\newpage

# Chapter 8: Testing, Results & Energy Analysis

## 8.1 Unit Testing Protocol
Before full-scale deployment, individual modules were subjected to stress tests.
1.  **IR Counter Test:**
    *   *Method:* 50 volunteers walked in and out rapidly.
    *   *Result:* 49/50 counted correctly. 98% Accuracy.
    *   *Correction:* Added 250ms debounce delay to fix double-counting.
2.  **Relay Switching Latency:**
    *   *Observation:* Relays click within 100ms of threshold breach.
3.  **Flame Sensor Range:**
    *   *Result:* Detected a lighter flame at 1.5 meters.

## 8.2 Energy Saving Calculation
To quantify the "Green Impact" of this project, we compare a standard manual auditorium vs. our Smart Auditorium over a typical month (30 days, 4 events per week).

**Base Data:**
*   Auditorium Capacity: 4 Fans (75W each), 10 LED Tubes (20W each), 1 Exhaust (100W).
*   Total Load: 600 Watts (0.6 kW).
*   Usage: 4 Hours/Event x 16 Events = 64 Hours/Month.
*   Electricity Cost: $0.15 per kWh.

### Scenario A: Manual Operation (Control Group)
In manual mode, wardens often switch EVERYTHING ON 30 mins before an event and leave it ON until everyone leaves.
*   **Phantom Load:** +1 Hour per event (forgotten usage).
*   **Total Hours:** 64 + 16 = 80 Hours.
*   **Consumption:** 80 Hours * 0.6 kW = **48 kWh/Month**.

### Scenario B: Smart Operation (Test Group)
The system dynamically sheds load.
*   *Average Occupancy:* 50% (Only 2 Fans ON instead of 4).
*   *Light Sensing:* Lights OFF during day events (50% of time).
*   *Phantom Load:* 0 Hours (Auto-cutoff when Count = 0).
*   **Calculated Consumption:**
    *   (Fans: 2 * 0.075 * 64) = 9.6 kWh
    *   (Lights: 50% active * 0.2 * 64) = 6.4 kWh
    *   (Exhaust: Only when hot * 20% * 0.1 * 64) = 1.28 kWh
*   **Total:** **17.28 kWh/Month**.

### Result:
*   **Energy Saved:** 48 - 17.28 = **30.72 kWh**.
*   **Percentage Saving:** **64% Reduction**.
*   **Financial Savings:** ~$4.60 per month (per room).

---

\newpage

# Chapter 13: Conclusion & Future Scope

## 13.1 Critical Analysis
The **"Arduino-Based Smart Auditorium"** successfully meets its primary objective: minimizing human intervention in energy management. The bidirectional counter proves that simple IR physics can replace complex camera-based counting for small-scale applications. The system effectively acts as a logical gatekeeper, ensuring power is consumed *only* when human need is established quantitatively.

## 13.2 Future Scope: Technology Roadmap
1.  **LoRaWAN Integration:** Currently, the system is an "Island." By adding an SX1278 LoRa module, the auditorium could broadcast its occupancy status to a campus-wide central dashboard 5km away.
2.  **Machine Learning on Edge:** The ESP32-CAM could be upgraded to run a TinyML model (like MobileNet) to perform "Person Detection" instead of simple motion streaming, eliminating the need for IR sensors entirely.
3.  **Predictive Cooling:** Integrating with a campus Google Calendar API to "Pre-cool" the hall 10 minutes *before* a scheduled lecture begins.

## 13.3 Final Verdict
This project serves as a scalable blueprint for sustainable infrastructure. It transforms a "Dumb Building" into a "Cognitive Entity"—one that senses, thinks, and acts to preserve our most valuable resource: Energy.

---
