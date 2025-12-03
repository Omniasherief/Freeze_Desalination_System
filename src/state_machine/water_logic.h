#ifndef WATER_LOGIC_H
#define WATER_LOGIC_H

#include "../sensors/ls_sensors.h"
#include "../sensors/temperature.h"
#include "../actuators/pumps.h"
#include "../actuators/valves.h"
#include "../actuators/cooling.h"

// Define the stages of the water treatment cycle
enum WaterState {
  STAGE1, // Initial Filling / Check for water
  STAGE2, // Tank 1 Filling complete, Start Tank 3 Filling
  STAGE3, // Tank 3 Filling complete, Start Cooling
  STAGE4, // Cooling complete, Start Brine Discharge (Valve 1)
  STAGE5  // Final Drainage/Recycle and Restart (Valve 2)
};

WaterState currentState = STAGE1;

void initStateMachine() {
  currentState = STAGE1;
}

// Function to manage the state transitions of the water system
void updateStateMachine() {
  switch(currentState){
    case STAGE1:
      // Fill Phase (Pump 1) - Condition to start filling
      if(!LS2_state){ // If LS2 (Low Level Sensor) detects no water, start Pump 1
        pump1_state = true;
        currentState = STAGE2;
      }
      break;

    case STAGE2:
      // Primary Tank Filling Complete - Condition to stop P1 and start P2
      if(LS2_state && LS1_state){ // If both LS2 and LS1 (High Level Sensor) detect water
        pump1_state = false;
        pump2_state = true; // Start Pump 2 to fill Tank 3
        currentState = STAGE3;
      }
      break;

    case STAGE3:
      // Secondary Tank Filling Complete - Condition to stop P2 and start Cooling
      if(LS3_state){ // If LS3 (Tank 3 Level Sensor) detects water
        pump2_state = false;
        cooling_state = true; // Start the cooling process (desalination)
        currentState = STAGE4;
      }
      break;

    case STAGE4:
      // Cooling Complete - Condition to stop Cooling and discharge brine (Valve 1)
      if(waterTemp <= -2){ // If water temperature reaches -2°C or lower
        cooling_state = false;
        valve1_state = true;
        valve1_timer = millis();  // Start timing for Valve 1's open duration (Brine discharge)
        currentState = STAGE5;
      }
      break;

    case STAGE5:
      // Final Drainage and Restart - Controlled drainage via Valve 2
      
      // 1. Conditional Opening: Open Valve 2 once the temperature reaches the reset threshold (3 or 5)
      //    We check !valve2_state to ensure we only open it once
      if((waterTemp == 3 || waterTemp == 5) && !valve2_state){
        valve2_state = true;
        valve2_timer = millis();  // Start timing for Valve 2's open duration (Final drain)
      }
      
      // 2. Transition Condition: Move to STAGE1 ONLY after Valve 2 has closed
      //    (Valve 2 is closed automatically in updateValves() after its set timer runs out, 
      //     confirming the drain duration has passed).
      if((waterTemp == 3 || waterTemp == 5) && !valve2_state){
        // Now that Valve 2 is closed, we can safely restart the cycle.
        pump1_state = true; // Start Pump 1 to be ready for STAGE1 logic
        currentState = STAGE1;
      }
      break;
  }

  // Actuator updates must be called outside the switch to ensure continuous control
  // and timer management is executed at every loop iteration.
  updatePumps();
  updateValves();
  updateCooling();
}

#endif
