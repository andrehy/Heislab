#include "run_elevator.h"
#include "state_machine.h"

/*
  This is the state machine for the elevator. All input and orders should be
  taken care of in this function according to the current state of the
  elevator.
  
  int order_list[N_FLOORS] = 0;
  
  order_list[zhe_floor] == 0 => no order in zhe_floor
  order_list[zhe_floor] == 1 => external 'up' order in zhe_floor
  order_list[zhe_floor] == -1 => external 'down' order in zhe_floor
  order_list[zhe_floor] == 2 => internal order in zhe_floor
  
<<<<<<< HEAD
*/


int state_machine(int state){
  switch (state) {
    case 0: // UP
      /*
        Check floor sensor. If floor is reached, check orderlist. Stop if order_list[zhe_floor] == 1 or 2.
      */
      break;
    case 1: // DOWN
=======
  
  
*/

int order_list[N_FLOORS] = 0;

void state_machine(){
  switch (current_state) {
    case UP:
      state_up();
      
      /*
      
	state_up(); => if(check_sensors() == FLOOR) -> 
	
        Check floor sensor. If floor is reached, check orderlist. Stop if order_list[zhe_floor] == 1 or 2.
      */
      break;
    case DOWN: // DOWN
>>>>>>> New update
      /*
        Check floor sensor. If floor is reached, check orderlist. Stop if order_list[zhe_floor] == -1 or 2.
      */
      break;
<<<<<<< HEAD
    case 2: // AT_FLOOR
=======
    case STOP_U:
      /*
        Check obstruction() etc. and change state accordingly..
      */
      break;
    case STOP_D:
      /*
        Check obstruction() etc. and change state accordingly..
      */
      break;
    case STOP:
      state_stop();
>>>>>>> New update
      /*
        Check obstruction() etc. and change state accordingly..
      */
      break;
<<<<<<< HEAD
    case 3: // EMERGENCY
      // DO SOMETHING
      break;
    case 4: // UNDEF
      initialize_elevator(); // Change of state is handled in initialize_elevator().
      break;
    default:
    state = 4; // Return UNDEF?
  }
  return state;
=======
    case EMERGENCY: // EMERGENCY
      // DO SOMETHING
      break;
    case UNDEF: // UNDEF
      initialize_elevator(); // Change of state is handled in initialize_elevator().
      break;
    default:
      current_state = UNDEF; // Return UNDEF?
  }
}

void state_up(){
  
  int current_floor = elev_get_floor_sensor_signal();
  
  if (current_floor != -1){
    
    elev_set_floor_indicator(current_floor);
    
    if (order_list[current_floor] == 2 || order_list[current_floor] == 1){
      stop_elev();
      order_list[current_floor] = 0; // Resett order_list[] for this floor
      if (other_orders() == 0)
	current_state = STOP;
      else
	current_state = STOP_UP;
      
    }
    if (order_list[current_floor] == -1 && other_orders(current_floor) == 0){
      stop_elev();
      order_list[current_floor] = 0;
      current_state = STOP_D;
    }
    /*
      TODO: RESET FLOOR_INDICATOR!
    */
    
  }
>>>>>>> New update
}

/* 
  This function recieves the current floor and searches through the order_list[] and returns..?

*/
int other_orders(int current_floor){
  switch (current_state) {
    case UP:
      
      for (int i=current_floor+1;i<N_FLOORS;i++){
	if (order_list[i] == -1)
	  return 1;
      }
      
      break;
    case DOWN:
      
      for (int i=current_floor-1;i>=0;i--) {
	if (order_list[i] == 1)
	  return 1;
      }
      break;
    case default:
      return 0;
  }
  
  return 0; // if all else fails.. Return 0
}

void state_stop(){
  // TODO: FIX EVERYTHING!
}
