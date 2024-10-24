// Author: Antoni Jubes
/**
 * the code may not be the most efficient, but it is functional and easy to read
 * ! to optimize for performance, pointers should be used in all the matrix
 * operations instead of copying the structs (to simulate reference just as in
 * C++)
 */
#include <math.h>
#include <stdbool.h>

#include "Util_MAT.h"
#include "Sens_FAC.h"
#include "Util_ANG.h"
#include "Util_ORI.h"
#include "Util_TIM.h"
#include "Sens_GRD.h"
#include "Coms_ESP.h"


// definitions of data structures and their associated methods
struct ground_measurement {
  struct vector_3d measurement;
  time_t time;
};

////const struct causes failure
//const struct ground_measurement empty_measurement = {0., 0., 0., 0};

// functions for measurements ring buffer

#define MAX_MEASUREMENT_DATA_BUFFER 10

struct ground_measurement_ring_buffer {
  uint8_t size;
  uint8_t head;
  uint8_t tail;
  struct ground_measurement data[MAX_MEASUREMENT_DATA_BUFFER];
};
//const struct ground_measurement_ring_buffer empty_buffer = {0, 0, 0, {}};

void gm_append(struct ground_measurement_ring_buffer* data_buffer,
               struct ground_measurement new_data) {
  data_buffer->data[data_buffer->head] = new_data;
  data_buffer->head = (data_buffer->head + 1) % MAX_MEASUREMENT_DATA_BUFFER;
  if (data_buffer->head == data_buffer->tail) {
    data_buffer->tail = (data_buffer->tail + 1) % MAX_MEASUREMENT_DATA_BUFFER;
  } else {
    data_buffer->size++;
  }
}

struct ground_measurement gm_at(
    struct ground_measurement_ring_buffer* data_buffer, uint8_t index) {
  return data_buffer
      ->data[(data_buffer->tail + index) % MAX_MEASUREMENT_DATA_BUFFER];
}

void gm_purge(struct ground_measurement_ring_buffer* data_buffer) {
  data_buffer->size = 0;
  data_buffer->head = 0;
  data_buffer->tail = 0;
}

const struct ground_state ground_state_default = {
    {0., 0., 1.}, {0., 0., 0.}, 0};

// parameters for ground filter

struct parameters {
  float sigma_a, sigma_sensor_g, huber_threshold, sigma_angle;
} params = {0.001, .1, 100, 0.2};

// variables used for ground filter
// THE state variable for ground filter
struct ground_state ground = {
    {0., 0., 1.}, {0., 0., 0.}, 32000};

// information matrix of THE state variable (inverse of covariance matrix)
// three different 2x2 information matrices since the 3 xyz dimensions are
// decoupled
struct matrix_2d previous_information_matrices[3] = {
    {{0., 0.},{ 0., 0.}}, 
    {{0., 0.},{ 0., 0.}}, 
    {{0., 0.},{ 0., 0.}}
};

// measurements are stored in the ring buffer
struct ground_measurement_ring_buffer measurements = {0, 0, 0, {}};

// angle state variables for ground filter
struct angle_state edge_angles[3] = {
    {180., 0., 0.}, {180., 0., 0.}, {180., 0., 0.}};

// neighbor state variables for ground filter
struct ground_state neighbor_states[3] = {  
  {{0., 0., 1.}, {0., 0., 0.}, 32000},
  {{0., 0., 1.}, {0., 0., 0.}, 32000},  
  {{0., 0., 1.}, {0., 0., 0.}, 32000}  
};


bool neighbor_updated[3] = {false, false, false};
bool neighbor_is_connected[3] = {false, false, false};

time_t time_difference(time_t, time_t);

// functions for ground filter

void Sens_GRD_update_connections(bool connections[3]) {
  uint8_t i;
  for (i = 0; i < 3; i++) {
    if (neighbor_is_connected[i] != connections[i]) {
      neighbor_states[i].time = Util_TIM_get_time();
    }
    neighbor_is_connected[i] = connections[i];
  }
}

void Sens_GRD_add_measurement(int16_t x, int16_t y, int16_t z, time_t time) {
    // xyz values confirmed
    const float div = Sens_GRD_Norm_Measurment(x, y, z);

    struct ground_measurement new_measurement = {{
            ((float)x*div), ((float)y*div), ((float)z*div)}, 
            time};
  gm_append(&measurements, new_measurement);
}

void Sens_GRD_purge_measurements(void) { gm_purge(&measurements); }

void Sens_GRD_update_angle_state(uint8_t edge, struct angle_state new_state) {
  edge_angles[edge] = new_state;
}

void Sens_GRD_update_neighbor_state(uint8_t edge,
                                    struct ground_state neighbor_state) {
  neighbor_states[edge] = neighbor_state;
  neighbor_updated[edge] = true;
}

struct ground_state Sens_GRD_get_ground_estimate(void) { return ground; }


struct ground_state Sens_GRD_get_ground_state_edge_frame(uint8_t edge) {
	struct ground_state edge_frame_ground = ground;
	edge_frame_ground.ground = mori_frame_to_edge_frame(ground.ground, edge);
	edge_frame_ground.velocity = mori_frame_to_edge_frame(ground.velocity, edge);
	return edge_frame_ground;
}

struct angle_state Sens_GRD_compute_angle(uint8_t edge, time_t time) {
  // ! might not be necessary to normalize the ground estimates

    // get ground estimate in edge frame
    struct ground_state ground_edge = Sens_GRD_get_ground_state_edge_frame(edge);
    struct vector_3d ground_estimate = ground_edge.ground;

    // update local ground estimate with velocity
    ground_estimate.x += ground_edge.velocity.x * time_difference(time, ground_edge.time);
    ground_estimate.y += ground_edge.velocity.y * time_difference(time, ground_edge.time);
    ground_estimate.z += ground_edge.velocity.z * time_difference(time, ground_edge.time);

    // normalize ground estimate (necessary?)
    ground_estimate = vec_normalize_3d(ground_estimate);

    // copy neighbor ground estimate into new array
    struct vector_3d neighbor_ground_estimate = neighbor_states[edge].ground;
    // update neighbor ground estimate with velocity
    neighbor_ground_estimate.x +=
        neighbor_states[edge].velocity.x *
        time_difference(time, neighbor_states[edge].time);
    neighbor_ground_estimate.y +=
        neighbor_states[edge].velocity.y *
        time_difference(time, neighbor_states[edge].time);
    neighbor_ground_estimate.z +=
        neighbor_states[edge].velocity.z *
        time_difference(time, neighbor_states[edge].time);
    // normalize neighbor ground estimate (necessary?)
    neighbor_ground_estimate = vec_normalize_3d(neighbor_ground_estimate);

    // initialize result angle state with default values
    struct angle_state result = {180., 0., time};
    // compute angle between ground estimates
    result.ang = orientation_angle(ground_estimate, neighbor_ground_estimate);
    // compute angle velocity between ground estimates
    result.vel =
        orientation_velocity(ground_estimate, neighbor_ground_estimate,
                             ground_edge.velocity, neighbor_states[edge].velocity);

    return result;
//  }
}

/**
 * todo (toni): if performance is an issue, we can optimize the computation of
 * the prediction step by saving the inverted information matrix (cov matrix) at
 * the end of the iteration, so that in the predict step we only need to add the
 * noise covariance matrix (and then perform the inversion). Similar to what is
 * done with filter in Mozart.
 */
void Sens_GRD_iterate_filter(time_t time) {
  // since the filter factors are decoupled, we can iterate over the edges
  // independently
  struct vector_2d nu[3] = {{0., 0.}, {0., 0.}, {0., 0.}};
  struct matrix_2d S[3] = {previous_information_matrices[0],
                           previous_information_matrices[1],
                           previous_information_matrices[2]};

  float delta_t = (float)time_difference(time, ground.time);
  struct vector_2d mean[3] = {{ground.ground.x + delta_t * ground.velocity.x, ground.velocity.x},
                              {ground.ground.y + delta_t * ground.velocity.y, ground.velocity.y},
                              {ground.ground.z + delta_t * ground.velocity.z, ground.velocity.z}};
  
  // predict step over decoupled dimensions
  // arrays so that they can be easily iterated over
  float ground_estimate[3] = {ground.ground.x, ground.ground.y,
                              ground.ground.z};
  float velocity_estimate[3] = {ground.velocity.x, ground.velocity.y,
                                ground.velocity.z};
  
  // iterate over decoupled dimensions
  uint8_t i;
  for (i = 0; i < 3; i++) {
    // predict factor
    struct vector_4d full_nu_i = factor_ground_trajectory_info_vec(
        params.sigma_a, ground_estimate[i], velocity_estimate[i],
        mean[i].x, mean[i].y,
        time_difference(time, ground.time));
    
    struct matrix_4d full_S_i = factor_ground_trajectory_info_mat(
        params.sigma_a, ground_estimate[i], velocity_estimate[i],
        mean[i].x, mean[i].y,
        time_difference(time, ground.time));
    
    if (Sens_GND_Check_NAN(full_nu_i)) {
        Coms_ESP_Verbose_One_Byte(111);
//        return;
    };
    
    // add previous information matrix to top left 2x2 block matrix
    full_S_i.row_1.x += S[i].row_1.x;
    full_S_i.row_1.y += S[i].row_1.y;
    full_S_i.row_2.x += S[i].row_2.x;
    full_S_i.row_2.y += S[i].row_2.y;
    
    // marginalize out the first two variables (previous time state variables)

    // top left
    struct matrix_2d A = {{full_S_i.row_1.x, full_S_i.row_1.y},
                          {full_S_i.row_2.x, full_S_i.row_2.y}};
    // top right
    struct matrix_2d B = {{full_S_i.row_1.z, full_S_i.row_1.w},
                          {full_S_i.row_2.z, full_S_i.row_2.w}};
    // bottom left
    struct matrix_2d C = {{full_S_i.row_3.x, full_S_i.row_3.y},
                          {full_S_i.row_4.x, full_S_i.row_4.y}};
    // bottom right
    struct matrix_2d D = {{full_S_i.row_3.z, full_S_i.row_3.w},
                          {full_S_i.row_4.z, full_S_i.row_4.w}};

    // first vector half
    struct vector_2d a = {full_nu_i.x, full_nu_i.y};
    // second vector half
    struct vector_2d b = {full_nu_i.z, full_nu_i.w};

    // precompute C * A^-1
    struct matrix_2d C_A_inv = mat_product_2d(C, mat_inverse_2d(A));

    // compute S_i = D - C * A^-1 * B   
    S[i] = mat_subtract_2d(D, mat_product_2d(C_A_inv, B));

    // compute nu_i = b - C * A^-1 * a
    nu[i] = vec_subtract_2d(b, mat_product_vec_2d(C_A_inv, a));
  }

  // originally this was non-linear so different linearization steps had to
  // be computed. However, since everything is now simplified and linear, it is
  // good enough to just compute everything in one step.

  // add all measurements to filter through factor
  // ? if it does not work, change it to only one measurement at a time, with is_updated flag 
  for (uint8_t m_i = 0; m_i < measurements.size; ++m_i) {
    struct ground_measurement full_measurement = gm_at(&measurements, m_i);
    float ground_measurement_arr[3] = {full_measurement.measurement.x,
                                   full_measurement.measurement.y,
                                   full_measurement.measurement.z};
    // iterate over decoupled dimensions
    for (uint8_t i = 0; i < 3; i++) {
      // add measurement factor only to the ground variable and not to the
      // velocity
      nu[i].x += factor_ground_measurement_info_vec(
          params.sigma_sensor_g, ground_measurement_arr[i], mean[i].x);
      S[i].row_1.x += factor_ground_measurement_info_mat(
          params.sigma_sensor_g, ground_measurement_arr[i], mean[i].x);
    }
  }

  // prepare ground estimate
  struct vector_3d local_ground_estimate = {mean[0].x,mean[1].x,mean[2].x};
  
  // add all neighbor states to filter through angle factors
  // ? try it with this commented, to see it working at least without neighbors
  for (uint8_t edge = 0; edge < 3; ++edge) {
    // will just add the angle factor once for each time new data is received
    if (neighbor_is_connected[edge] && neighbor_updated[edge]) {
      // update angle value
      float angle =
          edge_angles[edge].ang +
          edge_angles[edge].vel * time_difference(time, edge_angles[edge].time);

      // update neighbor ground vector using the velocity
      struct vector_3d neighbor_ground_estimate = vec_sum_3d(
          neighbor_states[edge].ground,
          vec_scale_r_3d(neighbor_states[edge].velocity,
                         time_difference(time, neighbor_states[edge].time)));

      // normalize this ground vector
      neighbor_ground_estimate = vec_normalize_3d(neighbor_ground_estimate);

      // compute angle factor
      struct vector_3d nu_angle = factor_angle_value_info_vec(
          params.sigma_angle, edge, angle, local_ground_estimate,
          neighbor_ground_estimate);
      float nu_angle_arr[3] = {nu_angle.x, nu_angle.y, nu_angle.z};

      // will not create a matrix_3d struct when it is an identity matrix
      //   struct matrix_3d S_angle = factor_angle_value_info_mat(
      //       params.sigma_angle, edge, angle, local_ground_estimate,
      //       neighbor_ground_estimate);

      // will just work with the proportional value directly
      float sigma_inv = 1.0f / (params.sigma_angle * params.sigma_angle);

      // iterate over decoupled dimensions
      for (uint8_t i = 0; i < 3; i++) {
        // update nu
        nu[i].x += nu_angle_arr[i];
        // update S
        S[i].row_1.x += sigma_inv;
      }
      neighbor_updated[edge] = false;
    }
  }

  struct ground_state new_ground = ground;
  new_ground.ground = local_ground_estimate;
  
  // update step over decoupled dimensions
  for (uint8_t i = 0; i < 3; i++) {
    // compute mean_i = S_i^-1 * nu_i
    struct vector_2d mean_diff =
        mat_product_vec_2d(mat_inverse_2d(S[i]), nu[i]);
    mean[i] = vec_sum_2d(mean[i], mean_diff);
  }
  new_ground.ground.x = mean[0].x;
  new_ground.ground.y = mean[1].x;
  new_ground.ground.z = mean[2].x;
  new_ground.velocity.x = mean[0].y;
  new_ground.velocity.y = mean[1].y;
  new_ground.velocity.z = mean[2].y;
  
  // normalize the ground vector
  new_ground.ground = vec_normalize_3d(new_ground.ground);
  
  // make velocity vector orthogonal to ground vector
  new_ground.velocity = vec_subtract_3d(
      new_ground.velocity,
      vec_scale_r_3d(
          new_ground.ground,
          vec_dot_product_3d(new_ground.ground, new_ground.velocity)));

  // update ground state
  ground = new_ground;

  // update time
  ground.time = time;

  // update the previous information matrices
  previous_information_matrices[0] = S[0];
  previous_information_matrices[1] = S[1];
  previous_information_matrices[2] = S[2];
}


time_t time_difference(time_t time_1, time_t time_2) {
    return ((time_t)(time_1 - time_2));
}


bool Sens_GND_Check_NAN(struct vector_4d value) {
    bool naan_bread = false;
    naan_bread |= isnan(value.x);
    naan_bread |= isnan(value.y);
    naan_bread |= isnan(value.z);
    naan_bread |= isnan(value.w);
//    naan_bread &= isnormal(value.x);
//    naan_bread &= isnormal(value.y);
//    naan_bread &= isnormal(value.z);
//    naan_bread &= isnormal(value.w);
    return naan_bread;
}


// Norm stolen from:
// Adapted from https://www.geeksforgeeks.org/finding-the-frobenius-norm-of-a-given-matrix/

float Sens_GRD_Norm_Measurment(int16_t x, int16_t y, int16_t z) {   
    const float flx = (float) x;
    const float fly = (float) y;
    const float flz = (float) z;
    float sumSq = 0.;
    sumSq += flx * flx;
    sumSq += fly * fly;
    sumSq += flz * flz;
    sumSq = 1/sqrt(sumSq);
    return sumSq;
}