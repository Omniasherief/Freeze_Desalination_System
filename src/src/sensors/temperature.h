#ifndef TEMPERATURE_H
#define TEMPERATURE_H

#include <OneWire.h>
#include <DallasTemperature.h>

#define ONE_WIRE_BUS 4 
OneWire oneWire(ONE_WIRE_BUS); 
DallasTemperature sensors(&oneWire);

float waterTemp = 0;
void initTemperatureSensor() {
  sensors.begin();
}

void readTemperature() {
  sensors.requestTemperatures(); // Ask the sensor to measure
  waterTemp = sensors.getTempCByIndex(0); // Read the first sensor on the bus
  Serial.print("Temperature: ");
  Serial.println(waterTemp);
}

#endif
