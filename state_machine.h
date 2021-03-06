#ifndef _STATE_MACHINE_
#define _STATE_MACHINE_

void state_machine();

/*
  The following functions are to determine actions in the diffferent states according to current input.
*/
void state_up();
void state_down();
void state_stop_u();
void state_stop_d();
void state_stop();
void state_emergency();

void clear_all_orders();
int other_orders(int current_floor);

#endif
