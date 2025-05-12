#include <Wire.h>
#include <WiFi.h>
#include <WiFiManager.h>
#include <Adafruit_PN532.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// OLED display configuration
#define SCREEN_WIDTH 128         // OLED width in pixels
#define SCREEN_HEIGHT 64         // OLED height in pixels
#define OLED_RESET    -1         // No reset pin used for OLED
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// PN532 NFC module using I2C (SDA = GPIO21, SCL = GPIO22)
Adafruit_PN532 nfc(21, 22);

// Store the last scanned UID
String lastUID = "None";

// Define reset button pin for WiFi credentials
#define RESET_BUTTON_PIN 4       // Use GPIO 4 for reset button

// Function to display messages on the OLED screen
void showOLEDMessage(String line1, String line2 = "", String line3 = "") {
  display.clearDisplay();
  display.setTextSize(1);  // Set text size
  display.setTextColor(SSD1306_WHITE);

  int lineHeight = 10;
  int y = 0;

  // Display each line if not empty
  if (line1 != "") {
    display.setCursor(0, y);
    display.println(line1);
    y += lineHeight;
  }
  if (line2 != "") {
    display.setCursor(0, y);
    display.println(line2);
    y += lineHeight;
  }
  if (line3 != "") {
    display.setCursor(0, y);
    display.println(line3);
  }

  display.display();
}

void setup() {
  Serial.begin(115200);        // Start serial communication
  delay(1000);                 // Small delay for startup

  Wire.begin(21, 22);          // Initialize I2C communication

  // Initialize OLED display
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("SSD1306 initialization failed");
    while (1); // Halt if OLED fails
  }

  display.setTextColor(SSD1306_WHITE);
  showOLEDMessage("NFC Card Reader", "Booting...");

  // Configure reset button
  pinMode(RESET_BUTTON_PIN, INPUT_PULLUP);

  // Reset WiFi credentials if reset button is pressed
  if (digitalRead(RESET_BUTTON_PIN) == LOW) {
    Serial.println("Reset button pressed. Clearing WiFi settings...");
    showOLEDMessage("WiFi Reset", "Clearing settings...");
    delay(2000);

    WiFiManager wifiManager;
    wifiManager.resetSettings();
    ESP.restart();  // Restart to apply changes
  }

  // Connect to WiFi using WiFiManager
  WiFiManager wifiManager;
  if (!wifiManager.autoConnect("NFC Card Reader")) {  // WiFi will be named "NFC Card Reader"
    Serial.println("WiFi connection failed");
    showOLEDMessage("WiFi Failed", "Please restart");
    delay(5000);
    ESP.restart();  // Restart on failure
  }

  // Display connection info
  Serial.println("WiFi Connected");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  showOLEDMessage("WiFi Connected", "IP:", WiFi.localIP().toString());
  delay(3000);

  // Initialize NFC module
  nfc.begin();
  uint32_t versiondata = nfc.getFirmwareVersion();
  if (!versiondata) {
    Serial.println("PN532 not detected");
    showOLEDMessage("NFC Not Found", "Check wiring");
    while (1); // Halt if NFC not found
  }

  nfc.SAMConfig();  // Configure NFC for card reading
  Serial.println("NFC Ready");
  showOLEDMessage("NFC Reader", "Waiting for tap");
}

void loop() {
  uint8_t uid[7];          // Buffer to store UID
  uint8_t uidLength;       // Length of UID

  // Check for NFC card
  if (nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, uid, &uidLength)) {
    Serial.println("\nNFC Card Detected");
    Serial.print("UID: ");

    lastUID = "";
    for (uint8_t i = 0; i < uidLength; i++) {
      char hex[4];
      sprintf(hex, "%02X ", uid[i]);  // Convert UID to hex string
      Serial.print(hex);
      lastUID += hex;
    }
    Serial.println();

    // Display UID on OLED
    if (lastUID != "") {
      showOLEDMessage("Card Detected", "UID:", lastUID);
    } else {
      showOLEDMessage("Error", "Invalid UID");
    }
    delay(3000);

    // Return to idle state
    showOLEDMessage("NFC Reader", "Waiting for tap");
  }

  delay(200);  // Small delay between reads
}

/*
 * Developed by Yousuf Humran
 * NFC Card Reader Project
 */
