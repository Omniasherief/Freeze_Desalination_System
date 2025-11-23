#ifndef COOLING_H
#define COOLING_H

#define COOLING_PIN 33

bool cooling_state = false;

void initCooling() {
  pinMode(COOLING_PIN, OUTPUT);
  digitalWrite(COOLING_PIN, LOW);
}

void updateCooling() {
  digitalWrite(COOLING_PIN, cooling_state ? HIGH : LOW);
}

#endif
