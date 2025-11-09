// #ifndef MQTT_HANDLER_H
// #define MQTT_HANDLER_H

// #include <PubSubClient.h>
// #include "../sensors/ls_sensors.h"
// #include "../sensors/temperature.h"
// #include "../sensors/tds_sensor.h"
// #include "../actuators/pumps.h"
// #include "../actuators/valves.h"
// #include "../actuators/cooling.h"

// // External MQTT client
// extern WiFiClient espClient;
// extern PubSubClient client;

// // Global flag for system mode
// extern bool manual_mode_active;  // true = MANUAL, false = AUTO

// // MQTT broker configuration
// const char* MQTT_BROKER = "192.168.1.51";  // change
// const int MQTT_PORT = 1883;
// const char* MQTT_CLIENT_ID = "ESP32_WaterSystem";

// // ------------------------
// // MQTT callback function
// // ------------------------
// void mqttCallback(char* topic, byte* payload, unsigned int length) {
//     String msg;
//     for (unsigned int i = 0; i < length; i++)
//         msg += (char)payload[i];

//     Serial.print("MQTT msg on [");
//     Serial.print(topic);
//     Serial.print("] -> ");
//     Serial.println(msg);

//     String t = String(topic);

//     // ===== Mode switching =====
//     if (t == "water/control/mode") {
//         if (msg == "AUTO") manual_mode_active = false;
//         else if (msg == "MANUAL") manual_mode_active = true;
//         Serial.print("Mode changed: ");
//         Serial.println(manual_mode_active ? "MANUAL" : "AUTO");
//         return; // no further processing for mode message
//     }

//     // ===== Manual commands =====
//     // Only allow manual commands if system is in MANUAL mode
//     if (!manual_mode_active) return;

//     if (t == "water/control/pump1") {
//         pump1_state = (msg == "ON");
//         updatePumps();
//     }
//     else if (t == "water/control/pump2") {
//         pump2_state = (msg == "ON");
//         updatePumps();
//     }
//     else if (t == "water/control/valve1") {
//         if (msg == "OPEN") { valve1_state = true; valve1_timer = millis(); }
//         else if (msg == "CLOSE") valve1_state = false;
//         updateValves();
//     }
//     else if (t == "water/control/valve2") {
//         if (msg == "OPEN") { valve2_state = true; valve2_timer = millis(); }
//         else if (msg == "CLOSE") valve2_state = false;
//         updateValves();
//     }
// }

// // ------------------------
// // Reconnect to MQTT broker
// // ------------------------
// void reconnectMQTT() {
//     while (!client.connected()) {
//         Serial.print("Connecting MQTT...");
//         if (client.connect(MQTT_CLIENT_ID)) {
//             Serial.println("connected");
//             client.subscribe("water/control/#"); // subscribe to all control topics
//         } else {
//             Serial.print("failed, rc=");
//             Serial.print(client.state());
//             delay(2000);
//         }
//     }
// }

// // ------------------------
// // Publish single sensor or actuator
// // ------------------------
// void publishSensor(const char* topic, const char* payload) {
//     if (client.connected()) {
//         client.publish(topic, payload);
//     }
// }

// // ------------------------
// // Publish all sensors & actuators + system mode
// // ------------------------
// void publishAllMQTT() {
//     char buf[32];

//     // Level Sensors
//     publishSensor("water/LS1", LS1_state ? "1" : "0");
//     publishSensor("water/LS2", LS2_state ? "1" : "0");
//     publishSensor("water/LS3", LS3_state ? "1" : "0");

//     // Temperature
//     dtostrf(waterTemp, 6, 2, buf);
//     publishSensor("water/temperature", buf);

//     // TDS
//     dtostrf(tdsValue, 6, 2, buf);
//     publishSensor("water/tds", buf);

//     // Actuators
//     publishSensor("water/pump1/status", pump1_state ? "ON" : "OFF");
//     publishSensor("water/pump2/status", pump2_state ? "ON" : "OFF");
//     publishSensor("water/valve1/status", valve1_state ? "ON" : "OFF");
//     publishSensor("water/valve2/status", valve2_state ? "ON" : "OFF");
//     publishSensor("water/cooling/status", cooling_state ? "ON" : "OFF");

//     // Publish system mode
//     publishSensor("water/system/mode", manual_mode_active ? "MANUAL" : "AUTO");
// }

// #endif
#ifndef MQTT_HANDLER_H
#define MQTT_HANDLER_H

#include <PubSubClient.h>
#include "../sensors/ls_sensors.h"
#include "../sensors/temperature.h"
#include "../sensors/tds_sensor.h"
#include "../actuators/pumps.h"
#include "../actuators/valves.h"
#include "../actuators/cooling.h"

// External MQTT client
extern WiFiClient espClient;
extern PubSubClient client;

// Global flag for system mode
extern bool manual_mode_active;

// MQTT broker configuration
const char* MQTT_BROKER = "192.168.1.51";
const int MQTT_PORT = 1883;
const char* MQTT_CLIENT_ID = "ESP32_WaterSystem";

// Last retry timestamp
unsigned long lastMQTTRetry = 0;
const unsigned long mqttRetryInterval = 5000; // 5s

// MQTT callback
void mqttCallback(char* topic, byte* payload, unsigned int length) {
    String msg;
    for (unsigned int i = 0; i < length; i++)
        msg += (char)payload[i];

    Serial.print("MQTT msg on [");
    Serial.print(topic);
    Serial.print("] -> ");
    Serial.println(msg);

    String t = String(topic);

    if (t == "water/control/mode") {
        manual_mode_active = (msg != "AUTO");
        Serial.print("Mode changed: ");
        Serial.println(manual_mode_active ? "MANUAL" : "AUTO");
        return;
    }

    if (!manual_mode_active) return; // only manual commands allowed in MANUAL

    if (t == "water/control/pump1") { pump1_state = (msg == "ON"); updatePumps(); }
    else if (t == "water/control/pump2") { pump2_state = (msg == "ON"); updatePumps(); }
    else if (t == "water/control/valve1") { valve1_state = (msg=="OPEN"); if(msg=="OPEN") valve1_timer=millis(); updateValves();}
    else if (t == "water/control/valve2") { valve2_state = (msg=="OPEN"); if(msg=="OPEN") valve2_timer=millis(); updateValves();}
}

// ------------------------
// Non-blocking MQTT reconnect
// ------------------------
bool reconnectMQTT_NonBlocking() {
    if (client.connected()) return true;

    unsigned long now = millis();
    if (now - lastMQTTRetry < mqttRetryInterval) return false;

    lastMQTTRetry = now;
    Serial.print("Connecting MQTT...");
    if (client.connect(MQTT_CLIENT_ID)) {
        Serial.println("connected");
        client.subscribe("water/control/#");
        return true;
    } else {
        Serial.print("failed, rc=");
        Serial.println(client.state());
        return false; // لا توقف الكود
    }
}

// ------------------------
// Publish helpers (unchanged)
// ------------------------
void publishSensor(const char* topic, const char* payload) {
    if (client.connected()) client.publish(topic, payload);
}

void publishAllMQTT() {
    char buf[32];
    publishSensor("water/LS1", LS1_state?"1":"0");
    publishSensor("water/LS2", LS2_state?"1":"0");
    publishSensor("water/LS3", LS3_state?"1":"0");

    dtostrf(waterTemp,6,2,buf);
    publishSensor("water/temperature", buf);

    dtostrf(tdsValue,6,2,buf);
    publishSensor("water/tds", buf);

    publishSensor("water/pump1/status", pump1_state?"ON":"OFF");
    publishSensor("water/pump2/status", pump2_state?"ON":"OFF");
    publishSensor("water/valve1/status", valve1_state?"ON":"OFF");
    publishSensor("water/valve2/status", valve2_state?"ON":"OFF");
    publishSensor("water/cooling/status", cooling_state?"ON":"OFF");

    publishSensor("water/system/mode", manual_mode_active?"MANUAL":"AUTO");
}

#endif
