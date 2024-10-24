//Note: only currently valid for one edge. Will need to expand to all edges after testing.
//#include "mcc_generated_files/mcc.h"
#ifndef Util_TIM_H
#define	Util_TIM_H

#include <stdint.h>

#define MAX_TIME 0xFFFF

typedef uint32_t time_t;

//time_acc will be a counter of 10 ms intervals
//if time = last_time - time
//Note: Assume mori never runs beyond time_counter

// ------------------------------------------------------ //
// --------------- T1 Specific functions ---------------- //
// ------------------------------------------------------ // 
void Util_TIM_initiate_timer(void);

// Called once every T1 (at 100 Hz)
void Util_TIM_increment_timers(void);

time_t Util_TIM_get_time(void);

volatile uint16_t Util_TIM_get_time_diff(time_t);

#endif