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

// Global clients
WiFiClient espClient;
PubSubClient client(espClient);

// System mode flag: false = AUTO, true = MANUAL
bool manual_mode_active = false;  // start in AUTO by default

unsigned long previousMillis = 0;
const unsigned long INTERVAL = 1000; // 1 second

void setup() {
    Serial.begin(115200);

    // ---- WiFi setup ----
    setupWiFi();

    // ---- MQTT setup ----
    client.setServer(MQTT_BROKER, MQTT_PORT);
    client.setCallback(mqttCallback);

    // ---- Sensors init ----
    initLSSensors();
    initTemperatureSensor();
    initTDSSensor();

    // ---- Actuators init ----
    initPumps();
    initValves();
    initCooling();

    // ---- State machine ----
    initStateMachine();

    Serial.println("Setup complete. System starting in AUTO mode.");
}

void loop() {
    // Reconnect MQTT if disconnected
    if (!client.connected()) {
        reconnectMQTT();
    }
    client.loop();

    unsigned long currentMillis = millis();
    if (currentMillis - previousMillis >= INTERVAL) {
        previousMillis = currentMillis;

        // ---- Read sensors ----
        readLSSensors();
        readTemperature();
        readTDS();

        // ---- Update state machine only in AUTO mode ----
        if (!manual_mode_active) {
            updateStateMachine();
        }

        // ---- Publish all data ----
        publishAllMQTT();
    }
}
