#ifndef Sens_FUS_H
#define	Sens_FUS_H

void Sens_FUS_increment_timers(void);

float Sens_FUS_get_angle_est(uint8_t);
float Sens_FUS_get_weight(uint8_t, uint8_t);

void Sens_FUS_update_loc_enc(uint8_t, float);
void Sens_FUS_update_nbr_enc(uint8_t, float);
void Sens_FUS_update_loc_acc(void);
void Sens_FUS_update_nbr_acc(uint8_t, uint16_t, uint16_t, uint16_t, uint8_t, float);
float Sens_FUS_angle_to_ground(uint16_t, uint16_t, uint16_t);

void Sens_FUS_execute(void);
void Sens_FUS_update_loc_acc(void);
void Sens_FUS_get_measurements(uint8_t, float[], float[]); 
void Sens_FUS_prediction_step(uint8_t, float[], float[]); 
void Sens_FUS_generate_all_weights(uint8_t, float[], float[], float[], float, uint8_t);

void Sens_FUS_weight_model_agreement(uint8_t, float[], float[], uint8_t, float);
void Sens_FUS_update_step(uint8_t, float[], float[], float, float, uint8_t, float[]);
void Sens_FUS_weights_moving_average(uint8_t, float[]);
void Sens_FUS_weight_adjust_movement(float[], float);

#endif	/* Sens_FUS_H */


