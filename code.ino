#include <Wire.h>
#include <WiFi.h>
#include <WiFiManager.h>
#include <Adafruit_PN532.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET    -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
Adafruit_PN532 nfc(21, 22);

String lastUID = "None";

#define RESET_BUTTON_PIN 4
#define POWER_SWITCH_PIN 5
#define CHARGING_PIN 13

bool isCharging = false;

// Function to display messages on OLED
void showOLEDMessage(String line1, String line2 = "", String line3 = "") {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);

  int y = 0;
  if (line1 != "") { display.setCursor(0, y); display.println(line1); y += 10; }
  if (line2 != "") { display.setCursor(0, y); display.println(line2); y += 10; }
  if (line3 != "") { display.setCursor(0, y); display.println(line3); }

  display.display();
}

void setup() {
  Serial.begin(115200);
  delay(1000);

  Wire.begin(21, 22);

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) while (1);  // Initialize OLED
  display.setTextColor(SSD1306_WHITE);
  showOLEDMessage("NFC Card Reader", "Booting...");

  pinMode(RESET_BUTTON_PIN, INPUT_PULLUP);
  pinMode(POWER_SWITCH_PIN, INPUT_PULLUP);
  pinMode(CHARGING_PIN, INPUT);

  if (digitalRead(RESET_BUTTON_PIN) == LOW) {
    WiFiManager wifiManager;
    wifiManager.resetSettings();  // Reset WiFi settings if button pressed
    ESP.restart();  // Restart ESP32 to apply reset
  }

  if (digitalRead(POWER_SWITCH_PIN) == LOW) {
    showOLEDMessage("Power Off", "Please turn on the switch");
    while (1);  // Stop execution if power switch is OFF
  }

  WiFiManager wifiManager;
  if (!wifiManager.autoConnect("NFC Card Reader")) {
    showOLEDMessage("WiFi Failed", "Please restart");
    delay(5000);  // Wait for 5 seconds before restarting
    ESP.restart();  // Restart ESP32 if WiFi connection fails
  }

  showOLEDMessage("WiFi Connected", "IP:", WiFi.localIP().toString());
  delay(3000);  // Wait 3 seconds after successful WiFi connection

  nfc.begin();
  uint32_t versiondata = nfc.getFirmwareVersion();
  if (!versiondata) {
    showOLEDMessage("NFC Not Found", "Check wiring");
    while (1);  // Stay here if NFC module is not found
  }

  nfc.SAMConfig();  // Configure the NFC module for use
  showOLEDMessage("NFC Reader", "Waiting for tap");
}

void loop() {
  uint8_t uid[7];
  uint8_t uidLength;

  isCharging = digitalRead(CHARGING_PIN) == HIGH;

  if (isCharging) {
    showOLEDMessage("Charging...", "Please wait");
  }

  if (nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, uid, &uidLength)) {
    lastUID = "";
    // Convert the UID to a string
    for (uint8_t i = 0; i < uidLength; i++) {
      char hex[4];
      sprintf(hex, "%02X ", uid[i]);
      lastUID += hex;
    }

    showOLEDMessage("Card Detected", "UID:", lastUID);
    delay(3000);  // Show UID for 3 seconds

    showOLEDMessage("NFC Reader", "Waiting for tap");
  }

  delay(200);  // Delay before checking again
}
//developed by Yousuf Humran
