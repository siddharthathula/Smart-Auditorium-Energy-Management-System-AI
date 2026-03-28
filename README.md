# 🚀 AI-Powered Smart Auditorium: Energy & Occupancy Manager

**Transform any auditorium into a self-managing, power-saving ecosystem.**

![Project Status](https://img.shields.io/badge/Status-Stable-success)
![Hardware](https://img.shields.io/badge/Hardware-Arduino%20Uno-blue)
![IoT](https://img.shields.io/badge/IoT-ESP32--CAM-orange)

The **Smart Auditorium System** is a robust, low-cost solution designed to eliminate "phantom energy usage" in large public spaces. By using intelligent occupancy counting and environmental sensing, the system dynamically scales its power consumption to match real-time human needs.

## ✨ High-Click Features
- **👥 Intelligent Bi-Directional Counting**: Tracks the exact number of people in the room to scale cooling (Fans) from 0% to 100%.
- **☀️ Smart Lighting (LDR)**: Automatic day/night detection ensures lights are only ON when the ambient light is insufficient.
- **🔥 Fire/Flame Security**: Integrated hardware interrupt that immediately cuts all power and sounds a high-decibel alarm during fire emergencies.
- **🌡️ Climate Control (DHT11)**: Automated exhaust fan management based on temperature and humidity thresholds.
- **📡 IoT Video Streaming**: ESP32-CAM integration for remote monitoring and visual verification of events.

## 🛠️ Hardware Stack
- **Arduino Uno**: The brain of the automation logic.
- **ESP32-CAM**: Handles IoT video telemetry.
- **4-Channel Relay Module**: Safely manages 240V AC loads.
- **In-Out IR Sensors**: Precision counting logic.
- **LCD 16x2 Display**: Real-time dashboard for headcount and environment stats.

## 📂 Project Structure
- `smart_auditorium.ino`: Optimized C++ firmware for Arduino.
- `reports/`: Comprehensive technical documentation and energy saving analysis.
- `images/`: High-resolution circuit diagrams and assembly photos.

## 🚀 Quick Start
1. **Flash Firmware**: Upload `smart_auditorium.ino` to your Arduino Uno.
2. **Wiring**: Connect sensors as per the mapping in the source code header.
3. **Power Up**: Provide stable 5V for logic and connect AC loads to relays.
4. **Monitor**: Watch the LCD update in real-time as people enter/exit.

## 📄 License
MIT License - Open for educational and commercial use.
