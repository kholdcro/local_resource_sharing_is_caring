// Author: Antoni Jubes
// Factors for sensor fusion filter
#ifndef Sens_FAC_H
#define Sens_FAC_H

#include "Util_MAT.h"
#include "Util_TIM.h"

// Trajectory smoothing factor (relates different time-successive states).
// It is the one that is used in the prediction step.

/**
 * @brief Information vector for the trajectory smoothing factor (relates
 * different time-successive states).
 * It is the one that is used in the prediction step.
 *
 * The corresponding states will be the previous ground vector and velocity and
 * the estimated ground vector and velocity.
 * Note: It works with the difference between the given estimate and the real
 * one (for all 4 state variables), so that when the optimum is found, to get
 * the real state variables, the estimate must be added to the result.
 * Note 2: since the dimensions are decoupled, this is a factor for only one
 * dimension of the state vector. The other dimensions are not affected by this
 * factor, so this needs to be called for each dimension separately.
 *
 * @param sigma_a Standard deviation of the acceleration.
 * @param previous_gt Previous ground vector estimate.
 * @param previous_vt Previous ground velocity estimate.
 * @param estimated_gt Estimated ground vector.
 * @param estimated_vt Estimated ground velocity.
 * @param delta_t Time difference between the previous and the estimated state.
 * @return Information vector.
 */
struct vector_4d factor_ground_trajectory_info_vec(
    float sigma_a, float previous_gt, float previous_vt, float estimated_gt,
    float estimated_vt, time_t delta_t);
/**
 * @brief Information matrix for the trajectory smoothing factor (relates
 * different time-successive states).
 * It is the one that is used in the prediction step.
 *
 * The corresponding states will be the previous ground vector and velocity and
 * the estimated ground vector and velocity.
 * Note: It works with the difference between the given estimate and the real
 * one (for all 4 state variables), so that when the optimum is found, to get
 * the real state variables, the estimate must be added to the result.
 *
 * @param sigma_a Standard deviation of the acceleration.
 * @param previous_gt Previous ground vector estimate.
 * @param previous_vt Previous ground velocity estimate.
 * @param estimated_gt Estimated ground vector.
 * @param estimated_vt Estimated ground velocity.
 * @param delta_t Time difference between the previous and the estimated state.
 * @return Information matrix.
 */
struct matrix_4d factor_ground_trajectory_info_mat(
    float sigma_a, float previous_gt, float previous_vt, float estimated_gt,
    float estimated_vt, time_t delta_t);

// Ground vector measurement factor.
// It is the one that is used in the update step.

/**
 * @brief Information vector for the ground vector measurement factor.
 * It is the one that is used in the update step.
 *
 * The full corresponding states will be the estimated ground vector and
 * velocity. However, only the ground vector is used in the factor.
 * Note: It works with the difference between the given estimate and the real
 * one (for all 2 state variables), so that when the optimum is found, to get
 * the real state variables, the estimate must be added to the result.
 * Note 2: since the dimensions are decoupled, this is a factor for only one
 * dimension of the state vector. The other dimensions are not affected by this
 * factor, so this needs to be called for each dimension separately.
 *
 * @param sigma_sensor_g Standard deviation of the ground vector measurement.
 * @param estimated_gt Estimated ground vector.
 * @param g_measurement Ground vector measurement.
 * @return Information vector.
 */
float factor_ground_measurement_info_vec(float sigma_sensor_g,
                                         float estimated_gt,
                                         float g_measurement);
/**
 * @brief Information matrix for the ground vector measurement factor.
 * It is the one that is used in the update step.
 *
 * The full corresponding states will be the estimated ground vector and
 * velocity. However, only the ground vector is used in the factor.
 * Note: It works with the difference between the given estimate and the real
 * one (for all 2 state variables), so that when the optimum is found, to get
 * the real state variables, the estimate must be added to the result.
 * Note 2: since the dimensions are decoupled, this is a factor for only one
 * dimension of the state vector. The other dimensions are not affected by this
 * factor, so this needs to be called for each dimension separately.
 *
 * @param sigma_sensor_g Standard deviation of the ground vector measurement
 * (coming from accelerometer).
 * @param estimated_gt Estimated ground vector.
 * @param g_measurement Ground vector measurement.
 * @return Information matrix.
 */
float factor_ground_measurement_info_mat(float sigma_sensor_g,
                                         float estimated_gt,
                                         float g_measurement);

// Angle factor.
// It is the one that is used in the update step whenever the neighbor
// information is sent.

/**
 * @brief Information vector for the angle factor.
 * It is the one that is used in the update step whenever the neighbor
 * information is sent.
 *
 * The full corresponding states will be the estimated ground vector and
 * velocity. However, only the ground vector is used in the factor.
 * Note: It works with the difference between the given estimate and the real
 * one (for all 2 state variables), so that when the optimum is found, to get
 * the real state variables, the estimate must be added to the result.
 * Note 2: in this case, the dimensions are decoupled for the information matrix
 * but not for the information vector, so we will work with 3D vector and 3x3
 * matrices.
 *
 * @param sigma_angle Standard deviation assumed on the different components of
 * the neighbor estimate.
 * @param connected_edge Edge that connects the current node with the neighbor.
 * @param angle Angle estimate from the angle edge filter.
 * @param estimated_gt Estimated ground vector.
 * @param neighbor_gt Neighbor ground vector estimate.
 * @return Information vector.
 */
struct vector_3d factor_angle_value_info_vec(float sigma_angle,
                                             uint8_t connected_edge,
                                             float angle,
                                             struct vector_3d estimated_gt,
                                             struct vector_3d neighbor_gt);
/**
 * @brief Information matrix for the angle factor.
 * It is the one that is used in the update step whenever the neighbor
 * information is sent.
 *
 * The full corresponding states will be the estimated ground vector and
 * velocity. However, only the ground vector is used in the factor.
 * Note: It works with the difference between the given estimate and the real
 * one (for all 2 state variables), so that when the optimum is found, to get
 * the real state variables, the estimate must be added to the result.
 * Note 2: in this case, the dimensions are decoupled for the information matrix
 * but not for the information vector, so we will work with 3D vector and 3x3
 * matrices.
 *
 * Note 3: should use the neighbor info matrix in this filter, but for now we
 * will use the simple version where we assume the neighbor info matrix to be
 * the identity matrix times the 1/sigma_angle^2, to simplify the calculations
 * (no gbp updates equations necessary).
 *
 * @param sigma_angle Standard deviation assumed on the different components of
 * the neighbor estimate.
 * @param connected_edge Edge that connects the current node with the neighbor.
 * @param angle Angle estimate from the angle edge filter.
 * @param estimated_gt Estimated ground vector.
 * @param neighbor_gt Neighbor ground vector estimate.
 * @return Information matrix.
 */
struct matrix_3d factor_angle_value_info_mat(float sigma_angle,
                                             uint8_t connected_edge,
                                             float angle,
                                             struct vector_3d estimated_gt,
                                             struct vector_3d neighbor_gt);

#endif /* Sens_FAC_H */
