
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

    WiFi.mode(WIFI_STA);  
    WiFi.disconnect();
    delay(50);            
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
        delay(50);  // reconection delay
        WiFi.begin(WIFI_SSID, WIFI_PASS);
    }
}

#endif
