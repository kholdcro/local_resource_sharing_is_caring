#include "Defs_GLB.h"
#include "Util_TIM.h"

volatile time_t time_counter = 0;

// ------------------------------------------------------ //
// --------------- T1 Specific functions ---------------- //
// ------------------------------------------------------ // 
void Util_TIM_initiate_timer(void) {
    time_counter = 0;
}

// Called once every T1 (at 100 Hz)
void Util_TIM_increment_timers(void) {
	time_counter++;
}

time_t Util_TIM_get_time(void){
	return time_counter;
}

volatile uint16_t Util_TIM_get_time_diff(time_t time){
	return (uint16_t)((MAX_TIME + time_counter - time)%MAX_TIME); //? why is there a MAX_TIME?
}
