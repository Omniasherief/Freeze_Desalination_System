// scan_and_connect.ino
#include <WiFi.h>

const char* WIFI_SSID = "BORM";      
const char* WIFI_PASS = "Mahmoud@No24102013";   

bool wifiConnected = false;
unsigned long lastScan = 0;
const unsigned long scanInterval = 15000; // scan
unsigned long lastBegin = 0;
const unsigned long retryInterval = 10000; // reconnect  

// Static IP test (false = use DHCP)
const bool useStaticIP = false;
IPAddress localIP(192,168,43,50);
IPAddress gateway(192,168,43,1);
IPAddress subnet(255,255,255,0);
IPAddress dns1(8,8,8,8);

void setup() {
  Serial.begin(115200);
  delay(200);
  Serial.println("\n=== Scan & Connect Test ===");
  Serial.print("Module MAC: ");
  Serial.println(WiFi.macAddress());

  // start scanning immediately
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);
  Serial.println("Starting scan...");
  int n = WiFi.scanNetworks(true, true); // async scan
  lastScan = millis();
}

void loop() {
  // Check async scan finished
  int n = WiFi.scanComplete();
  if (n >= 0) {
    Serial.printf("Scan complete: %d networks\n", n);
    for (int i = 0; i < n; ++i) {
      Serial.printf("%2d: SSID:'%s'  BSSID:%s  CH:%d  RSSI:%d\n",
                    i+1,
                    WiFi.SSID(i).c_str(),
                    WiFi.BSSIDstr(i).c_str(),
                    WiFi.channel(i),
                    WiFi.RSSI(i));
    }
    WiFi.scanDelete();
    // schedule next scan after interval
    lastScan = millis();
    // Try connect now (non-blocking)
    Serial.println("Attempting WiFi.begin() ...");
    if (useStaticIP) {
      Serial.println("Using static IP configuration...");
      WiFi.config(localIP, gateway, subnet, dns1);
    }
    WiFi.begin(WIFI_SSID, WIFI_PASS);
    lastBegin = millis();
  }

  // Periodic status print
  static unsigned long lastPrint = 0;
  if (millis() - lastPrint > 2000) {
    lastPrint = millis();
    Serial.print("WiFi.status() = ");
    Serial.println(WiFi.status());
    if (WiFi.status() == WL_CONNECTED && !wifiConnected) {
      wifiConnected = true;
      Serial.print("Connected! IP: ");
      Serial.println(WiFi.localIP());
    }
  }

  // retry WiFi.begin if still not connected
  if (WiFi.status() != WL_CONNECTED && millis() - lastBegin >= retryInterval) {
    lastBegin = millis();
    Serial.println("Retry connect (WiFi.begin) ...");
    WiFi.disconnect();
    delay(50);
    if (useStaticIP) {
      WiFi.config(localIP, gateway, subnet, dns1);
    }
    WiFi.begin(WIFI_SSID, WIFI_PASS);
  }

  // If enough time passed, trigger another scan
  if (millis() - lastScan > scanInterval) {
    Serial.println("Starting new scan...");
    WiFi.scanNetworks(true, true);
    lastScan = millis();
  }
}
