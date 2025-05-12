# NFC Card Reader

**NFC Card Reader** is a simple and lightweight NFC card reader system built with ESP32, PN532, and an OLED display. This project allows you to read NFC cards and display the unique UID of each card on the OLED screen, making it ideal for access control, authentication systems, or any NFC-related applications.

---

## Features

- Reads NFC cards using the PN532 module
- Displays the unique UID of the card on an OLED screen
- Supports real-time card detection and feedback
- Simple and efficient setup using ESP32
- **Portable**: Powered by a small Li-ion or Li-Po battery
- **Power Control**: On/Off switch for easy power management

---

## Hardware Requirements

| Component          | Description                        |
|-------------------|------------------------------------|
| ESP32             | Microcontroller                    |
| PN532 NFC Module  | NFC card reader module (I2C interface) |
| OLED Display      | 128x64 I2C OLED display            |
| Li-ion/Li-Po Battery | 3.7V battery for portability        |
| TP4056 Charging Module | For safe battery charging         |
| MT3608 Step-Up Converter | To boost battery voltage to 5V |
| On/Off Switch     | For power control                  |

---

## Wiring Configuration

### PN532 NFC Module (I2C)

| PN532 Pin | ESP32 Pin |
|-----------|-----------|
| VCC       | 5V        |
| GND       | GND       |
| SDA       | GPIO21    |
| SCL       | GPIO22    |

### OLED Display (I2C)

| OLED Pin | ESP32 Pin |
|----------|-----------|
| VCC      | 3.3V      |
| GND      | GND       |
| SDA      | GPIO21    |
| SCL      | GPIO22    |

### Battery and Power Setup

| Component                 | Description                        |
|---------------------------|------------------------------------|
| Li-ion/Li-Po Battery       | Provides 3.7V power               |
| TP4056 Charging Module    | Manages charging of the battery   |
| MT3608 Step-Up Converter  | Boosts voltage to 5V for ESP32 and PN532 |
| On/Off Switch             | Controls power to the system      |

---

## System Workflow

1. On power-up, the OLED displays "Booting..."
2. Once connected to WiFi, it shows the IP address.
3. Upon scanning an NFC card:
   - The UID of the card is shown on the OLED display.
4. If no card is detected, the display continues showing "Waiting for tap."

---

## Installation

1. Clone this repository or download the project files.
2. Open the `.ino` file in the Arduino IDE.
3. Install the necessary libraries:
   - **Adafruit PN532**
   - **Adafruit SSD1306**
   - **Adafruit GFX**
   - **WiFiManager**
   - **WebServer**
4. Select your ESP32 board and upload the code.

---

## Library Dependencies

1. **Adafruit PN532**  
   - Library for interfacing with the PN532 NFC module.

2. **Adafruit SSD1306**  
   - Library for controlling the OLED display.

3. **Adafruit GFX**  
   - Provides graphics primitives for drawing on displays.

4. **WiFiManager**  
   - Library to simplify WiFi connection management.

---

## WiFi Configuration

The device will use **WiFiManager** to connect to a WiFi network. On boot, if the device is unable to connect, it will start an access point to configure the WiFi settings via a web interface.

---

# TapLink NFC Reader OLED Output

## 1. Startup
- **Message**: "TapLink" and "Booting..."
- **Display**:
  ```
  TapLink
  Booting...
  ```

## 2. WiFi Connection

### Successful WiFi Connection
- **Message**: "WiFi Connected", "IP: <IP_Address>"
- **Display**:
  ```
  WiFi Connected
  IP: <IP_Address>
  ```

### Failed WiFi Connection
- **Message**: "WiFi Failed", "Please restart"
- **Display**:
  ```
  WiFi Failed
  Please restart
  ```

## 3. NFC Card Detection
- **Message**: "Card Detected", "UID: <UID>"
- **Display**:
  ```
  Card Detected
  UID: <UID>
  ```

## 4. Idle State
- **Message**: "NFC Reader", "Waiting for tap"
- **Display**:
  ```
  NFC Reader
  Waiting for tap
  ```

## Developed by  
**Yousuf Humran**

