//Note: only currently valid for one edge. Will need to expand to all edges after testing.
//#include "mcc_generated_files/mcc.h"
#include "Defs_GLB.h"
#include "Sens_FUS.h"
#include "Sens_ACC.h"
#include "Sens_ENC.h"
#include "Coms_ESP.h"
#include "Mnge_RGB.h"
#include "math.h"

#define MAX_TIME 0xFFFF

#define INV_PI_x180 57.2957795 // Convert to radians
#define ERROR_INDEX 254
#define BYTE_DELAY 0.05 // To be updated according to packet size

#define MIN_ANGLE_STDEV 1
#define MIN_VEL_STDEV 0.1

#define LOC 0
#define NBR 1
#define ACC 2

#define X_AXIS 0
#define Y_AXIS 1
#define Z_AXIS 2

#define PRED_SIZE 10 //Size of prediction regression
#define ACC_SIZE 30 //Size of internal buffers
#define LOOP_TIME 10 //100 Hz = 10 ms, update as needed
#define MAX_ACC_TIME 30 //Time to reject ACC measurments
#define INV_MAX_ACC_TIME_x1000 3.333333 // == 1000 / MAX_ACC_TIME

#define LOC_VALID_MASK 0b00000001
#define NBR_VALID_MASK 0b00000010
#define ACC_VALID_MASK 0b00000100

#define SQRT_TWO_PI 2.506628274631

#define WEIGHT_MOV_AVG 0.05
#define WEIGHT_ONE_MINUS_AVG 0.95

//#define WEIGHT_MOV_AVG 0.2
//#define WEIGHT_ONE_MINUS_AVG 0.8

struct accel_buffer
{
uint8_t size;
uint8_t head;
uint8_t tail;
uint16_t data[ACC_SIZE];  
};

struct time_buffer
{
uint8_t size;
uint8_t head;
uint8_t tail;
uint32_t data[ACC_SIZE];  
};



struct float_buffer
{
uint8_t size;
uint8_t head;
uint8_t tail;
float data[ACC_SIZE];  
};


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
uint32_t time;
uint16_t time_diff;
};


//time_acc will be a counter of 10 ms intervals
//if time = last_time - time
//Note: Assume mori never runs beyond time_counter
volatile uint32_t time_counter = 0;

volatile float GBL_angle_estimate[3] = {180.f, 180.f, 180.f};
volatile float GBL_vel_estimate[3] = {0.};

volatile float LAST_WEIGHT[3][3] = {{0.33}, {0.33}, {0.33}};

struct angle_store angle_loc[3] = {{180., 180., 0., 1},
											{0., 0., 0., 1},
											{0., 0., 0., 1}};
struct angle_store angle_nbr[3] = {{180., 180., 0., 5},
											{0., 0., 0., 5},
											{0., 0., 0., 5}};

struct accel_buffer accel_ring_X = {ACC_SIZE, 0, 0, {0}};
struct accel_buffer accel_ring_Y = {ACC_SIZE, 0, 0, {0}};
struct accel_buffer accel_ring_Z = {ACC_SIZE, 0, 0, {0}};
struct time_buffer accel_times = {ACC_SIZE, 0, 0, {1000}};

struct float_buffer orient_ring[3] =  {{ACC_SIZE, 0, 0, {0.}},
												{ACC_SIZE, 0, 0, {0.}},
												{ACC_SIZE, 0, 0, {0.}}};
struct time_buffer orient_time[3] =  {{ACC_SIZE, 0, 0, {1000}},
												{ACC_SIZE, 0, 0, {1000}},
												{ACC_SIZE, 0, 0, {1000}}};

struct pred_float_buffer pred_ring[3] = {{PRED_SIZE, 0, 0, {180.}},
											 {PRED_SIZE, 0, 0, {180.}},
											 {PRED_SIZE, 0, 0, {180.}}};



// ---------- internal function declarations ------------ //
void write_to_buffer(struct accel_buffer*, uint16_t);
void write_to_float_buffer(struct float_buffer*, float);
void write_to_pred_float_buffer(struct pred_float_buffer*, float);
void write_to_time_buffer(struct time_buffer*, float);
volatile uint16_t get_time_diff(uint32_t);
uint8_t check_valid(uint8_t);
void Sens_FUS_double_check_valid(uint8_t, uint8_t*, float[], float[]);
void get_acc_pred(float*, float*, struct time_buffer, struct float_buffer);
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
// ------------------- Public Getters ------------------- //
// ------------------------------------------------------ // 
float Sens_FUS_get_angle_est(uint8_t edge) {
	return GBL_angle_estimate[edge];
}

float Sens_FUS_get_weight(uint8_t edge, uint8_t wht_num) {
	return LAST_WEIGHT[edge][wht_num];
}

// ------------------------------------------------------ //
// --------------- Local Variable Updates --------------- //
// ------------------------------------------------------ // 
// Called in tmr1
void Sens_FUS_update_loc_enc(uint8_t edge, float angle) {
	angle_loc[edge].old_ang = angle_loc[edge].ang;
	angle_loc[edge].ang = angle;
	angle_loc[edge].time_diff = get_time_diff(angle_loc[edge].time);
	angle_loc[edge].time = time_counter;
	return;
}

// Called in Coms_123 once per T3 (20 Hz)
void Sens_FUS_update_nbr_enc(uint8_t edge, float angle) {
	angle_nbr[edge].old_ang = angle_nbr[edge].ang;
	angle_nbr[edge].ang = angle;
	angle_nbr[edge].time_diff = get_time_diff(angle_nbr[edge].time);
	angle_nbr[edge].time = time_counter;
	return;
}

// Called in Coms_Rel within T3
void Sens_FUS_update_nbr_acc(uint8_t edge, uint16_t acc1, uint16_t acc2, 
							 uint16_t acc3, uint8_t dist, float accum_angle) {
	    
	if ((acc1 + acc2 + acc3) == 0) return;

	// Our model doesn't hold if the Mori is vertical - there's no relative angle to ground
//	uint16_t z_diff_nbr = 4095-acc3;
//	if ((1950 < z_diff_nbr) && (z_diff_nbr < 2150)) return;

	// Coms_ESP_Verbose_One_Byte(acc1 / 10);
	// Coms_ESP_Verbose_One_Byte(acc2 / 10);
	// Coms_ESP_Verbose_One_Byte(acc3 / 10);


	uint8_t delay_idx;
	uint16_t time_tmp;
	float sensor_prop_delay = 0;
	if (dist > 0) {
		sensor_prop_delay = BYTE_DELAY * dist; //Constant should be divided by 10
		delay_idx = ERROR_INDEX; // arbitrarily large
		for (uint8_t i=0; i< ACC_SIZE; i++) {
			uint8_t idx = (accel_times.head - i -1)%ACC_SIZE;
			time_tmp = get_time_diff(accel_times.data[idx]);
			if (time_tmp >= sensor_prop_delay) {
				delay_idx = idx;
				break;
			}
		}

		if (delay_idx == ERROR_INDEX) {
			return;
		}
	} else {
		delay_idx = (accel_times.head -1)%ACC_SIZE; //Could be here!!!!!!
		// time_tmp = get_time_diff(accel_times.data[delay_idx]);
	}
    
	if ((accel_ring_X.data[delay_idx] + accel_ring_Y.data[delay_idx] + accel_ring_Z.data[delay_idx]) == 0) {
		return;
	}
    
    //Model does not work if Mori is vertical
//	uint16_t z_diff_loc = 4095-accel_ring_Z.data[delay_idx];
//	if ((1950 < z_diff_loc) && (z_diff_loc < 2150)) return;

	//For testing, I can set accel_ring_X-Z.data as constants
	const float ang_loc = Sens_FUS_angle_to_ground(accel_ring_X.data[delay_idx],
		accel_ring_Y.data[delay_idx], accel_ring_Z.data[delay_idx]);
	const float ang_far = Sens_FUS_angle_to_ground(acc1, acc2, acc3);
    
	// Coms_ESP_Verbose_One_Byte(accel_ring_X.data[delay_idx] / 10);
	// Coms_ESP_Verbose_One_Byte(accel_ring_Y.data[delay_idx] / 10);
	// Coms_ESP_Verbose_One_Byte(accel_ring_Z.data[delay_idx] / 10);

    // Need to implement new process here to compensate if one module is flat
	const int8_t sign = sgn_acc((int16_t)accel_ring_Z.data[delay_idx]-2047);
    
	// requires accum_angle to be set to 0 if direct from neighbour
	// Note: 180 subtracted from accum_angle on transmission
    float angle;
	if (sign >=0) {
		angle = 180 - abs(ang_loc - ang_far) + accum_angle;		
	} else {
		angle = 180 + abs(ang_loc - ang_far) + accum_angle;
	}

	if ((angle > 300.) || (angle < 60.)) return;

//	Coms_ESP_Verbose_One_Byte((uint8_t)(accel_ring_Z.data[delay_idx]/10));
    
    // Get jumps in angles when the table or Mori are jostled
    if (abs(angle-GBL_angle_estimate[edge]) > 10.) return;
    //We do get weird angles here 
    //Not accum_angle (fairly confident)
    // accel_ring_Z[delay idx] hits 187 sometimes! 103 normally
    // Jumps when moving quickly!!! E.g., landing
	write_to_float_buffer(&orient_ring[edge], angle);
	write_to_time_buffer(&orient_time[edge], accel_times.data[delay_idx]-sensor_prop_delay);
	return;
}


float Sens_FUS_angle_to_ground(uint16_t accX, uint16_t accY, uint16_t accZ) {
	//Note - does not work when horizontal. Possible to fix?    
	const float X = ((float)accX) - 2047.f;
	const float Y = ((float)accY) - 2047.f;
	const float Z = ((float)accZ) - 2047.f;
	const float numer = Z * 1024.f;
	const float denom = sqrtf(X * X + Y * Y + Z * Z) * 1024.f;

	float angle = numer / denom;
	angle = acosf(angle);    
	angle = -angle * INV_PI_x180;
	return angle;
}



// ------------------------------------------------------ //
// ----------------- Sensor Fusion Code ----------------- //
// ------------------------------------------------------ // 
void Sens_FUS_execute(void) {
	// Sens_FUS_update_loc_acc();
	for (uint8_t edge=0; edge < 3; edge ++) {
		if (!Flg_EdgeSyn[edge]) continue;

        // Coms_ESP_Verbose_One_Byte(69);
        // Coms_ESP_Verbose_One_Byte(69);
		
		uint8_t valid = check_valid(edge);
        // Coms_ESP_Verbose_One_Byte(valid);
		if (valid == 0) continue;

		
		float meas_angs[3] = {0., 0., 0.};
		float meas_vels[3] = {0., 0., 0.};
		float pred_ang = 180.;
		float pred_vel = 0.;
		float weights[3] = {0., 0., 0.};
		

//        Coms_ESP_Verbose_One_Byte(valid);
		Sens_FUS_get_measurements(edge, meas_angs, meas_vels);
		Sens_FUS_prediction_step(edge, &pred_ang, &pred_vel);
        
        // Coms_ESP_Verbose_One_Byte(angle_loc[edge].ang);
        // Coms_ESP_Verbose_One_Byte(angle_nbr[edge].ang);
         // Coms_ESP_Verbose_Float(meas_vels[0]);
         // Coms_ESP_Verbose_Float(meas_vels[1]);
         // Coms_ESP_Verbose_Float(meas_vels[2]);
         // Coms_ESP_Verbose_One_Byte(pred_ang);
//        Coms_ESP_Verbose_One_Byte((pred_vel+128)*10);
        // Coms_ESP_Verbose_Float(pred_vel);

		Sens_FUS_double_check_valid(edge, &valid, meas_angs, meas_vels);
        
        
		Sens_FUS_generate_all_weights(edge, weights, meas_angs, meas_vels, pred_vel, valid);
		Sens_FUS_weights_moving_average(edge, weights);
        // Coms_ESP_Verbose_One_Byte(weights[0]*100);
        // Coms_ESP_Verbose_One_Byte(weights[1]*100);
        // Coms_ESP_Verbose_One_Byte(weights[2]*100);
		
		Sens_FUS_update_step(edge, meas_angs, meas_vels, pred_ang, pred_vel, valid, weights);
        // Coms_ESP_Verbose_Float(GBL_angle_estimate[edge]);
        // Coms_ESP_Verbose_Float(GBL_vel_estimate[edge]);
    }

	// Sens_FUS_increment_timers();
}



void Sens_FUS_get_measurements(uint8_t edge, float angs[], float vels[]) {
	vels[0] = ((angle_loc[edge].ang-angle_loc[edge].old_ang)/(angle_loc[edge].time_diff))*100.;  // 0.01 = 10 ms / 1000 ms = 0.01 s
    angs[0] = angle_loc[edge].ang + vels[0]*((float)get_time_diff(angle_loc[edge].time)*0.01);
    
	vels[1] = ((angle_nbr[edge].ang-angle_nbr[edge].old_ang)/(angle_nbr[edge].time_diff))*100.;  // 0.01 = 10 ms / 1000 ms = 0.01 s
	angs[1] = angle_nbr[edge].ang + vels[1]*((float)get_time_diff(angle_nbr[edge].time)*0.01);

	static uint8_t count = 0; //Wait to fill ACC buffer
	if (count < ACC_SIZE) {
		count++;
		return;
	}
	get_acc_pred(&angs[2],&vels[2], orient_time[edge], orient_ring[edge]);
	vels[2] *= 100.;
            
	return;
} 


void Sens_FUS_prediction_step(uint8_t edge, float *ang, float *vel) {
	//Initial Conditions
	static uint8_t counter[3] = {0};
	if (counter[edge] < PRED_SIZE) {
		counter[edge]++;
		*ang = GBL_angle_estimate[edge];
		*vel = GBL_vel_estimate[edge];
		// *vel = 0.;
		return;
	}
    
	// const float x[PRED_SIZE] = {0, 10, 20, 30, 40, 50, 60, 70, 80, 90};
	const float x[PRED_SIZE] = {90., 80., 70., 60., 50., 40., 30., 20., 10., 0.};
	// const float x[PRED_SIZE] = {0.0, 0.010, 0.020, 0.030, 0.040, 0.050, 0.060, 0.070, 0.080, 0.090};
	float y[PRED_SIZE] = {0.};

	for(uint8_t i=0; i<PRED_SIZE; i++) {
		y[i] = pred_ring[edge].data[(pred_ring[edge].head + i)%PRED_SIZE];
		// Coms_ESP_Verbose_One_Byte(y[i]);
	}

	//precalculated, kept for clarity
	// sumx = 450; sumxsq = 28500; d = 82500;
	float sumy = 0;
	float sumxy = 0;

	for (uint8_t i=0; i<PRED_SIZE; i++) {
		sumy += y[i];
		sumxy += x[i]*y[i];
	}


	// *vel = -((PRED_SIZE*sumxy-450.*sumy))/82500.;
	// *ang = (sumy*28500.-450.*sumxy)/82500.;	
	*vel = -((PRED_SIZE*sumxy-450.*sumy))*0.01212121212121212121; // also multiplied by 1000 to convert to seconds
	*ang = (sumy*28500.-450.*sumxy)*0.00001212121212121212;	
	// *vel = -((PRED_SIZE*sumxy-0.450*sumy))/0.0825;
	// *ang = (sumy*0.0285-0.450*sumxy)/0.0825;	
	return;
} 


void Sens_FUS_update_step(uint8_t edge, float meas_angs[], float meas_vels[], float pred_ang, float pred_vel, uint8_t valid, float weights[]) {
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


void Sens_FUS_generate_all_weights(uint8_t edge, float weights[], float meas_angs[], float meas_vels[], float pred_vel, uint8_t valid) {
	float weight_ang[3] = {0.};
	float weight_vel[3] = {0.};
	float weight_mov[3] = {1., 1., 1.};

	Sens_FUS_weight_model_agreement(edge, weight_ang, meas_angs, valid, MIN_ANGLE_STDEV);
	Sens_FUS_weight_model_agreement(edge, weight_vel, meas_vels, valid, MIN_VEL_STDEV);
	Sens_FUS_weight_adjust_movement(weight_mov, pred_vel);

	for (uint8_t i=0; i <3; i++) {
		weights[i] = min_three_flts(weight_ang[i],  weight_vel[i],  weight_mov[i]);
	}
//    weights[0] = 0.33;
//    weights[1] = 0.33;
//    weights[2] = 0.33;
    
	normalize_three(weights);
	return;
}


void Sens_FUS_weight_model_agreement(uint8_t edge, float weights[], float x[], uint8_t valid, float min_stdev) {
	float mean = mean_valid_calculation(edge, x, valid);
	float stdev = stdev_calculation(x, mean, valid, min_stdev);
	gaussian_weights(weights, stdev, x, mean, valid);
	normalize_three(weights);
}


void Sens_FUS_weight_adjust_movement(float weights[], float pred){
	weights[2] -= abs(pred) * MAX_ACC_TIME * 10;
	weights[2] = clamp_f_const(weights[2]);
	return;
}


void Sens_FUS_weights_moving_average(uint8_t edge, float weights[]) {
	weights[0] = WEIGHT_ONE_MINUS_AVG*LAST_WEIGHT[edge][0] + WEIGHT_MOV_AVG*weights[0];
	weights[1] = WEIGHT_ONE_MINUS_AVG*LAST_WEIGHT[edge][1] + WEIGHT_MOV_AVG*weights[1];
	weights[2] = WEIGHT_ONE_MINUS_AVG*LAST_WEIGHT[edge][2] + WEIGHT_MOV_AVG*weights[2];

	LAST_WEIGHT[edge][0] = weights[0];
	LAST_WEIGHT[edge][1] = weights[1];
	LAST_WEIGHT[edge][2] = weights[2];
    return;
}



// ------------------------------------------------------ //
// --------------- T1 Specific functions ---------------- //
// ------------------------------------------------------ // 
// Called once every T1 (at 100 Hz)
void Sens_FUS_increment_timers(void) {
	time_counter++;
}


// Called once every T1 (at 100 Hz)
void Sens_FUS_update_loc_acc() {
	write_to_buffer(&accel_ring_X, Sens_ACC_GetRaw(X_AXIS));
	write_to_buffer(&accel_ring_Y, Sens_ACC_GetRaw(Y_AXIS));
	write_to_buffer(&accel_ring_Z, Sens_ACC_GetRaw(Z_AXIS));
	write_to_time_buffer(&accel_times, time_counter);
}


// ------------------------------------------------------ //
// ----------------- INTERNAL FUNCTIONS ----------------- //
// ------------------------------------------------------ // 
void write_to_buffer(struct accel_buffer *buffer, uint16_t packet) {
	buffer->data[buffer->head] = packet;
	buffer->head = (buffer->head+1) % buffer->size;
};



void write_to_float_buffer(struct float_buffer *buffer, float packet) {
	buffer->data[buffer->head] = packet;
	buffer->head = (buffer->head+1) % buffer->size;
};


void write_to_pred_float_buffer(struct pred_float_buffer *buffer, float packet) {
	buffer->data[buffer->head] = packet;
	buffer->head = (buffer->head+1) % buffer->size;
};


void write_to_time_buffer(struct time_buffer *buffer, float packet) {
	buffer->data[buffer->head] = packet;
	buffer->head = (buffer->head+1) % buffer->size;
};


volatile uint16_t get_time_diff(uint32_t time){
	return (uint16_t)((MAX_TIME + time_counter - time)%MAX_TIME);
}


uint8_t check_valid(uint8_t edge) {
	uint8_t valid = 0;
	valid |= ((get_time_diff(angle_loc[edge].time) < 10)); // && (abs(angle_loc[edge].ang - GBL_angle_estimate[edge]) < 15));
	valid |= ((get_time_diff(angle_nbr[edge].time) < 10)) << 1; // && (abs(angle_nbr[edge].ang - GBL_angle_estimate[edge]) < 15)) << 1;
	
    uint16_t tmp_max = 0; //Check to see if last accel from neighbour greater than 200 ms
	for (uint8_t i=0; i < orient_time[edge].size; i++) {
		if (orient_time[edge].data[i] > tmp_max) {
			tmp_max = orient_time[edge].data[i];
		}
	}
    // Also check if last accel from self greater than 200 ms (doesn't matter which is greater)
	for (uint8_t i=0; i < accel_times.size; i++) {
		if (accel_times.data[i] > tmp_max) {
			tmp_max = accel_times.data[i];
		}
    }

	valid |= ((time_counter - tmp_max < 20)) << 2; // checks value, then moves 0b00000001 to 0b00000100
	return valid;
}


void Sens_FUS_double_check_valid(uint8_t edge, uint8_t *valid, float meas_angs[], float meas_vels[]) {
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


// Separated linear_regression between this and the prediction step for optimization
void get_acc_pred(float *ang, float *vel, struct time_buffer times, struct float_buffer accel) {	
	// code for linear regression
	float sumx = 0;
	float sumxsq = 0;
	float sumy = 0;
	float sumxy = 0;
	const float last_time = (float)times.data[(ACC_SIZE+times.head-1)%ACC_SIZE];

	float mean = 0.;
	for (uint8_t i=0; i<ACC_SIZE; i++) 
		mean += accel.data[i];
	mean /= ACC_SIZE;

	uint8_t n = ACC_SIZE;
	for (uint8_t i=0; i<ACC_SIZE; i++) {
		const float x = (float)times.data[i] - last_time;
		if (((-x) >= MAX_ACC_TIME) || (!isnormal(accel.data[i])) || (accel.data[i] < 60.) || (accel.data[i] > 300.)) {//} || (abs(accel.data[i]- mean) > 3.)) {
			n -= 1;
			continue;
		}
//        Coms_ESP_Verbose_One_Byte(-x);
		sumx += x;
		sumxsq += x*x;
		sumy += accel.data[i];
		sumxy += x*accel.data[i];
	}

	if (n == 0) return;

	const float d = n*sumxsq - sumx*sumx;
	*vel = ((n*sumxy-sumx*sumy))/d;
	*ang = (sumy*sumxsq-sumx*sumxy)/d;
	return;
}


float min_three_flts(float a, float b, float c) {
	return (a <= b && a <= c) ? a : (b <= a && b <= c) ? b : c;
}


float mean_valid_calculation(uint8_t edge, float x[], uint8_t valid) {
	float mean = 0;
	float count = 0;
	for (uint8_t i=0; i<3; i++) {
		if ((valid & (0b00000001 << i)) == false) continue;
		mean += x[i]*LAST_WEIGHT[edge][i];
		count++;
	}
	return mean / count;
}


float stdev_calculation(float x[], float mean, uint8_t valid, float min_value) {
	float tmp[3] = {0.};
	uint8_t count = 0;
	for (uint8_t i=0; i<3; i++) {
		if ((valid & (0b00000001 << i)) == false) continue;
		const float tmp_diff = (x[i] - mean);
		tmp[count] = (tmp_diff * tmp_diff);
		count++;
	}

	float stdev = 0;
	for (uint8_t i=0; i<count; i++) 
		stdev += tmp[i];
	stdev /= count;

	stdev = sqrt(stdev);
    
	return stdev > min_value ? stdev : min_value;
}


void normalize_three(float x[]) {
	float mean = x[0] + x[1] + x[2];
	if (mean == 0) {
		x[0] = 0.33;
		x[1] = 0.33;
		x[2] = 0.33;
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
	const float diff = (x-mean);
	return (1/(stdev*SQRT_TWO_PI))*exp(-((diff*diff)/(stdev*stdev))*0.5);
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