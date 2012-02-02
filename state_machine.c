/*
  state_machine.c
  
  Andreas Hystad & Thomas Stenersen (c) 2012
  
  This is the state machine for the elevator. All input and orders should be
  taken care of in this function according to the current state of the
  elevator.
  
  order_list[current_floor] == 0 => no order in current_floor
  order_list[current_floor] == 1 => external 'up' order in current_floor
  order_list[current_floor] == -1 => external 'down' order in current_floor
  order_list[current_floor] == 2 => internal order in current_floor 
*/

#include <stdio.h>
#include "run_elevator.h"
#include "state_machine.h"
#include "elev.h"
#include "io_signals.h"

void state_machine(){
  
  //printf("Current state: %d\n",(int)current_state);
  
  switch (current_state) {
    case UP:
      state_up();
      break;
    case DOWN:
      state_down();
      break;
    case STOP_U:
      state_stop_u();
      break;
    case STOP_D:
      state_stop_d();
      break;
    case STOP:
      state_stop();
      break;
    case EMERGENCY:
      state_emergency();
      break;
    case UNDEF:
      initialize_elevator();
      break;
    default:
      current_state = UNDEF;
  }
}

/*
  This function is called during state == UP. It handles all possible scenarios during this state.
*/
void state_up(){
  
  int current_floor = elev_get_floor_sensor_signal();
  
  if (current_floor != -1){ 
    
    // STOP if order in same direction or internal order.
    if (order_list[current_floor] == 2 || order_list[current_floor] == 1){
      stop_elev();
      
      // Reset order_list[] for this floor
      order_list[current_floor] = 0;
      
      // Are there other orders above this floor? If so, next state is STOP_U, else go to STOP.
      if (other_orders(current_floor)){
	current_state = STOP_U;
      } else {
	current_state = STOP;
      }  
    }
    
    // If there is an external order going downwards and no other orders above it, handle the order.
    if (order_list[current_floor] == -1 && other_orders(current_floor) == 0){
      stop_elev();
      order_list[current_floor] = 0; // Reset order_list[] for this floor
      current_state = STOP;
    }
    
    // Should not be necessary, but if top floor is reaced: STOP.
    if (current_floor == (N_FLOORS-1)){
      stop_elev();
      current_state = STOP;
    }
  }
}

/*
  This function is called during state == DOWN. It handles all possible scenarios during this state.
*/
void state_down(){
  
  int current_floor = elev_get_floor_sensor_signal();
  
  if (current_floor != -1){
    
    // STOP if order in same direction or internal order.
    if (order_list[current_floor] == 2 || order_list[current_floor] == -1){
      stop_elev();
      
      // Reset order_list[] for this floor
      order_list[current_floor] = 0;
      
      // Are there other orders above this floor? If so, next state is STOP_D, else go to STOP.
      if (other_orders(current_floor)){
	current_state = STOP_D;
      }
      else {
	current_state = STOP;
      }
    }
    
    // If there is an external order going upwards and no other orders below it, handle the order.
    if (order_list[current_floor] == 1 && other_orders(current_floor) == 0){
      stop_elev();
      order_list[current_floor] = 0; // Reset order_list[] for this floor
      current_state = STOP;
    }
    
    // Should not be necessary, but if bottom floor is reaced: STOP.
    if (current_floor == 0){
      stop_elev();
      current_state = STOP;
    }
    
  }
}

/*
  This function is called during state == STOP_U. It handles all possible scenarios during this state.
*/
void state_stop_u(){
  
  int current_floor = elev_get_floor_sensor_signal();
  
  // Should not need to test this, but just to be sure...
  if (current_floor != -1) {
    
    // If the internal or external button(s) are pressed: update timer.
    if  (order_list[current_floor] != 0)
      update_timer();
    
    // After timer is updated: clear orders in current floor.
    order_list[current_floor] = 0;
  }
  
  // After timer is expired. Continue upwards.
  if (clock()>timer){
    elev_set_speed(300);
    current_state = UP;
  }
}

/*
  This function is called during state == STOP_U. It handles all possible scenarios during this state.
*/
void state_stop_d(){
  
  int current_floor = elev_get_floor_sensor_signal();
  
  // Should not need to test this, but just to be sure...
  if (current_floor != -1) {
    
    // If the internal or external button(s) are pressed: update timer.
    if  (order_list[current_floor] != 0)
      update_timer();
    
    // After timer is updated: clear orders in current floor.
    order_list[current_floor] = 0;
  }
  
  // After timer is expired. Continue downwards.
  if (clock()>timer){
    elev_set_speed(-300);
    current_state = DOWN;
  }
}

/*
  This function is called during state == STOP. It handles all possible scenarios during this state.
*/
void state_stop(){
  
  int current_floor = elev_get_floor_sensor_signal();
  
  // Should not need to test this, but just to be sure...
  if (current_floor != -1) {
    
    // If the internal or external button(s) are pressed: update timer.
    if (order_list[current_floor] != 0)
      update_timer();
    
    // After timer is updated: clear orders in current floor.
    order_list[current_floor] = 0;
  }
  
  // After timer is expired. Search for new orders.
  if (clock()>timer){
    
    for (int i=0;i<N_FLOORS;i++){
      if (order_list[i] != 0) {
	
	// If the order is below the current floor: next state = DOWN.
	if (i<current_floor) {
	  elev_set_speed(-300);
	  current_state = DOWN;
	}

	// If the order is above the current floor: next state = UP.
	if (i>current_floor) {
	  elev_set_speed(300);
	  current_state = UP;
	}
      } // End if
    } // End for
  } // End if
}

/*
  This function is called during state == EMERGENCY. It handles all possible scenarios during this state.
  
  To get out of this state, someone has to press one of the internal floor buttons. The elevator will then
  be reinitialized.
*/
void state_emergency(){
  
  // Stop the elevator.
  elev_set_speed(0);

  io_clear_all_lights();
  
  clear_all_orders();

  // Check if someone presses one of the internal floor buttons.
  for (int i=0;i<N_FLOORS;i++){
    if (elev_get_button_signal(BUTTON_COMMAND,i)){
      elev_set_button_lamp(BUTTON_COMMAND, i, 1);
      current_state = UNDEF;
    }
  } 
}

void clear_all_orders(){
  for (int i=0;i<N_FLOORS;i++)
    order_list[i]=0;
}

/*
  This function searches for orders above or below the current floor. This is to optimize the order handling.
*/
int other_orders(int current_floor){
  switch (current_state) {
    case UP:
      // Search through the orders for the floors above the current.
      for (int i=current_floor+1;i<N_FLOORS;i++){
	
	// If there are orders above the current floor return 1 (TRUE).
	If (order_list[i] != 0)
	  return 1;
      }
      
      break;
    case DOWN:
      // Search through the orders for the floors above the current.
      for (int i=current_floor-1;i>=0;i--) {
	
	// If there are orders below the current floor return 1 (TRUE).
	if (order_list[i] != 0)
	  return 1;
      }
      break;
      
    default:
      return 0;
  }
  return 0; // If all else fails.. Return 0
}
