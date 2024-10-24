#include "math.h"
#include "Sens_EDG.h"
#include "Util_TIM.h"
#include "Util_ANG.h"
#include "Coms_ESP.h"
#include <stdbool.h>


#define MIN_ANGLE_STDEV 1
#define MIN_VEL_STDEV 0.1

#define PRED_SIZE 10 //Size of prediction regression
#define MAX_ACC_TIME 30 //Time to reject ACC measurments

#define LOC_VALID_MASK 0b00000001
#define NBR_VALID_MASK 0b00000010
#define ACC_VALID_MASK 0b00000100

#define WEIGHT_MOV_AVG 0.05

#define UPDATE_BIAS_PARAMETER 0.01


struct pred_float_buffer
{
uint8_t size;
uint8_t head;
uint8_t tail;
float data[PRED_SIZE];  
};

struct angle_store
{
float ang;
float old_ang;
time_t time;
uint16_t time_diff;
};

volatile float GBL_angle_estimate[3] = {180.f, 180.f, 180.f};
volatile float GBL_vel_estimate[3] = {0.};

volatile float LAST_WEIGHT[3][3] = {{0.33}, {0.33}, {0.33}};

float local_bias[3] = {0.,0.,0.};
float neighbor_bias[3] = {0.,0.,0.};

struct angle_store angle_loc[3] = {{180., 180., 0., 1},
											{0., 0., 0., 1},
											{0., 0., 0., 1}};

struct angle_store angle_nbr[3] = {{180., 180., 0., 5},
											{0., 0., 0., 5},
											{0., 0., 0., 5}};
struct angle_state angle_grd[3] = {{180., 0., 0},
											{0., 0., 0},
											{0., 0., 0}};

struct angle_state angle_estimate[3] = {{180., 0., 0},
										{180., 0., 0},
										{180., 0., 0}};

struct pred_float_buffer pred_ring[3] = {{PRED_SIZE, 0, 0, {180.}},
											 {PRED_SIZE, 0, 0, {180.}},
											 {PRED_SIZE, 0, 0, {180.}}};



// ---------- internal function declarations ------------ //
void write_to_pred_float_buffer(struct pred_float_buffer*, float);
uint8_t check_measurement_validity(uint8_t, time_t);
void Sens_EDG_double_check_valid(uint8_t, uint8_t*, float[], float[]);
float min_three_flts(float, float, float);
float mean_valid_calculation(uint8_t, float[], uint8_t);
float stdev_calculation(float[], float, uint8_t, float);
void normalize_three(float[]);
void gaussian_weights(float[], float, float[], float, uint8_t);
float gaussian_calculation(float, float, float);
float clamp_f_const(float); 
float clamp_f_loc(float, float, float);
int8_t sgn_acc(int16_t);


// ------------------------------------------------------ //
// ------------------- Initialization ------------------- //
// ------------------------------------------------------ //
void Sens_EDG_initialize(void) {
	return;
}

// ------------------------------------------------------ //
// ------------------- Public Getters ------------------- //
// ------------------------------------------------------ // 
float Sens_EDG_get_angle_est(uint8_t edge) {
	return GBL_angle_estimate[edge];
}


float Sens_EDG_get_vel_est(uint8_t edge) {
	return GBL_vel_estimate[edge];
}


struct angle_state Sens_EDG_get_angle_state(uint8_t edge) {
	return angle_estimate[edge];
}

float Sens_EDG_get_weight(uint8_t edge, uint8_t wht_num) {
	return LAST_WEIGHT[edge][wht_num];
}

// ------------------------------------------------------ //
// --------------- Local Variable Updates --------------- //
// ------------------------------------------------------ // 
// Called in tmr1
void Sens_EDG_update_loc_ang(uint8_t edge, float angle, time_t time) {
	angle_loc[edge].old_ang = angle_loc[edge].ang;
	angle_loc[edge].ang = angle;
	angle_loc[edge].time_diff = (time-angle_loc[edge].time);
	angle_loc[edge].time = time;
	return;
}

// Called in Coms_123 once per T3 (20 Hz)
void Sens_EDG_update_nbr_ang(uint8_t edge, float angle, time_t time) {
	angle_nbr[edge].old_ang = angle_nbr[edge].ang;
	angle_nbr[edge].ang = angle;
	angle_nbr[edge].time_diff = (time-angle_nbr[edge].time);
	angle_nbr[edge].time = time;
	return;
}

// Called in Coms_Rel within T3
void Sens_EDG_update_grd_ang(uint8_t edge, float angle, float velocity, time_t time) {
	angle_grd[edge].ang = angle;
	angle_grd[edge].vel = velocity;
	angle_grd[edge].time = time;
	return;
}

// ------------------------------------------------------ //
// ----------------- Sensor Fusion Code ----------------- //
// ------------------------------------------------------ // 
void Sens_EDG_execute(uint8_t edge, time_t time) {
	// Coms_ESP_Verbose_One_Byte(69);
	// Coms_ESP_Verbose_One_Byte(69);
	
	uint8_t valid = check_measurement_validity(edge, time);
	// Coms_ESP_Verbose_One_Byte(valid);
//    valid &= 0b00000011;
	if (valid == 0) return;
//    valid = 1;

	
	float meas_angs[3] = {0., 0., 0.};
	float meas_vels[3] = {0., 0., 0.};
	float pred_ang = 180.;
	float pred_vel = 0.;
	float weights[3] = {0., 0., 0.};
	

//        Coms_ESP_Verbose_One_Byte(valid);
	Sens_EDG_prediction_step(edge, &pred_ang, &pred_vel);
	Sens_EDG_process_measurements(edge, time, meas_angs, meas_vels);
	
	Sens_EDG_double_check_valid(edge, &valid, meas_angs, meas_vels);
	
	Sens_EDG_generate_all_weights(edge, weights, meas_angs, meas_vels, pred_vel, valid);
	Sens_EDG_weights_moving_average(edge, weights);

	Sens_EDG_update_step(edge, meas_angs, meas_vels, pred_ang, pred_vel, valid, weights);
	Sens_EDG_update_bias(edge, meas_angs);

	angle_estimate[edge].ang = GBL_angle_estimate[edge];
	angle_estimate[edge].vel = GBL_vel_estimate[edge];
	angle_estimate[edge].time = time;
}


/**
 * Predicts what the measurement would be (shifts the measurement from their taken 
 * time to the time of the loop execution)
 * 
 * TODO(toni): this should be put as valid filter updates and not with the shifted measurements.
 * !Changed: velocity is now in the counter time units
 * Previously was Sens_EDG_get_measurements
 */
void Sens_EDG_process_measurements(uint8_t edge, time_t time, float angs[], float vels[]) {
	vels[0] = ((angle_loc[edge].ang-angle_loc[edge].old_ang)/(angle_loc[edge].time_diff));
    angs[0] = angle_loc[edge].ang + vels[0]*((float)(time - angle_loc[edge].time)) + local_bias[edge];   
    
	vels[1] = ((angle_nbr[edge].ang-angle_nbr[edge].old_ang)/(angle_nbr[edge].time_diff));
	angs[1] = angle_nbr[edge].ang + vels[1]*((float)(time - angle_nbr[edge].time)) + neighbor_bias[edge];   
    
	vels[2] = angle_grd[edge].vel;
	angs[2] = angle_grd[edge].ang + vels[2]*((float)(time - angle_grd[edge].time));
            
	return;
} 

/**
 * TODO: to keep this simple, maybe it would be good to do a linear regression on the 
 * differentiated series ((a_k - a_{k-1})/t), so that we assume the velocity to be the
 * linear one (constant acceleration).
 * 
 * TODO(toni): change vel estimate to be a state variable,
 * so that it is not computed everytime and it also is continuous in the graph
 * prediction using the previous 10 steps to compute velocity, and with this velocity 
 * updates the previous angle estimate. Should this be that closed? 
 * or should there be like an exponential moving average to compute velocity.
*/
void Sens_EDG_prediction_step(uint8_t edge, float *ang, float *vel) {
	//Initial Conditions
	static uint8_t counter[3] = {0};
	if (counter[edge] < PRED_SIZE) {
		counter[edge]++;
		*ang = GBL_angle_estimate[edge];
		*vel = GBL_vel_estimate[edge];
		return;
	}
    
	const float times[PRED_SIZE] = {-9., -8., -7., -6., -5., -4., -3., -2., -1., 0.};
	float pred[PRED_SIZE] = {0.};

	for(uint8_t i=0; i<PRED_SIZE; i++) {
		pred[i] = pred_ring[edge].data[(pred_ring[edge].head + i)%PRED_SIZE];
	}

	// precalculated, kept for clarity
	// sumx = 450; sumxsq = 28500; d = 82500;
	float sumy = 0;
	float sumxy = 0;

	for (uint8_t i=0; i<PRED_SIZE; i++) {
		sumy += pred[i];
		sumxy += times[i]*pred[i];
	}

	*vel = ((PRED_SIZE*sumxy+45.*sumy))*0.001212121212121212121;
	*ang = (sumy*285.+45.*sumxy)*0.001212121212121212;
	
    
	return;
} 


void Sens_EDG_update_step(uint8_t edge, float meas_angs[], float meas_vels[], float pred_ang, float pred_vel, uint8_t valid, float weights[]) {
	float ang = pred_ang;
	float vel = pred_vel;

	for(uint8_t i=0; i <3; i++) {
		if ((valid & (0b00000001 << i)) == false) continue;
		ang += 0.2*weights[i]*(meas_angs[i] - pred_ang);
		vel += 0.2*weights[i]*(meas_vels[i] - pred_vel);
	}
    
	GBL_angle_estimate[edge] = ang;
	GBL_vel_estimate[edge] = vel;

	write_to_pred_float_buffer(&pred_ring[edge], ang);

	return;
}

void Sens_EDG_update_bias(uint8_t edge, float meas_angs[]){
	local_bias[edge] -= UPDATE_BIAS_PARAMETER * (meas_angs[0] - GBL_angle_estimate[edge]);
	neighbor_bias[edge] -= UPDATE_BIAS_PARAMETER * (meas_angs[1] - GBL_angle_estimate[edge]);
	return;
}

void Sens_EDG_generate_all_weights(uint8_t edge, float weights[], float meas_angs[], float meas_vels[], float pred_vel, uint8_t valid) {
	float weight_ang[3] = {0.};
	float weight_vel[3] = {0.};
	float weight_mov[3] = {1., 1., 1.};

	Sens_EDG_weight_model_agreement(edge, weight_ang, meas_angs, valid, MIN_ANGLE_STDEV);
	Sens_EDG_weight_model_agreement(edge, weight_vel, meas_vels, valid, MIN_VEL_STDEV);
	Sens_EDG_weight_adjust_movement(weight_mov, pred_vel);

	for (uint8_t i=0; i <3; i++) {
		weights[i] = min_three_flts(weight_ang[i],  weight_vel[i],  weight_mov[i]);
	}
//    weights[0] = 0.33;
//    weights[1] = 0.33;
//    weights[2] = 0.33;
    
	normalize_three(weights);
	return;
}


void Sens_EDG_weight_model_agreement(uint8_t edge, float weights[], float x[], uint8_t valid, float min_stdev) {
	float mean = mean_valid_calculation(edge, x, valid);
	float stdev = stdev_calculation(x, mean, valid, min_stdev);
	gaussian_weights(weights, stdev, x, mean, valid);
	normalize_three(weights);
}


void Sens_EDG_weight_adjust_movement(float weights[], float pred_velocity){
	weights[2] -= fabs(pred_velocity) * MAX_ACC_TIME; // ! in the python code the time is not this but just the difference between the current and previous estimate one
	weights[2] = clamp_f_const(weights[2]);
	return;
}


void Sens_EDG_weights_moving_average(uint8_t edge, float weights[]) {
	weights[0] -= WEIGHT_MOV_AVG*(weights[0]-LAST_WEIGHT[edge][0]);
	weights[1] -= WEIGHT_MOV_AVG*(weights[1]-LAST_WEIGHT[edge][1]);
	weights[2] -= WEIGHT_MOV_AVG*(weights[2]-LAST_WEIGHT[edge][2]);

	LAST_WEIGHT[edge][0] = weights[0];
	LAST_WEIGHT[edge][1] = weights[1];
	LAST_WEIGHT[edge][2] = weights[2];
    return;
}


// ------------------------------------------------------ //
// ----------------- INTERNAL FUNCTIONS ----------------- //
// ------------------------------------------------------ // 

void write_to_pred_float_buffer(struct pred_float_buffer *buffer, float packet) {
	buffer->data[buffer->head] = packet;
	buffer->head = (buffer->head+1) % buffer->size;
};

/**
 * Check that changes in elapsed time and angle are small enough:
 *		- Time difference less than 100 ms
 *		- Angle difference less than 15 deg with previous estimate
 *	TODO: the difference with the angle should be according with the velocity?
 *	
 *	Args:
 *		edge (uint8_t): edge where the filter needs to be applied
 *		time (float): current time (only one time is saved in mori_module)
 *
 *	Returns:
 *		uint8_t: Only the last 3 bits are important, indicating if each measurement 
 *		is valid
 */
uint8_t check_measurement_validity(uint8_t edge, time_t time) {
	uint8_t valid = 0;
	// check for local
	valid |= ((time-angle_loc[edge].time) < 10); // && (abs(angle_loc[edge].ang - GBL_angle_estimate[edge]) < 15));
	//check for neighbor
	valid |= ((time-angle_nbr[edge].time) < 10) << 1; // && (abs(angle_nbr[edge].ang - GBL_angle_estimate[edge]) < 15)) << 1;
	// check for ground angle state
	valid |= ((time-angle_nbr[edge].time) < 10) << 2;
	return valid;
}


void Sens_EDG_double_check_valid(uint8_t edge, uint8_t *valid, float meas_angs[], float meas_vels[]) {
	for (uint8_t i=0; i<3;i++) {
		if (!(*valid & (0b00000001 << i))) continue;
		const bool good = (isnormal(meas_angs[i]) | isnormal(meas_vels[i]));
		if (!good) {
			*valid ^= 0b00000001 << i;
			meas_angs[i] = GBL_angle_estimate[edge]; //To override nans
			meas_vels[i] = GBL_vel_estimate[edge];
		}
	}

}


float min_three_flts(float a, float b, float c) {
	return (a <= b && a <= c) ? a : (b <= a && b <= c) ? b : c;
}


float mean_valid_calculation(uint8_t edge, float x[], uint8_t valid) {
	float mean = 0;
	float acum_weight = 0;
	for (uint8_t i=0; i<3; i++) {
		if ((valid & (0b00000001 << i)) == false) continue;
		mean += x[i]*LAST_WEIGHT[edge][i];
		acum_weight += LAST_WEIGHT[edge][i];
	}
	return mean / acum_weight;
}


float stdev_calculation(float x[], float mean, uint8_t valid, float min_value) {
	float stdev = 0;
	uint8_t count = 0;
	for (uint8_t i=0; i<3; i++) {
		if ((valid & (0b00000001 << i)) == false) continue;
		const float tmp_diff = (x[i] - mean);
		stdev += (tmp_diff * tmp_diff);
		count++;
	}

	stdev /= count; // biased estimator

	stdev = sqrt(stdev);
    
	return stdev > min_value ? stdev : min_value;
}


void normalize_three(float x[]) {
	float mean = x[0] + x[1] + x[2];
	if (mean < 1e-6) {
		x[0] = 0.33;
		x[1] = 0.33;
		x[2] = 1. - 0.33 - 0.33;
		return;
	}
	x[0] /= mean;
	x[1] /= mean;
	x[2] /= mean;
	return;
}


void gaussian_weights(float weights[], float stdev, float x[], float mean, uint8_t valid) {
	// if (stdev < 0.0000000001) return; //Impossible now
	if (valid & LOC_VALID_MASK) weights[0] = gaussian_calculation(stdev, x[0], mean);
	if (valid & NBR_VALID_MASK) weights[1] = gaussian_calculation(stdev, x[1], mean);
	if (valid & ACC_VALID_MASK) weights[2] = gaussian_calculation(stdev, x[2], mean);
	return;
}


float gaussian_calculation(float stdev, float x, float mean) {
	const float diff = (x-mean); // /stdev
	return exp(-((diff*diff)/(stdev*stdev))*0.5);
	// const float x2 = diff*diff;
	// const float x4 = x2 * x2;
	// return 1/(1+x2*0.5 + x4*0.125);
}


/* https://stackoverflow.com/questions/427477/fastest-way-to-clamp-a-real-fixed-floating-point-value */
#define CLAMP_MIN 0.1
#define CLAMP_MAX 1
float clamp_f_const(float d) {
  const float t = d < CLAMP_MIN ? CLAMP_MIN : d;
  return t > CLAMP_MAX ? CLAMP_MAX : t;
}

/* https://stackoverflow.com/questions/427477/fastest-way-to-clamp-a-real-fixed-floating-point-value */
float clamp_f_loc(float d, float min, float max) {
  const float t = d < min ? min : d;
  return t > max ? max : t;
}


int8_t sgn_acc(int16_t value) {
    return (value > 0) - (value < 0);
}