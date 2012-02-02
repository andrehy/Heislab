#include "io_signals.h"
#include "run_elevator.h"
#include "elev.h"
#include <time.h>

void io_signals(){
  
  int current_floor = elev_get_floor_sensor_signal();
  
  io_set_floor_indicator(current_floor);
  
  io_clear_lights_current_floor(current_floor);
  
  io_external_orders(current_floor);
  
  io_internal_orders(current_floor);
  
  io_door();
  
  io_check_stop();
}

void io_set_floor_indicator(int current_floor){
  if (current_floor != -1)
    elev_set_floor_indicator(current_floor);
}

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

void io_external_orders(){
  // Get and set outdoor ups
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
  
  // Get and set outdoor downs
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

void io_internal_orders(){
  
  // Get and set internal shait
  for (int i=0;i<N_FLOORS;i++){
    if (elev_get_button_signal(BUTTON_COMMAND,i)){
      elev_set_button_lamp(BUTTON_COMMAND, i, 1);
      order_list[i]=2;
    }
  }
    
}

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