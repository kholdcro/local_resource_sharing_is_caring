// Author: Antoni Jubes
// Ground filter for the Mori
#ifndef Sens_GRD_H
#define Sens_GRD_H

#include <stdbool.h>
#include <stdint.h>

#include "Util_ANG.h"
#include "Util_MAT.h"
#include "Util_TIM.h"

/**
 * @brief The ground state structure contains the ground vector and the velocity
 * of the ground vector and the time when this was estimated.
 *
 * The ground vector is the vector that points from the center of the Mori to
 * the ground (gravity vector), and it is expressed in the Mori frame.
 *
 * The velocity is the velocity of the ground vector, and it is expressed in the
 * Mori frame.
 */
struct ground_state {
  struct vector_3d ground;
  struct vector_3d velocity;
  time_t time;
};

/**
 * @brief Initialization of the ground filter (should be called once at the
 * beginning)
 * @param connections array of booleans indicating whether a neighbor is
 * connected
 */

void Sens_GRD_update_connections(bool connections[]);

/**
 * @brief Add a new measurement to the ground filter.
 * @param x x component of the measurement
 * @param y y component of the measurement
 * @param z z component of the measurement
 * @param time time of the measurement
 */
void Sens_GRD_add_measurement(int16_t x, int16_t y, int16_t z, time_t time);
/**
 * Delete the stored measurements (should be called everytime the filter is
 * iterated so as not to reuse old measurements).
 */
void Sens_GRD_purge_measurements(void);

/**
 * @brief Update the ground filter with an updated angle state (angle, speed and
 * time) for a given edge. This function should be called for each edge after
 * the local angle filter.
 * @param edge edge for which the angle state is updated
 * @param new_state new angle state
 */
void Sens_GRD_update_angle_state(uint8_t edge, struct angle_state new_state);
/**
 * Update the ground filter with an updated neighbor ground state estimate
 * (ground, velocity and time of estimation).
 */
void Sens_GRD_update_neighbor_state(uint8_t edge,
                                    struct ground_state neighbor_state);

/**
 * @brief Get a copy of the current ground estimate of the ground filter.
 * @return ground estimate
 */
struct ground_state Sens_GRD_get_ground_estimate(void);
/**
 * @brief Get a copy of the current ground estimate of the ground filter
 * expressed in the reference frame of a given edge.
 * @param edge edge for which the ground estimate is expressed
 * @return ground estimate
 */
struct ground_state Sens_GRD_get_ground_state_edge_frame(uint8_t edge);

/**
 * @brief Compute an estimate of the angle state (angle, speed and time) for a
 * given edge and time according to the ground filter estimated variables.
 * @param edge edge for which the angle state is computed
 * @param time time at which the angle state is computed
 * @return angle state
 */
struct angle_state Sens_GRD_compute_angle(uint8_t edge, time_t time);

/**
 * @brief Perform a single iteration of the ground filter with the new
 * measurements, the new updated angles, and the possibly updated neighbor
 * ground states.
 *
 * If the neighbor ground states are not updated, the ground filter will not use
 * the neighbor angle factors.
 *
 * If no new measurements are available, no time has passed and the neighbors
 * are not updated either, the ground filter iteration will be an empty
 * iteration.
 *
 * @param time time at which the iteration is performed
 */
void Sens_GRD_iterate_filter(time_t);

bool Sens_GND_Check_NAN(struct vector_4d);

float Sens_GRD_Norm_Measurment(int16_t, int16_t, int16_t);

#endif /* Sens_GRD_H */
