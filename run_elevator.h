#ifndef _RUN_ELEVATOR_H_
#define _RUN_ELEVATOR_H_

#include "elev.h"
#include <time.h>

#define WAIT 3

void initialize_elevator();
void stop_elev();
void run_elevator();
void update_timer();

typedef enum {UP, DOWN, STOP_U, STOP_D, STOP, EMERGENCY, UNDEF} state_t; // States of system

int order_list[N_FLOORS];
state_t current_state;

clock_t timer;

#endif