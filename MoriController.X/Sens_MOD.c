// Author: Antoni Jubes
// ! maybe should be renamed to Sens_FUS?

#include "Sens_MOD.h"

#include <stdint.h>
#include <math.h>

#include "Sens_EDG.h"
#include "Sens_GRD.h"
#include "Util_TIM.h"
#include "Coms_ESP.h"

bool is_connected[3] = {false, false, false};
float ground_angle_calc[3] = {180., 180., 180.};
float FUS_LiveOffset[3] = {0., 0., 0.};
float FUS_LrgOffsetMult[3] = {1., 1., 1.};


// Local function declarations
float clamp_f_again(float, float, float);

// is there no initialization on Sens_EDG ?
void Sens_MOD_initialize(void) {
  Sens_EDG_initialize();
}

void Sens_MOD_update_connections(volatile bool connections[3]) {
  is_connected[0] = connections[0];
  is_connected[1] = connections[1];
  is_connected[2] = connections[2];
  Sens_GRD_update_connections(is_connected);
}

void Sens_MOD_update_local_angle(uint8_t edge, float angle) {
  Sens_EDG_update_loc_ang(edge, angle, Util_TIM_get_time()); //need to update with time
}

void Sens_MOD_update_neighbor_angle(uint8_t edge, float angle) {
  Sens_EDG_update_nbr_ang(edge, angle, Util_TIM_get_time()); //need to update with time
}


void Sens_MOD_add_ground_measurement(uint16_t x, uint16_t y, uint16_t z) {
  Sens_GRD_add_measurement(x, y, z, Util_TIM_get_time());
}

void Sens_MOD_update_neighbor_state(uint8_t edge,
                                    struct ground_state neighbor_state) {
  Sens_GRD_update_neighbor_state(edge, neighbor_state);
}


struct ground_state Sens_MOD_get_ground_state_edge_frame(uint8_t edge) {
  return Sens_GRD_get_ground_state_edge_frame(edge);
}


void Sens_MOD_iterate_filter(void) {
  time_t current_time = Util_TIM_get_time();

  for (uint8_t edge = 0; edge < 3; edge++) {
    if (is_connected[edge]) {
      Sens_EDG_execute(edge, current_time);
      Sens_GRD_update_angle_state(edge, Sens_EDG_get_angle_state(edge));
    }
  }

  Sens_GRD_iterate_filter(current_time);
  Sens_GRD_purge_measurements();

  for (uint8_t edge = 0; edge < 3; edge++) {
    if (is_connected[edge]) {
      struct angle_state grd_angle_state =
          Sens_GRD_compute_angle(edge, current_time);
      ground_angle_calc[edge] = grd_angle_state.ang;
      Sens_EDG_update_grd_ang(edge, grd_angle_state.ang, grd_angle_state.vel,
                              grd_angle_state.time);
    }
  }
}

float Sens_MOD_get_ground_angle(uint8_t edge) {
    return ground_angle_calc[edge];
}

/* ******************** RETURN FORMATTED ANGLE ****************************** */
uint16_t Sens_MOD_GetAngleFormatted(uint8_t edge, bool withLiveOffset) {
    return (uint16_t)(10.0f * Sens_MOD_GetAngle(edge, withLiveOffset) + 1800.f); 
}

/* ******************** GET CURRENT ANGLE *********************************** */
float Sens_MOD_GetAngle(uint8_t edge, bool WithLiveOffset) {
    if (WithLiveOffset)
        return (float) Sens_EDG_get_angle_est(edge) - FUS_LiveOffset[edge] - 180.f;
    return (float) Sens_EDG_get_angle_est(edge) - 180.f;
}

/* ******************** GET ANGLE DELTA ************************************* */
float Sens_MOD_GetDelta(uint8_t edge) {
    return Sens_EDG_get_vel_est(edge);
}

void Sens_MOD_SetLiveOffset(uint8_t edge, uint16_t nbrFusVal) {
    FUS_LiveOffset[edge] = (((float)Sens_MOD_GetAngleFormatted(edge, false)) 
            - ((float)nbrFusVal)) * 0.05f; // *0.5 to divide by two, *0.1 to translate to degrees

    
    // if angle offset between neightbours bigger than 5 deg,
    // scale output PWM down (zero at 30 deg diff)
    if (fabsf(FUS_LiveOffset[edge]) < 5.0f){ 
        FUS_LrgOffsetMult[edge] = 1.0f;
        return;
    } 
    FUS_LrgOffsetMult[edge] = 1.0f - (fabsf(FUS_LiveOffset[edge]) - 5.0f) * 0.1f;
    FUS_LrgOffsetMult[edge] = clamp_f_again(FUS_LrgOffsetMult[edge], 0.0f, 1.0f);
    return;
}


float Sens_MOD_GetLrgOffsetMult(uint8_t edge){
    return FUS_LrgOffsetMult[edge];
}

//Can eventually remove - all "helper" functions should go into a separate file
/* https://stackoverflow.com/questions/427477/fastest-way-to-clamp-a-real-fixed-floating-point-value */
float clamp_f_again(float d, float min, float max) {
  const float t = d < min ? min : d;
  return t > max ? max : t;
}