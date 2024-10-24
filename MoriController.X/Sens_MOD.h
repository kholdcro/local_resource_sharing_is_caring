// Author: Antoni Jubes
// ! maybe should be renamed to Sens_FUS?
#ifndef SENS_MOD_H
#define SENS_MOD_H

#include <stdint.h>

#include "Sens_GRD.h"
#include "Util_TIM.h"

/**
 * @brief Initializes the module.
 *
 * @param connections Array of 3 booleans, where each index represents a
 * connection to a neighbor.
 */
void Sens_MOD_initialize();

void Sens_MOD_update_connections(volatile bool connections[]);

/**
 * @brief Updates the local angle estimate from the local sensor of the given
 * edge.
 *
 * @param edge The edge whose angle will be updated.
 * @param angle The new angle measurement-estimate.
 */
void Sens_MOD_update_local_angle(uint8_t edge, float angle);
/**
 * @brief Updates the neighbor angle estimate from the neighbor sensor of the
 * given edge.
 *
 * @param edge The edge whose neighbor angle will be updated.
 * @param angle The new angle measurement-estimate.
 */
void Sens_MOD_update_neighbor_angle(uint8_t edge, float angle);

/**
 * @brief Adds a new ground measurement to the module that comes from the
 * accelerometer, which is assumed to be in the edge 0 frame.
 *
 * @param x The x component of the measurement.
 * @param y The y component of the measurement.
 * @param z The z component of the measurement.
 */
void Sens_MOD_add_ground_measurement(uint16_t x, uint16_t y, uint16_t z);
/**
 * @brief Updates the ground vector state of the neighbor of the given edge.
 *
 * @param edge The edge whose neighbor will be updated.
 * @param neighbor_state The new ground state of the neighbor.
 */
void Sens_MOD_update_neighbor_state(uint8_t edge,
                                    struct ground_state neighbor_state);

/**
 * @brief Iterates the two different filters of the module (the ground vector
 * filter and the angle filter).
 * @details This function uses the time gotten from Util_TIM_get_time(), so that
 * needs to be periodically updated.
 */
void Sens_MOD_iterate_filter(void);

float Sens_MOD_get_ground_angle(uint8_t);

uint16_t Sens_MOD_GetAngleFormatted(uint8_t edge, bool withLiveOffset);
float Sens_MOD_GetAngle(uint8_t edge, bool WithLiveOffset);
float Sens_MOD_GetDelta(uint8_t edge);
void Sens_MOD_SetLiveOffset(uint8_t edge, uint16_t nbrFusVal);
float Sens_MOD_GetLrgOffsetMult(uint8_t edge);

#endif