/*
  io_signals.c

  Andreas Hystad & Thomas Stenersen (c) 2012

  This file contains the functions that handles all I/O. These functions
  will also add orders in the order_list[] array.
 */

#include "io_signals.h"
#include "run_elevator.h"
#include "elev.h"
#include <time.h>

/*
  This is the main I/O function. It is called from the main loop and 
  calls other I/O sub-functions.
 */
void io_signals(){
  
  int current_floor = elev_get_floor_sensor_signal();
  
  io_set_floor_indicator(current_floor);
  
  io_clear_lights_current_floor(current_floor);
  
  io_external_orders(current_floor);
  
  io_internal_orders(current_floor);
  
  io_door();
  
  io_check_stop();
}

/*
  This function sets the external floor indicators when a new floor is reached.
 */
void io_set_floor_indicator(int current_floor){
  if (current_floor != -1)
    elev_set_floor_indicator(current_floor);
}

/*
  This function clears the internal and/or external button lights whenever an
  order in that floor is handled.

  THIS FUNCTION COULD BE OPTIMIZED...
 */
void io_clear_lights_current_floor(int current_floor){
  
  if (current_floor >= 0 && current_floor < N_FLOORS){
    
    if (current_state == STOP){
      elev_set_button_lamp(BUTTON_COMMAND,current_floor,0);
      
      if (current_floor != 0){
	elev_set_button_lamp(BUTTON_CALL_DOWN,current_floor,0);
      }
      if (current_floor < (N_FLOORS-1)){
	elev_set_button_lamp(BUTTON_CALL_UP,current_floor,0);
      }
    }
    
    if (current_state == STOP_U && current_floor < (N_FLOORS-1)) {
      elev_set_button_lamp(BUTTON_CALL_UP,current_floor,0);
      elev_set_button_lamp(BUTTON_CALL_DOWN,current_floor,0);
      elev_set_button_lamp(BUTTON_COMMAND,current_floor,0);
    }
    
    if (current_state == STOP_D && current_floor > 0){
      elev_set_button_lamp(BUTTON_CALL_UP,current_floor,0);
      elev_set_button_lamp(BUTTON_CALL_DOWN,current_floor,0);
      elev_set_button_lamp(BUTTON_COMMAND,current_floor,0);
    }
    
  }
}

/*
  This function handles the external orders.
 */

void io_external_orders(){
  // Get and set outdoor orders up.
  for (int i=0;i<N_FLOORS-1;i++){
    if (elev_get_button_signal(BUTTON_CALL_UP,i)){
      elev_set_button_lamp(BUTTON_CALL_UP, i, 1);
      if(order_list[i]==-1 || order_list[i]==2){
	order_list[i]=2;
      }
      else{
	order_list[i]=1;
      }
    }
  }
  
  // Get and set outdoor orders down.
  for (int i=N_FLOORS-1;i>0;i--){
    if (elev_get_button_signal(BUTTON_CALL_DOWN,i)){
      elev_set_button_lamp(BUTTON_CALL_DOWN, i, 1);
      if(order_list[i]==1 || order_list[i]==2){
	order_list[i]=2;
      }
      else{
	order_list[i]=-1;
      }
    }
  }
}

/*
  This function handles the internal orders.
 */
void io_internal_orders(){
  
  // Get and set internal orders.
  for (int i=0;i<N_FLOORS;i++){
    if (elev_get_button_signal(BUTTON_COMMAND,i)){
      elev_set_button_lamp(BUTTON_COMMAND, i, 1);
      order_list[i]=2;
    }
  }
    
}

/*
  This function handles the opening and closing of the elevator door.
 */
void io_door(int current_floor){
  if (clock() < timer) {
    if (elev_get_obstruction_signal()) {
      update_timer();
    }
    elev_set_door_open_lamp(1);
  } else {
    elev_set_door_open_lamp(0);
  }
}

void io_check_stop(){
  if (elev_get_stop_signal()){
    current_state = EMERGENCY;
    elev_set_stop_lamp(1);
  }
}

void io_clear_all_lights(){
  for (int i=0;i<N_FLOORS-1;i++){
    elev_set_button_lamp(BUTTON_CALL_UP, i, 0);
  }
  
  for (int i=N_FLOORS-1;i>0;i--){
    elev_set_button_lamp(BUTTON_CALL_DOWN, i, 0);
  }
  
  for (int i=0;i<N_FLOORS;i++){
    elev_set_button_lamp(BUTTON_COMMAND, i, 0);
  }
  
  if (current_state != EMERGENCY)
    elev_set_stop_lamp(0);
}
