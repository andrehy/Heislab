#include "runElevator.h"
#include "state_machine.h"

/*
  This is the state machine for the elevator. All input and orders should be
  taken care of in this function according to the current state of the
  elevator.
*/


int state_machine(int state){
  switch (state) {
    case 0: // UP
      // DO SOMETHING
      break;
    case 1: // DOWN
      // DO SOMETHING
      break;
    case 2: // AT_FLOOR
      // DO SOMETHING
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
