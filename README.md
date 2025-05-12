# NFC Card Reader

**NFC Card Reader** is a simple and lightweight NFC card reader system built with ESP32, PN532, and an OLED display. This project allows you to read NFC cards and display the unique UID of each card on the OLED screen, making it ideal for access control, authentication systems, or any NFC-related applications.

---

## Features

- Reads NFC cards using the PN532 module
- Displays the unique UID of the card on an OLED screen
- Supports real-time card detection and feedback
- Simple and efficient setup using ESP32
- **Push Button Reset**: Allows resetting the WiFi connection

---

## Hardware Requirements

| Component          | Description                        |
|-------------------|------------------------------------|
| ESP32             | Microcontroller                    |
| PN532 NFC Module  | NFC card reader module (I2C interface) |
| OLED Display      | 128x64 I2C OLED display            |
| Push Button       | For resetting WiFi connection      |

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

### Push Button Reset

| Push Button Pin | ESP32 Pin |
|-----------------|-----------|
| One side of button | GND    |
| Other side of button | GPIO4  |

---

## System Workflow

1. On power-up, the OLED displays "Booting..."
2. Once connected to WiFi, it shows the IP address.
3. Upon scanning an NFC card:
   - The UID of the card is shown on the OLED display.
4. If no card is detected, the display continues showing "Waiting for tap."
5. **Push Button Reset**: Pressing the button resets the WiFi configuration and initiates the WiFiManager to reconnect to a WiFi network.

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

