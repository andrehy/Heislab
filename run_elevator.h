#ifndef _RUN_ELEVATOR_H_
#define _RUN_ELEVATOR_H_

void initialize_elevator();
void stop_elev();
void run_elevator();

typedef enum {UP, DOWN, STOP_U, STOP_D, STOP, EMERGENCY, UNDEF} state_t; // States of system

state_t current_state;

#endif