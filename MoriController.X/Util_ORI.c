#include "Util_ORI.h"

#include <math.h>
#include <stdint.h>

// precompute 180/pi
#define PI_180 57.295779513082320876798154814105

float orientation_angle(struct vector_3d local_ground,
                        struct vector_3d neighbor_ground) {
  float norm_local =
      local_ground.y * local_ground.y + local_ground.z * local_ground.z;
  float norm_neighbor = neighbor_ground.y * neighbor_ground.y +
                        neighbor_ground.z * neighbor_ground.z;

  if (norm_local < 1e-6 || norm_neighbor < 1e-6) {
    return 180;
  }
  
  float cos_alpha = (-local_ground.y * neighbor_ground.y +
                     local_ground.z * neighbor_ground.z);
  float sin_alpha =
      (local_ground.y * neighbor_ground.z + local_ground.z * neighbor_ground.y);
  
  return 180 - atan2f(sin_alpha, cos_alpha) * PI_180;
}

float orientation_velocity(struct vector_3d local_ground,
                           struct vector_3d neighbor_ground,
                           struct vector_3d local_velocity,
                           struct vector_3d neighbor_velocity) {
  float norm_local =
      local_ground.y * local_ground.y + local_ground.z * local_ground.z;
  float norm_neighbor = neighbor_ground.y * neighbor_ground.y +
                        neighbor_ground.z * neighbor_ground.z;

  if (norm_local < 1e-6 || norm_neighbor < 1e-6) {
    return 0.;
  }

  float velocity_local =
      (local_ground.y * local_velocity.z - local_ground.z * local_velocity.y) /
      norm_local;
  float velocity_neighbor = (neighbor_ground.y * neighbor_velocity.z -
                             neighbor_ground.z * neighbor_velocity.y) /
                            norm_neighbor;

  // we sum the velocities since they come from opposite frames of reference
  return (velocity_local + velocity_neighbor) * PI_180;
}

struct vector_3d mori_frame_to_edge_frame(struct vector_3d curr_ground,
                                          uint8_t edge) {
  struct vector_3d result;

  // rotates curr_ground 120 degrees through z axis (positive if edge is
  // 1, negative if edge is 2, no rotation if edge is 0) the result is stored in
  // result the sinus and cosinus are precomputed for speed
  static const float cosinus[3] = {1, -0.5, -0.5};
  static const float sinus[3] = {0, 0.8660254037844386, -0.8660254037844386};
  result.x = curr_ground.x * cosinus[edge] - curr_ground.y * sinus[edge];
  result.y = curr_ground.x * sinus[edge] + curr_ground.y * cosinus[edge];
  result.z = curr_ground.z;

  return result;
}
