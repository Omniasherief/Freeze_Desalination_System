// #include <WiFi.h>
// #include <PubSubClient.h>

// #include "communication/wifi_manager.h"
// #include "communication/mqtt_handler.h"

// #include "sensors/ls_sensors.h"
// #include "sensors/temperature.h"
// #include "sensors/tds_sensor.h"

// #include "actuators/pumps.h"
// #include "actuators/valves.h"
// #include "actuators/cooling.h"

// #include "state_machine/water_logic.h"

// // =========================================
// // Global clients 
// // =========================================
// // WiFiClient espClient;
// // PubSubClient client(espClient);

// // System mode flag: false = AUTO, true = MANUAL
// bool manual_mode_active = false;  // start in AUTO by default

// unsigned long previousMillis = 0;
// const unsigned long INTERVAL = 1000; // 1 second

// void setup() {
//     Serial.begin(115200);

//     // ---- WiFi setup ----
//     // setupWiFi();

//     // ---- MQTT setup ----
//     // client.setServer(MQTT_BROKER, MQTT_PORT);
//     // client.setCallback(mqttCallback);

//     // ---- Sensors init ----
//     initLSSensors();
//     initTemperatureSensor();
//     initTDSSensor();

//     // ---- Actuators init ----
//     initPumps();
//     initValves();
//     initCooling();

//     // ---- State machine ----
//     initStateMachine();

//     Serial.println("Setup complete. System starting in AUTO mode (offline).");
// }

// void loop() {
//     // ---- MQTT reconnect & loop ----
//     // if (!client.connected()) {
//     //     reconnectMQTT();
//     // }
//     // client.loop();

//     unsigned long currentMillis = millis();
//     if (currentMillis - previousMillis >= INTERVAL) {
//         previousMillis = currentMillis;

//         // ---- Read sensors ----
//         readLSSensors();
//         readTemperature();
//         readTDS();

//         // ---- Update state machine only in AUTO mode ----
//         if (!manual_mode_active) {
//             updateStateMachine();
//         }

//         // ---- Publish all data (disabled) ----
//         // publishAllMQTT();

//         //
//         Serial.println("Sensors read & logic updated (offline mode).");
//     }
// }








// // wifi works here 

// #include <WiFi.h>
// #include <PubSubClient.h>

// #include "communication/wifi_manager.h"
// #include "communication/mqtt_handler.h"

// #include "sensors/ls_sensors.h"
// #include "sensors/temperature.h"
// #include "sensors/tds_sensor.h"

// #include "actuators/pumps.h"
// #include "actuators/valves.h"
// #include "actuators/cooling.h"

// #include "state_machine/water_logic.h"

// // =========================================
// // Global MQTT
// // =========================================
// WiFiClient espClient;
// PubSubClient client(espClient);

// // =========================================
// // Flags & timers
// // =========================================
// bool manual_mode_active = false;
// unsigned long previousMillis = 0;
// const unsigned long INTERVAL = 1000; // كل ثانية

// // =========================================
// // Setup
// // =========================================
// void setup() {
// Serial.begin(115200);
// Serial.println("\n===========================");
// Serial.println("💧 Desalination Cooling System");
// Serial.println("===========================");

// // WiFi setup (non-blocking)
// setupWiFi();

// // MQTT setup
// client.setServer(MQTT_BROKER, MQTT_PORT);
// client.setCallback(mqttCallback);

// // Sensors & actuators
// initLSSensors();
// initTemperatureSensor();
// initTDSSensor();

// initPumps();
// initValves();
// initCooling();

// // State machine
// initStateMachine();

// Serial.println("✅ Setup complete. System running...");
// }

// // =========================================
// // Loop
// // =========================================
// void loop() {
// unsigned long currentMillis = millis();

// // Handle WiFi in background
// handleWiFi();

// // MQTT only when WiFi connected
// if (wifiConnected) {
// if (!client.connected()) {
// reconnectMQTT();
// }
// client.loop();
// }

// // Core logic every 1s
// if (currentMillis - previousMillis >= INTERVAL) {
// previousMillis = currentMillis;


// // Read sensors
// readLSSensors();
// readTemperature();
// readTDS();

// // Update logic (auto mode)
// if (!manual_mode_active) {
//   updateStateMachine();
// }

// // Publish data only if online
// if (wifiConnected && client.connected()) {
//   publishAllMQTT();
// }

// Serial.println("Sensors read & logic updated.");


// }
// }


// non_blocking mqtt

#include <WiFi.h>
#include <PubSubClient.h>

#include "communication/wifi_manager.h"
#include "communication/mqtt_handler.h"

#include "sensors/ls_sensors.h"
#include "sensors/temperature.h"
#include "sensors/tds_sensor.h"

#include "actuators/pumps.h"
#include "actuators/valves.h"
#include "actuators/cooling.h"

#include "state_machine/water_logic.h"

// =========================================
// Global MQTT
// =========================================
WiFiClient espClient;
PubSubClient client(espClient);

// =========================================
// Flags & timers
// =========================================
bool manual_mode_active = false;
unsigned long previousMillis = 0;
const unsigned long INTERVAL = 1000; 

// =========================================
// Setup
// =========================================
void setup() {
Serial.begin(115200);
Serial.println("\n===========================");
Serial.println("💧 Desalination Cooling System");
Serial.println("===========================");


// WiFi setup (non-blocking)
setupWiFi();

// MQTT setup
client.setServer(MQTT_BROKER, MQTT_PORT);
client.setCallback(mqttCallback);

// Sensors & actuators
initLSSensors();
initTemperatureSensor();
initTDSSensor();

initPumps();
initValves();
initCooling();

// State machine
initStateMachine();

Serial.println("✅ Setup complete. System running...");


}

// =========================================
// Loop
// =========================================
void loop() {
unsigned long currentMillis = millis();

// Handle WiFi in background
handleWiFi();

// MQTT only when WiFi connected
if (wifiConnected) {
    // Non-blocking reconnect
    reconnectMQTT_NonBlocking();

    // Execute MQTT loop only if connected
    if (client.connected()) {
        client.loop();
    }
}

// Core logic every 1s
if (currentMillis - previousMillis >= INTERVAL) {
    previousMillis = currentMillis;

    // Read sensors
    readLSSensors();
    readTemperature();
    readTDS();

    // Update logic (auto mode)
    if (!manual_mode_active) {
        updateStateMachine();
    }

    // Publish data only if online
    if (wifiConnected && client.connected()) {
        publishAllMQTT();
    }

    Serial.println("Sensors read & logic updated.");
}

}