// This is the main loop

#include "elev.h"
#include "state_machine.h"
#include "run_elevator.h"
#include <stdio.h>
#include <unistd.h>

void run_elevator(){
  
  
  // Elevator is started and state is unknown.

  current_state = UNDEF; 
  initialize_elevator();
  
  // Elevator is now at some floor.
  while(1){
    //break; // ** EXIT AT ONCE ** (Comment out to disable)
    state_machine(); // TO DO: Fix this. This is not pretty... Possible fix: move current_state to the header file.
    if (elev_get_stop_signal()) {
      elev_set_speed(0);
      break;
    }
  }
}


void initialize_elevator(){  
  elev_set_speed(-300);
  current_state = DOWN;
  while (elev_get_floor_sensor_signal() == -1)
    // WAIT UNTIL FIRST FLOOR IS REACHED
    ;
  stop_elev();
  current_state = STOP; // Current state is now AT_FLOOR
}

void stop_elev(){
  if (current_state == UP) {
    elev_set_speed(-300);
    usleep(5000);
    elev_set_speed(0);
  } 
  if (current_state == DOWN) {
    elev_set_speed(300);
    usleep(5000);
    elev_set_speed(0);
  }
}
