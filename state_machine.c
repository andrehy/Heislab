#include "runElevator.h"
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
  
*/


int state_machine(int state){
  switch (state) {
    case 0: // UP
      /*
        Check floor sensor. If floor is reached, check orderlist. Stop if order_list[zhe_floor] == 1 or 2.
      */
      break;
    case 1: // DOWN
      /*
        Check floor sensor. If floor is reached, check orderlist. Stop if order_list[zhe_floor] == -1 or 2.
      */
      break;
    case 2: // AT_FLOOR
      /*
        Check obstruction() etc. and change state accordingly..
      */
      break;
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
}
