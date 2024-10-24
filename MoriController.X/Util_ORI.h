// Author: Antoni Jubes
// Orientation Utils Functions used in the Sensor Fusion
#ifndef Util_ORI_H
#define Util_ORI_H

#include <stdint.h>

#include "Util_MAT.h"

/**
 * @brief Calculates the angle between two connected Mori modules. The angle is
 * 0 when the two MORI top faces would be totally facing each other, and 360
 * when they would be completely facing away from each other.
 *
 * The inputs are the ground vectors (refer to Sens_GRD.h for more information
 * on its definition) of the local Mori module and the neighbor Mori module.
 * Both vectors are expressed in their own frame. Each of their frames are
 * defined by:
 *  - z perpendicular to the MORI
 *  - x parallel to the connected edge
 *  - y points to the center of the MORI (!check if this is really like this!)
 *
 * This function is symmetric.
 *
 * @param local_ground ground vector of the local Mori module in its own local
 * edge frame
 * @param neighbor_ground ground vector of the neighbor Mori module in its own
 * neighbor edge frame
 * @return angle between the two MORIs in degrees
 */
float orientation_angle(struct vector_3d, struct vector_3d);

/**
 * @brief Calculates the velocity of the angle between two connected Mori using
 * the ground vectors and their velocities. Refer to orientation_angle for more
 * information on the definition of the angle and how the ground vectors should
 * be.
 *
 * This function is symmetric.
 *
 * @param local_ground ground vector of the local Mori module in its own local
 * edge frame
 * @param neighbor_ground ground vector of the neighbor Mori module in its own
 * neighbor edge frame
 * @param local_velocity velocity of the local ground vector in its own local
 * edge frame
 * @param neighbor_velocity velocity of the neighbor ground vector in its own
 * neighbor edge frame
 * @return velocity of the angle between the two MORIs in degrees per second
 */
float orientation_velocity(struct vector_3d, struct vector_3d, struct vector_3d,
                           struct vector_3d);

/**
 * @brief Converts a vector expressed in the Mori frame to the frame of the edge
 * of the Mori module. The Mori frame centered around an edge is defined by:
 * - z perpendicular to the MORI
 * - x parallel to the centered edge
 * - y points to the center of the MORI (!check if this is really like this!)
 *
 * The base Mori frame is the frame of the edge 0.
 *
 * Since it is a triangle, the Mori frame of the edge 1 is rotated 120 degrees
 * clockwise around the z axis. The Mori frame of the edge 2 is rotated 120
 * degrees counterclockwise around the z axis.
 *
 * ! take into account the case where the edge lengths are not the same since
 * they are reconfigurable!!
 *
 * @param curr_ground ground vector of the Mori module in its own base frame
 * (edge 0)
 * @param edge edge number of the Mori module that we want to transform it to
 * (0, 1 or 2)
 * @return ground vector of the Mori module in its own edge frame
 */
struct vector_3d mori_frame_to_edge_frame(struct vector_3d, uint8_t);

#endif