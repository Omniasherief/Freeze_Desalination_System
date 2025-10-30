#ifndef PUMPS_H
#define PUMPS_H

#define PUMP1_PIN 25
#define PUMP2_PIN 26

bool pump1_state = false;
bool pump2_state = false;

void initPumps() {
  pinMode(PUMP1_PIN, OUTPUT);
  pinMode(PUMP2_PIN, OUTPUT);
  digitalWrite(PUMP1_PIN, LOW);
  digitalWrite(PUMP2_PIN, LOW);
}

void updatePumps() {
  digitalWrite(PUMP1_PIN, pump1_state ? HIGH : LOW);
  digitalWrite(PUMP2_PIN, pump2_state ? HIGH : LOW);
}

#endif
