#include <Wire.h>
#include <WiFi.h>
#include <WiFiManager.h>
#include <Adafruit_PN532.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <WebServer.h>
#include <vector>

// OLED display setup
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET    -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// NFC module setup (I2C)
Adafruit_PN532 nfc(21, 22);

// WiFi reset button
#define RESET_BUTTON_PIN 4

// Web server setup
WebServer server(80);

// UID history
struct UIDEntry {
  String uid;
  String timestamp;
};
std::vector<UIDEntry> uidHistory;

// Last UID
String lastUID = "None";

// Show message on OLED
void showOLEDMessage(String line1, String line2 = "", String line3 = "") {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);

  int lineHeight = 10;
  int y = 0;

  if (line1 != "") { display.setCursor(0, y); display.println(line1); y += lineHeight; }
  if (line2 != "") { display.setCursor(0, y); display.println(line2); y += lineHeight; }
  if (line3 != "") { display.setCursor(0, y); display.println(line3); }

  display.display();
}

// Create web HTML content
String generateHTML() {
  String html = "<!DOCTYPE html><html><head><title>NFC Card Reader</title><meta name='viewport' content='width=device-width, initial-scale=1'>";
  html += "<style>body{font-family:Arial;}table,th,td{border:1px solid black;border-collapse:collapse;padding:5px;}</style>";
  html += "</head><body>";
  html += "<h2>NFC Card Reader</h2>";
  html += "<p><b>Device IP:</b> " + WiFi.localIP().toString() + "</p>";
  html += "<p><b>Signal Strength (RSSI):</b> " + String(WiFi.RSSI()) + " dBm</p>";
  html += "<p><b>System Status:</b> Working Fine ✅</p>";
  html += "<h3>Scan History</h3><table><tr><th>UID</th><th>Timestamp</th></tr>";

  for (auto entry : uidHistory) {
    html += "<tr><td>" + entry.uid + "</td><td>" + entry.timestamp + "</td></tr>";
  }

  html += "</table><br>";
  html += "<form action='/clear' method='POST'><button type='submit'>Clear History</button></form>";
  html += "</body></html>";
  return html;
}

// Setup function
void setup() {
  Serial.begin(115200);
  Wire.begin(21, 22);

  // OLED init
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("OLED failed");
    while (1);
  }

  showOLEDMessage("NFC Reader", "Booting...");

  // WiFi Reset Button
  pinMode(RESET_BUTTON_PIN, INPUT_PULLUP);
  if (digitalRead(RESET_BUTTON_PIN) == LOW) {
    showOLEDMessage("WiFi Reset", "Clearing settings...");
    WiFiManager wifiManager;
    wifiManager.resetSettings();
    delay(2000);
    ESP.restart();
  }

  // Connect WiFi
  WiFiManager wifiManager;
  if (!wifiManager.autoConnect("NFC Card Reader")) {
    showOLEDMessage("WiFi Failed", "Please restart");
    delay(5000);
    ESP.restart();
  }

  Serial.print("IP: ");
  Serial.println(WiFi.localIP());
  showOLEDMessage("WiFi Connected", "IP:", WiFi.localIP().toString());
  delay(5000);  // Show IP on OLED for 5 sec

  // Init NFC
  nfc.begin();
  uint32_t versiondata = nfc.getFirmwareVersion();
  if (!versiondata) {
    showOLEDMessage("NFC Not Found", "Check wiring");
    while (1);
  }
  nfc.SAMConfig();
  showOLEDMessage("NFC Reader", "Waiting for tap");

  // Setup WebServer
  server.on("/", []() {
    server.send(200, "text/html", generateHTML());
  });

  server.on("/clear", HTTP_POST, []() {
    uidHistory.clear();
    server.send(200, "text/html", "<p>History cleared. <a href='/'>Go Back</a></p>");
  });

  server.begin();
  Serial.println("Web server started");
}

// Get timestamp (basic approximation)
String getTimestamp() {
  time_t now = millis() / 1000;
  int mins = now / 60;
  int secs = now % 60;
  return String(mins) + "m " + String(secs) + "s";
}

// Main loop
void loop() {
  server.handleClient();

  uint8_t uid[7];
  uint8_t uidLength;

  if (nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, uid, &uidLength)) {
    Serial.println("NFC Card Detected");

    lastUID = "";
    for (uint8_t i = 0; i < uidLength; i++) {
      char hex[4];
      sprintf(hex, "%02X ", uid[i]);
      lastUID += hex;
    }

    String timestamp = getTimestamp();
    uidHistory.push_back({lastUID, timestamp});

    showOLEDMessage("Card Detected", "UID:", lastUID);
    delay(3000);
    showOLEDMessage("NFC Reader", "Waiting for tap");
  }

  delay(200);
}
//developed by Yousuf Humran
