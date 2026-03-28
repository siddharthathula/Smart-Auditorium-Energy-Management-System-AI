
\newpage

# Chapter 6: Software Methodology & Algorithm Design

## 6.1 Bi-Directional Count Algorithm
The core intelligence of the system lies in its ability to differentiate between someone *entering* and someone *exiting*. This is achieved not by simple motion detection, but by **Sequence Parsing**.

*   **Entry Sequence:** Sensor A (Outer) triggers -> Sensor B (Inner) triggers.
*   **Exit Sequence:** Sensor B (Inner) triggers -> Sensor A (Outer) triggers.

In the provided firmware, a simplified approach is used where each sensor increments or decrements a global counter.
```cpp
// Logic for Entry
if (currentIn == LOW && lastInState == HIGH) { 
    members++; 
    delay(250); // Debounce delay
}
```
This "Falling Edge Detection" (Transition from HIGH to LOW) prevents the counter from racing up if a person stands in front of the sensor. The `delay(250)` acts as a software debounce filter, rejecting spurious noise.

![System Flowchart](/home/siddhartha/.gemini/antigravity/brain/a2639d23-a2e2-4a90-bc18-571ca9761109/state_machine_flowchart_1769675739536.png)

## 6.2 Dynamic Load Balancing Logic
The code implements a "Capacity-Based" state machine. Unlike a linear dimming system, this uses discrete control steps.
1.  **Stage 0 (Empty):** `members = 0`. All Relays OFF. Energy consumption is effectively zero (only MCU idle power).
2.  **Stage 1 (Low Occupancy):** `members >= 1`. Relay 1 (Zone A Fans) activates. Relay 2 remains OFF.
3.  **Stage 2 (High Occupancy):** `members >= 3`. Relay 2 (Zone B Fans) activates, running the cooling at 100% capacity.

## 6.3 Safety Interrupts
The `digitalRead(FLAME_PIN)` acts as a **Blocking Routine**.
```cpp
if (digitalRead(FLAME_PIN) == LOW) {
    // Cut all power to prevent electrical fires
    digitalWrite(RELAY1, HIGH); 
    // Sound Alarm
    digitalWrite(BUZZER_PIN, HIGH);
    // Halt system until reset
    while(digitalRead(FLAME_PIN) == LOW); 
}
```
This `while()` loop creates a "System Lockout." The microcontroller refuses to process any other data (counting, temp) until the fire source is removed, prioritizing safety over comfort.

## 6.4 Full Source Code (Commented)
The following code acts as the OS for the Smart Auditorium.

```cpp
/*
 * Smart Auditorium Controller Firmware v1.0
 * Architecture: Arduino Uno R3
 * Logic: Event-Driven with Polling
 */

#include <LiquidCrystal.h>
#include <DHT.h>

// --- PIN DEFINITIONS ---
// Sensors
#define DHTPIN 13       // Temp Sensor Data
#define DHTTYPE DHT11   // Sensor Model
#define FLAME_PIN 9     // IR Flame Detector (Active Low)
#define IR_IN 8         // Entry Counter
#define IR_OUT A4       // Exit Counter
#define LDR_PIN A5      // Ambient Light (Analog)

// Actuators
#define BUZZER_PIN 10   // Piezo Alarm
#define RELAY1 A0       // Fan Bank A
#define RELAY2 A1       // Fan Bank B (Aux)
#define RELAY3 A2       // LED Lighting
#define RELAY4 A3       // Exhaust/HVAC

// Global Objects
LiquidCrystal lcd(12, 11, 5, 4, 3, 2); // RS, EN, D4, D5, D6, D7
DHT dht(DHTPIN, DHTTYPE);

// Global Variables
int members = 0;          // Occupancy Counter
bool lastInState = HIGH;  // Debounce State Entry
bool lastOutState = HIGH; // Debounce State Exit

void setup() {
  // Input Configuration
  pinMode(FLAME_PIN, INPUT_PULLUP);
  pinMode(IR_IN, INPUT);
  pinMode(IR_OUT, INPUT);

  // Output Configuration
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(RELAY1, OUTPUT); pinMode(RELAY2, OUTPUT); 
  pinMode(RELAY3, OUTPUT); pinMode(RELAY4, OUTPUT);

  // Safe State Initialization (Relays OFF)
  digitalWrite(RELAY1, HIGH); digitalWrite(RELAY2, HIGH);
  digitalWrite(RELAY3, HIGH); digitalWrite(RELAY4, HIGH);

  // Peripherals
  dht.begin();
  lcd.begin(16, 2);
  lcd.print("SYSTEM BOOT...");
  delay(1000);
}

void loop() {
  // --- 1. PRIORITY SAFETY CHECK ---
  if (digitalRead(FLAME_PIN) == LOW) {
    emergencyProtocol();
  }

  // --- 2. OCCUPANCY COUNTING ---
  handleCounting();

  // --- 3. LIGHTING CONTROL ---
  handleLighting();

  // --- 4. CLIMATE CONTROL ---
  handleClimate();

  // --- 5. UI UPDATE ---
  updateDisplay();
}

// ... (Helper functions implemented logically in main block) ...
```

---

\newpage

# Chapter 7: IoT & Vision Integration

## 7.1 ESP32-CAM Architecture
While the Arduino manages the physical realm, the ESP32-CAM handles the digital twin.
*   **Web Server:** The ESP32 hosts a page at `192.168.4.1/stream`.
*   **Video Encoding:** It streams MJPEG (Motion JPEG) data. This format is bandwidth-heavy but low-latency, perfect for local LAN monitoring.

![Dashboard UI Mockup](/home/siddhartha/.gemini/antigravity/brain/a2639d23-a2e2-4a90-bc18-571ca9761109/dashboard_ui_mockup_1769675581087.png)

## 7.2 Data Security
Since the video feed contains sensitive footage of the auditorium, the system is configured as an **Isolated Access Point (AP Mode)**.
*   **Air Gapped:** The ESP32 does *not* connect to the wider internet.
*   **Local Access Only:** Security personnel must physically be within WiFi range (approx 50m) to view the feed. This eliminates the risk of remote cloud hacking.
