#ifndef WATER_LOGIC_H
#define WATER_LOGIC_H

#include "../sensors/ls_sensors.h"
#include "../sensors/temperature.h"
#include "../actuators/pumps.h"
#include "../actuators/valves.h"
#include "../actuators/cooling.h"

enum WaterState {
  STAGE1,
  STAGE2,
  STAGE3,
  STAGE4,
  STAGE5
};

WaterState currentState = STAGE1;

void initStateMachine() {
  currentState = STAGE1;
}

void updateStateMachine() {
  switch(currentState){
    case STAGE1:
      if(!LS2_state){ // no water
        pump1_state = true;
        currentState = STAGE2;
      }
      break;

    case STAGE2:
      if(LS2_state && LS1_state){ // water detected
        pump1_state = false;
        pump2_state = true;
        currentState = STAGE3;
      }
      break;

    case STAGE3:
      if(LS3_state){ // water detected
        pump2_state = false;
        cooling_state = true;
        currentState = STAGE4;
      }
      break;

    case STAGE4:
      if(waterTemp <= -2){
        cooling_state = false;
        valve1_state = true;
        valve1_timer = millis();  // NEW: start timing valve1 open duration
        currentState = STAGE5;
      }
      break;

    case STAGE5:
      if(waterTemp == 3 || waterTemp == 5){
        valve2_state = true;
        valve2_timer = millis();  // NEW: start timing valve2 open duration
        pump1_state = true;
        currentState = STAGE1;   // NEW: restart the cycle
      }
      break;
  }

  updatePumps();   // NEW: refresh all pump states
  updateValves();  // NEW: refresh all valve states
  updateCooling(); // NEW: refresh cooling system state
}

#endif