// Author: Antoni Jubes
// Factors for sensor fusion filter
#include "Sens_FAC.h"

#include <math.h>
#include "Util_TIM.h"
#include "Util_MAT.h"
#include "Coms_ESP.h"

#define M_PI_DIV_180    0.01745329251994329577  /* pi/180 */

//
struct vector_4d factor_ground_trajectory_info_vec(float sigma_a, float previous_gt, float previous_vt, float estimated_gt, float estimated_vt, time_t delta_t){
   
   const struct matrix_4d info_mat = factor_ground_trajectory_info_mat(sigma_a, previous_gt, previous_vt, estimated_gt, estimated_vt, delta_t); 

   const struct vector_4d mean = {-previous_gt, -previous_vt, -estimated_gt, -estimated_vt};
   
   return mat_product_vec_4d(info_mat,mean);
}

struct matrix_4d factor_ground_trajectory_info_mat(float sigma_a, float previous_gt, float previous_vt, float estimated_gt, float estimated_vt, time_t delta_t){
   float sigma_inv = 1./(sigma_a*sigma_a);
   float delta_t_inv = 1./(float)delta_t;
   float delta_t_inv_2 = delta_t_inv*delta_t_inv;
   float delta_t_inv_3 = delta_t_inv_2*delta_t_inv;
   // premultiply with sigma_inv for optimization
   delta_t_inv *= sigma_inv;
   delta_t_inv_2 *= sigma_inv;
   delta_t_inv_3 *= sigma_inv;

   // return the info matrix
   struct matrix_4d A = {{12*delta_t_inv_3, 6*delta_t_inv_2, -12*delta_t_inv_3, 6*delta_t_inv_2},
                          {6*delta_t_inv_2, 4*delta_t_inv, -6*delta_t_inv_2, 2*delta_t_inv},
                          {-12*delta_t_inv_3, -6*delta_t_inv_2, 12*delta_t_inv_3, -6*delta_t_inv_2},
                          {6*delta_t_inv_2, 2*delta_t_inv, -6*delta_t_inv_2, 4*delta_t_inv}};

   return A;
}

// measurement and correlation structs and functions

// ground measurement only affects independently the different dimensions of the state vector
// this factor only applies to one dimension of the ground state vector
float factor_ground_measurement_info_vec(float sigma_sensor_g, float estimated_gt, float g_measurement){
   return (g_measurement-estimated_gt)/(sigma_sensor_g*sigma_sensor_g);
}
// this factor only applies to one dimension of the ground state vector
float factor_ground_measurement_info_mat(float sigma_sensor_g, float estimated_gt, float g_measurement){
   return 1./(sigma_sensor_g*sigma_sensor_g);

}

// angle factors
// should use the neighbor info matrix in this filter, but for now we will use the simple version where we assume the neighbor info matrix 
// to be the identity matrix times the 1/sigma_angle^2

// local_R_neighbor changes the frame of the neighbor_gt into the frame of the estimated_gt
struct matrix_3d local_R_neighbor(uint8_t connected_edge, float angle){
   static const float cos_edge[3] = {1, -0.5, -0.5};
   static const float sin_edge[3] = {0, 0.8660254037844386, -0.8660254037844386};   

   float cos_ang = cosf((-angle+180)*M_PI_DIV_180);
   float sin_ang = sinf((-angle+180)*M_PI_DIV_180);

   struct matrix_3d lRn = {{-cos_edge[connected_edge], -cos_ang*sin_edge[connected_edge], sin_ang*sin_edge[connected_edge]},
                           {sin_edge[connected_edge], -cos_ang*cos_edge[connected_edge], sin_ang*cos_edge[connected_edge]},
                           {0, sin_ang, cos_ang}};

   return lRn;
}

// this factor applies to all dimensions of the ground state vector
struct vector_3d factor_angle_value_info_vec(float sigma_angle, uint8_t connected_edge, float angle, struct vector_3d estimated_gt, struct vector_3d neighbor_gt){
   // rotate the neighbor_gt into the frame of the estimated_gt
   struct matrix_3d l_R_n = local_R_neighbor(connected_edge, angle);
   struct vector_3d nu = vec_subtract_3d(mat_product_vec_3d(l_R_n, neighbor_gt), estimated_gt);
   
   float sigma_inv = 1./(sigma_angle*sigma_angle);

   return vec_scale_r_3d(nu, sigma_inv);
}
// this factor applies to all dimensions of the ground state vector
// a simple version where we assume the neighbor info matrix to be proportional to 
// the identity matrix, so the gbp update equations do not need to be applied
struct matrix_3d factor_angle_value_info_mat(float sigma_angle, uint8_t connected_edge, float angle, struct vector_3d estimated_gt, struct vector_3d neighbor_gt){
   // if what we assume is an identity matrix for the neighbor info, after changing basis 
   // with the rotation matrix, it will still be an identity matrix!
   // so no need to premultiply and post multiply with the rotation matrix!
   float sigma_inv = 1./(sigma_angle*sigma_angle);
   // identity matrix times sigma_inv
   struct matrix_3d A = {{sigma_inv, 0, 0},
                         {0, sigma_inv, 0},
                         {0, 0, sigma_inv}};
   return A;
}
