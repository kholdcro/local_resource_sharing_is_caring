#ifndef Sens_EDG_H
#define Sens_EDG_H

#include <stdint.h>
#include <stdbool.h>
#include "Util_TIM.h"
#include "Util_ANG.h"

void Sens_EDG_initialize(void);

float Sens_EDG_get_angle_est(uint8_t edge);
float Sens_EDG_get_vel_est(uint8_t edge);
struct angle_state Sens_EDG_get_angle_state(uint8_t edge);
float Sens_EDG_get_weight(uint8_t edge, uint8_t wht_num);

void Sens_EDG_update_loc_ang(uint8_t edge, float angle, time_t time);
void Sens_EDG_update_nbr_ang(uint8_t edge, float angle, time_t time);
void Sens_EDG_update_grd_ang(uint8_t edge, float angle, float velocity, time_t time);

void Sens_EDG_execute(uint8_t edge, time_t time);
void Sens_EDG_process_measurements(uint8_t edge, time_t time, float angs[], float vels[]);
void Sens_EDG_prediction_step(uint8_t edge, float *ang, float *vel);
void Sens_EDG_generate_all_weights(uint8_t edge, float weights[],
                                   float meas_angs[], float meas_vels[],
                                   float pred_vel, uint8_t valid);

void Sens_EDG_weight_model_agreement(uint8_t edge, float weights[], float x[],
                                     uint8_t valid, float min_stdev);
void Sens_EDG_update_step(uint8_t edge, float measured_angles[],
                          float measured_vels[], float predicted_angle,
                          float predicted_vel, uint8_t valid, float weights[]);
void Sens_EDG_update_bias(uint8_t edge, float measured_angles[]);
void Sens_EDG_weights_moving_average(uint8_t edge, float weights[]);
void Sens_EDG_weight_adjust_movement(float weights[], float pred_velocity);

#endif /* Sens_EDG_H */
