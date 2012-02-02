/*
  run_elevator.c

  Andreas Hystad & Thomas Stenersen (c) 2012
  
  This is the main function in the elevator program. It contains the main program loop.

 */

#include "elev.h"
#include "state_machine.h"
#include "run_elevator.h"
#include "io_signals.h"
#include <stdio.h>
#include <unistd.h>

void run_elevator(){
  
  // Elevator is started and state is unknown.
  
  // Initialize timer.
  timer = clock();
  
  current_state = UNDEF; 
  initialize_elevator();
  
  // Elevator is now at some floor and ready for orders.
  while(1){
    
    // Handles io-signals, (sets/clears lights etc.)
    io_signals();
    
    // Calls the state machine switch function.
    state_machine();
    
    // If ... exit the main loop and terminate the program.
    if (elev_get_button_signal(BUTTON_CALL_UP,0) && current_state == EMERGENCY){
      elev_set_speed(0);
      break;
    }
  }

  initialize_elevator();
}


void initialize_elevator(){  
  elev_set_speed(-300);
  
  current_state = DOWN;
  
  io_clear_all_lights();
  
  while (elev_get_floor_sensor_signal() == -1)
    ; // Wait until first floor is reached. 

  stop_elev();
  
  clear_all_orders();
  
  current_state = STOP;
}

void stop_elev(){
  
  update_timer();
  
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

void update_timer(){
  timer = clock() + WAIT*CLOCKS_PER_SEC;
}
