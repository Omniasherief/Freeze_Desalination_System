// basic_connect.ino
#include <WiFi.h>

const char* WIFI_SSID = "BORM";      
const char* WIFI_PASS = "Mahmoud@No24102013";   

unsigned long lastPrint = 0;
unsigned long lastBegin = 0;
const unsigned long retryInterval = 10000; 

void setup() {
  Serial.begin(115200);
  delay(200);
  Serial.println();
  Serial.println("=== Basic WiFi Connect Test ===");
  Serial.print("Module MAC: ");
  Serial.println(WiFi.macAddress());
  /
  Serial.print("Starting WiFi: ");
  Serial.println(WIFI_SSID);
  WiFi.disconnect();
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  lastBegin = millis();
}

void loop() {
  // Print status every 2s
  if (millis() - lastPrint > 2000) {
    lastPrint = millis();
    Serial.print("WiFi.status() = ");
    Serial.print(WiFi.status());
    Serial.print("  (0=IDLE,1=NO_SSID,3=CONNECTED,4=AUTH_FAIL,5=LOST,6=DISCONNECTED)");
    Serial.println();
    if (WiFi.status() == WL_CONNECTED) {
      Serial.print("Connected! IP: ");
      Serial.println(WiFi.localIP());
    }
  }

  // Retry begin only every retryInterval (non-blocking)
  if (WiFi.status() != WL_CONNECTED && millis() - lastBegin >= retryInterval) {
    lastBegin = millis();
    Serial.println("Retrying WiFi.begin() ...");
    WiFi.disconnect();
    delay(50);
    WiFi.begin(WIFI_SSID, WIFI_PASS);
  }
}
