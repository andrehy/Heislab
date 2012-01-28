// This is the main loop

#include "elev.h"
#include "state_machine.h"
#include "runElevator.h"
#include <stdio.h>
#include <unistd.h>

state_t current_state; 

void run_elevator(){
  
  
  // Elevator is started and state is unknown.

  current_state = UNDEF; 
  elev_first_init();
  
  // Elevator is now at some floor.
  int a=1;
  while(1){
    //break; // ** EXIT AT ONCE ** (Comment out to disable)
    state_machine(&a);
    if (elev_get_stop_signal()) {
      elev_set_speed(0);
      break;
    }
  }
}


void elev_first_init(){  
  elev_set_speed(300);
  current_state = UP;
  while (elev_get_floor_sensor_signal() == -1)
    // WAIT UNTIL FIRST FLOOR IS REACHED
    ;
  stop_elev();
  current_state = AT_FLOOR; // Current state is now AT_FLOOR
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
