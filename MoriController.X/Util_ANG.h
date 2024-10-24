//Note: only currently valid for one edge. Will need to expand to all edges after testing.
//#include "mcc_generated_files/mcc.h"
#ifndef Util_ANG_H
#define	Util_ANG_H

#include <stdint.h>
#include "Util_TIM.h"

struct angle_state
{
float ang;
float vel;
time_t time;
};

#endif