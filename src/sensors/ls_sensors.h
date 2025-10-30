#ifndef LS_SENSORS_H
#define LS_SENSORS_H

#define LS1_PIN 12
#define LS2_PIN 13
#define LS3_PIN 15

bool LS1_state = false;
bool LS2_state = false;
bool LS3_state = false;

void initLSSensors() {
  pinMode(LS1_PIN, INPUT);
  pinMode(LS2_PIN, INPUT);
  pinMode(LS3_PIN, INPUT);
}

void readLSSensors() {
  LS1_state = digitalRead(LS1_PIN);
  LS2_state = digitalRead(LS2_PIN);
  LS3_state = digitalRead(LS3_PIN);

  Serial.print(" LS1: "); 
  Serial.print(LS1_state);
  Serial.print(" LS2: "); 
  Serial.print(LS2_state);
  Serial.print(" LS3: "); 
  Serial.println(LS3_state);
}

#endif
