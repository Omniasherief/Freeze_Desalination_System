#ifndef VALVES_H
#define VALVES_H

#define VALVE1_PIN 27
#define VALVE2_PIN 32

bool valve1_state = false;
bool valve2_state = false;

unsigned long valve1_timer = 0;
unsigned long valve2_timer = 0;

void initValves() {
  pinMode(VALVE1_PIN, OUTPUT);
  pinMode(VALVE2_PIN, OUTPUT);
  digitalWrite(VALVE1_PIN, LOW);
  digitalWrite(VALVE2_PIN, LOW);
}

void updateValves() {
  unsigned long now = millis();
  
  if(valve1_state && now - valve1_timer > 5000){
    valve1_state = false;
  }
  
  if(valve2_state && now - valve2_timer > 5000){ // we can another ls sensor to make sure this tank is empty but we add timer instead
    valve2_state = false;
  }

  digitalWrite(VALVE1_PIN, valve1_state ? HIGH : LOW);
  digitalWrite(VALVE2_PIN, valve2_state ? HIGH : LOW);
}

#endif
