// Author: Kevin Holdcroft
// Approximations for math functions that are faster than the standard library functions
#ifndef Util_MTH_H
#define	Util_MTH_H


#include <string.h>
#include <stdint.h>
#include <math.h>


#define ONEQTR_PI 0.785398f
#define THRQTR_PI 2.356194f
#define epsilon 1e-10f

// From: https://gist.github.com/volkansalma/2972237
float atan2_approximation1(float y, float x) {
    //http://pubs.opengroup.org/onlinepubs/009695399/functions/atan2.html Volkan SALMA
	float r, angle;
	const float abs_y = fabs(y) + epsilon;      // kludge to prevent 0/0 condition
	if ( x < 0.0f ){
		r = (x + abs_y) / (abs_y - x);
		angle = THRQTR_PI;
	} else {
		r = (x - abs_y) / (x + abs_y);
		angle = ONEQTR_PI;
	}
	angle += (0.1963f * r * r - 0.9817f) * r;
	if ( y < 0.0f ) return(-angle); // negate if in quad III or IV
	else return(angle);
}

// From: https://codegolf.stackexchange.com/questions/85555/the-fastest-square-root-calculator
float fast_sqrt(float n) {
    long i;
    float y = 1.0f / n;
    const float x = y * 0.5f;
    memcpy(&i, &y, sizeof y); // i = *(long *)&y;
    i = 0x5f3759df - (i >> 1);
    memcpy(&y, &i, sizeof i); // y = *(float *)&i;
    y = y * (1.5f - (x * y * y)); 
    return y;
}

#endif