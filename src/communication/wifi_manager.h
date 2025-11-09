// // src/communication/wifi_manager.h
// #ifndef WIFI_MANAGER_H
// #define WIFI_MANAGER_H

// #include <WiFi.h>

// char WIFI_SSID[] = "we23";      // change
// char WIFI_PASS[]= "123456WE";  // 

// void setupWiFi() {
//   // Serial.print("Connecting to WiFi ");
//   // Serial.print(WIFI_SSID);
//   // WiFi.begin(WIFI_SSID, WIFI_PASS);
//   // unsigned long start = millis();
//   // while (WiFi.status() != WL_CONNECTED) {
//   //   delay(500);
//   //   Serial.print(".");
//   //   if (millis() - start > 20000) { // time out 20s
//   //     Serial.println(" WiFi connection timed out");
//   //     break;
//   //   }
//   // }
//   // if (WiFi.status() == WL_CONNECTED) {
//   //   Serial.println();
//   //   Serial.println("WiFi connected. IP: ");
//   //   Serial.println(WiFi.localIP());
//   // } else {
//   //   Serial.println("WiFi not connected.");
//   // }
 


//       // Serial.print("Connecting to WiFi: ");
//       // Serial.println(WIFI_SSID);
//       // WiFi.begin(WIFI_SSID, WIFI_PASS);

// }

// #endif

#ifndef WIFI_MANAGER_H
#define WIFI_MANAGER_H

#include <WiFi.h>

const char* WIFI_SSID = "BORM";
const char* WIFI_PASS = "Mahmoud@No24102013";

bool wifiConnected = false;
unsigned long lastAttempt = 0;
constexpr unsigned long retryInterval = 10000; // 10s

void setupWiFi() {
    Serial.print("Starting WiFi connection to: ");
    Serial.println(WIFI_SSID);

    WiFi.mode(WIFI_STA);  // مهم لتجنب التعليق
    WiFi.disconnect();
    delay(50);            // delay قصير بعد disconnect
    WiFi.begin(WIFI_SSID, WIFI_PASS);
}

void handleWiFi() {
    if (WiFi.status() == WL_CONNECTED) {
        if (!wifiConnected) {
            wifiConnected = true;
            Serial.println("✅ WiFi connected!");
            Serial.print("IP: ");
            Serial.println(WiFi.localIP());
        }
        return;
    }

    wifiConnected = false;

    if (millis() - lastAttempt >= retryInterval) {
        lastAttempt = millis();
        Serial.print("🔁 Retrying WiFi connection... Status: ");
        Serial.println(WiFi.status());

        WiFi.disconnect();
        delay(50);  // delay قبل إعادة الاتصال
        WiFi.begin(WIFI_SSID, WIFI_PASS);
    }
}

#endif
