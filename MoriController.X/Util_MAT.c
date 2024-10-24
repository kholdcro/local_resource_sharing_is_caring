// Matrix operations used in the Sensor Fusion

#include "Util_MAT.h"

#include <math.h>
#include <stdint.h>

// 2d operations
struct vector_2d vec_sum_2d(struct vector_2d left, struct vector_2d right) {
  struct vector_2d result;
  result.x = left.x + right.x;
  result.y = left.y + right.y;
  return result;
}

struct vector_2d vec_subtract_2d(struct vector_2d left,
                                 struct vector_2d right) {
  struct vector_2d result;
  result.x = left.x - right.x;
  result.y = left.y - right.y;
  return result;
}

struct vector_2d mat_product_vec_2d(struct matrix_2d mat,
                                    struct vector_2d vec) {
  struct vector_2d result;
  result.x = mat.row_1.x * vec.x + mat.row_1.y * vec.y;
  result.y = mat.row_2.x * vec.x + mat.row_2.y * vec.y;
  return result;
}

struct matrix_2d mat_inverse_2d(struct matrix_2d mat) {
  struct matrix_2d result;
  float inv_det = 1. / (mat.row_1.x * mat.row_2.y - mat.row_1.y * mat.row_2.x);
  result.row_1.x = mat.row_2.y * inv_det;
  result.row_1.y = -mat.row_1.y * inv_det;
  result.row_2.x = -mat.row_2.x * inv_det;
  result.row_2.y = mat.row_1.x * inv_det;
  return result;
}

// struct matrix_2d mat_sum_2d(struct matrix_2d left, struct matrix_2d right) {
//     struct matrix_2d result;
//     result.row_1 = vec_sum_2d(left.row_1, right.row_1);
//     result.row_2 = vec_sum_2d(left.row_2, right.row_2);
//     return result;
// }

struct matrix_2d mat_subtract_2d(struct matrix_2d left,
                                 struct matrix_2d right) {
  struct matrix_2d result;
  result.row_1 = vec_subtract_2d(left.row_1, right.row_1);
  result.row_2 = vec_subtract_2d(left.row_2, right.row_2);
  return result;
}

struct matrix_2d mat_product_2d(struct matrix_2d left, struct matrix_2d right) {
  struct matrix_2d result;
  result.row_1.x = left.row_1.x * right.row_1.x + left.row_1.y * right.row_2.x;
  result.row_1.y = left.row_1.x * right.row_1.y + left.row_1.y * right.row_2.y;
  result.row_2.x = left.row_2.x * right.row_1.x + left.row_2.y * right.row_2.x;
  result.row_2.y = left.row_2.x * right.row_1.y + left.row_2.y * right.row_2.y;
  return result;
}

// Vector operations
// addition operations
struct vector_3d vec_sum_3d(struct vector_3d left, struct vector_3d right) {
  struct vector_3d result;
  result.x = left.x + right.x;
  result.y = left.y + right.y;
  result.z = left.z + right.z;
  return result;
}
// struct vector_3d vec_sum_scalar_l_3d(float left, struct vector_3d right) {
//     struct vector_3d result;
//     result.x = left + right.x;
//     result.y = left + right.y;
//     result.z = left + right.z;
//     return result;
// }
// struct vector_3d vec_sum_scalar_r_3d(struct vector_3d left, float right) {
//     struct vector_3d result;
//     result.x = left.x + right;
//     result.y = left.y + right;
//     result.z = left.z + right;
//     return result;
// }

// subtraction operations
struct vector_3d vec_subtract_3d(struct vector_3d left,
                                 struct vector_3d right) {
  struct vector_3d result;
  result.x = left.x - right.x;
  result.y = left.y - right.y;
  result.z = left.z - right.z;
  return result;
}

// product operations
float vec_dot_product_3d(struct vector_3d left, struct vector_3d right) {
  return left.x * right.x + left.y * right.y + left.z * right.z;
}
// struct vector_3d vec_scale_l_3d(float left, struct vector_3d right) {
//     struct vector_3d result;
//     result.x = left * right.x;
//     result.y = left * right.y;
//     result.z = left * right.z;
//     return result;
// }
struct vector_3d vec_scale_r_3d(struct vector_3d left, float right) {
  struct vector_3d result;
  result.x = left.x * right;
  result.y = left.y * right;
  result.z = left.z * right;
  return result;
}

struct vector_3d vec_normalize_3d(struct vector_3d v) {
  float magnitude = 1 / sqrtf(v.x * v.x + v.y * v.y + v.z * v.z);
  struct vector_3d result;
  result.x = v.x * magnitude;
  result.y = v.y * magnitude;
  result.z = v.z * magnitude;
  return result;
}

// Matrix operations

// addition operations

// struct matrix_3d mat_sum_scalar_l_3d(float left, struct matrix_3d right) {
//     struct matrix_3d result;
//     result.row_1 = vec_sum_scalar_l_3d(left, right.row_1);
//     result.row_2 = vec_sum_scalar_l_3d(left, right.row_2);
//     result.row_3 = vec_sum_scalar_l_3d(left, right.row_3);
//     return result;
// }
// struct matrix_3d mat_sum_scalar_r_3d(struct matrix_3d left, float right){
//     struct matrix_3d result;
//     result.row_1 = vec_sum_scalar_r_3d(left.row_1, right);
//     result.row_2 = vec_sum_scalar_r_3d(left.row_2, right);
//     result.row_3 = vec_sum_scalar_r_3d(left.row_3, right);
//     return result;
// }

// // product operations
// struct matrix_3d mat_scale_l_3d(float left, struct matrix_3d right) {
//     struct matrix_3d result;
//     result.row_1 = vec_scale_l_3d(left, right.row_1);
//     result.row_2 = vec_scale_l_3d(left, right.row_2);
//     result.row_3 = vec_scale_l_3d(left, right.row_3);
//     return result;
// }
// struct matrix_3d mat_scale_r_3d(struct matrix_3d left, float right) {
//     struct matrix_3d result;
//     result.row_1 = vec_scale_r_3d(left.row_1, right);
//     result.row_2 = vec_scale_r_3d(left.row_2, right);
//     result.row_3 = vec_scale_r_3d(left.row_3, right);
//     return result;
// }

struct vector_3d mat_product_vec_3d(struct matrix_3d left,
                                    struct vector_3d right) {
  struct vector_3d result;
  result.x = vec_dot_product_3d(left.row_1, right);
  result.y = vec_dot_product_3d(left.row_2, right);
  result.z = vec_dot_product_3d(left.row_3, right);
  return result;
}
// struct vector_3d vec_product_mat_3d(struct vector_3d left, struct matrix_3d
// right) {
//     struct vector_3d result;
//     result.x = left.x * right.row_1.x + left.y * right.row_2.x + left.z *
//     right.row_3.x; result.y = left.x * right.row_1.y + left.y * right.row_2.y
//     + left.z * right.row_3.y; result.z = left.x * right.row_1.z + left.y *
//     right.row_2.z + left.z * right.row_3.z; return result;
// }

// struct matrix_3d mat_transpose_3d(struct matrix_3d mat) {
//     struct matrix_3d result;
//     result.row_1.x = mat.row_1.x;
//     result.row_1.y = mat.row_2.x;
//     result.row_1.z = mat.row_3.x;
//     result.row_2.x = mat.row_1.y;
//     result.row_2.y = mat.row_2.y;
//     result.row_2.z = mat.row_3.y;
//     result.row_3.x = mat.row_1.z;
//     result.row_3.y = mat.row_2.z;
//     result.row_3.z = mat.row_3.z;
//     return result;
// }

// same but for 4D vectors and matrices

// struct vector_4d vec_sum_4d(struct vector_4d left, struct vector_4d right) {
//     struct vector_4d result;
//     result.x = left.x + right.x;
//     result.y = left.y + right.y;
//     result.z = left.z + right.z;
//     result.w = left.w + right.w;
//     return result;
// }

// struct vector_4d vec_sum_scalar_l_4d(float left, struct vector_4d right) {
//     struct vector_4d result;
//     result.x = left + right.x;
//     result.y = left + right.y;
//     result.z = left + right.z;
//     result.w = left + right.w;
//     return result;
// }

// struct vector_4d vec_sum_scalar_r_4d(struct vector_4d left, float right) {
//     struct vector_4d result;
//     result.x = left.x + right;
//     result.y = left.y + right;
//     result.z = left.z + right;
//     result.w = left.w + right;
//     return result;
// }

float vec_dot_product_4d(struct vector_4d left, struct vector_4d right) {
  return ((left.x * right.x) + (left.y * right.y) + (left.z * right.z) +
         (left.w * right.w));
}

// struct vector_4d vec_scale_l_4d(float left, struct vector_4d right) {
//     struct vector_4d result;
//     result.x = left * right.x;
//     result.y = left * right.y;
//     result.z = left * right.z;
//     result.w = left * right.w;
//     return result;
// }

// struct vector_4d vec_scale_r_4d(struct vector_4d left, float right) {
//     struct vector_4d result;
//     result.x = left.x * right;
//     result.y = left.y * right;
//     result.z = left.z * right;
//     result.w = left.w * right;
//     return result;
// }

// struct vector_4d vec_normalize_4d(struct vector_4d left) {
//     float magnitude = sqrtf(left.x * left.x + left.y * left.y + left.z *
//     left.z + left.w * left.w); struct vector_4d result; result.x = left.x /
//     magnitude; result.y = left.y / magnitude; result.z = left.z / magnitude;
//     result.w = left.w / magnitude;
//     return result;
// }

// struct matrix_4d mat_sum_scalar_l_4d(float left, struct matrix_4d right) {
//     struct matrix_4d result;
//     result.row_1 = vec_sum_scalar_l_4d(left, right.row_1);
//     result.row_2 = vec_sum_scalar_l_4d(left, right.row_2);
//     result.row_3 = vec_sum_scalar_l_4d(left, right.row_3);
//     result.row_4 = vec_sum_scalar_l_4d(left, right.row_4);
//     return result;
// }

// struct matrix_4d mat_sum_scalar_r_4d(struct matrix_4d left, float right) {
//     struct matrix_4d result;
//     result.row_1 = vec_sum_scalar_r_4d(left.row_1, right);
//     result.row_2 = vec_sum_scalar_r_4d(left.row_2, right);
//     result.row_3 = vec_sum_scalar_r_4d(left.row_3, right);
//     result.row_4 = vec_sum_scalar_r_4d(left.row_4, right);
//     return result;
// }

// struct matrix_4d mat_scale_l_4d(float left, struct matrix_4d right) {
//     struct matrix_4d result;
//     result.row_1 = vec_scale_l_4d(left, right.row_1);
//     result.row_2 = vec_scale_l_4d(left, right.row_2);
//     result.row_3 = vec_scale_l_4d(left, right.row_3);
//     result.row_4 = vec_scale_l_4d(left, right.row_4);
//     return result;
// }

// struct matrix_4d mat_scale_r_4d(struct matrix_4d left, float right) {
//     struct matrix_4d result;
//     result.row_1 = vec_scale_r_4d(left.row_1, right);
//     result.row_2 = vec_scale_r_4d(left.row_2, right);
//     result.row_3 = vec_scale_r_4d(left.row_3, right);
//     result.row_4 = vec_scale_r_4d(left.row_4, right);
//     return result;
// }

struct vector_4d mat_product_vec_4d(struct matrix_4d left,
                                    struct vector_4d right) {
  struct vector_4d result;
  result.x = vec_dot_product_4d(left.row_1, right);
  result.y = vec_dot_product_4d(left.row_2, right);
  result.z = vec_dot_product_4d(left.row_3, right);
  result.w = vec_dot_product_4d(left.row_4, right);
  return result;
}

// struct vector_4d vec_product_mat_4d(struct vector_4d left, struct matrix_4d
// right){
//     struct vector_4d result;
//     result.x = left.x * right.row_1.x + left.y * right.row_2.x + left.z *
//     right.row_3.x + left.w * right.row_4.x; result.y = left.x * right.row_1.y
//     + left.y * right.row_2.y + left.z * right.row_3.y + left.w *
//     right.row_4.y; result.z = left.x * right.row_1.z + left.y * right.row_2.z
//     + left.z * right.row_3.z + left.w * right.row_4.z; result.w = left.x *
//     right.row_1.w + left.y * right.row_2.w + left.z * right.row_3.w + left.w
//     * right.row_4.w; return result;
// }