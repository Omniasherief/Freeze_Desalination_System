
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