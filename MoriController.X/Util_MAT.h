// Author: Antoni Jubes
// Matrix operations used in the Sensor Fusion
// ! If performance is required, change the parameters to be pointers to the
// structures so that the structures are not copied
#ifndef Util_MAT_H
#define Util_MAT_H

#include <stdint.h>

// 2d vector operations
/**
 * @brief 2d vector structure
 */
struct vector_2d {
  float x;
  float y;
};

struct vector_2d vec_sum_2d(struct vector_2d left, struct vector_2d right);
struct vector_2d vec_subtract_2d(struct vector_2d left, struct vector_2d right);

// 2d matrix operations
struct matrix_2d {
  struct vector_2d row_1;
  struct vector_2d row_2;
};

struct vector_2d mat_product_vec_2d(struct matrix_2d mat, struct vector_2d vec);


struct matrix_2d mat_inverse_2d(struct matrix_2d mat);

// struct matrix_2d mat_sum_2d(struct matrix_2d left, struct matrix_2d right);
struct matrix_2d mat_subtract_2d(struct matrix_2d left, struct matrix_2d right);
struct matrix_2d mat_product_2d(struct matrix_2d left, struct matrix_2d right);

// Vector operations

// define the vector structures
struct vector_3d {
  float x;
  float y;
  float z;
};

// all code is functional and not in place

// addition operations
struct vector_3d vec_sum_3d(struct vector_3d left, struct vector_3d right);
// struct vector_3d vec_sum_scalar_l_3d(float left, struct vector_3d right);
// struct vector_3d vec_sum_scalar_r_3d(struct vector_3d left, float right);

struct vector_3d vec_subtract_3d(struct vector_3d left, struct vector_3d right);

// product operations
float vec_dot_product_3d(struct vector_3d left, struct vector_3d right);
// struct vector_3d vec_scale_l_3d(float left, struct vector_3d right);
struct vector_3d vec_scale_r_3d(struct vector_3d left, float right);

struct vector_3d vec_normalize_3d(struct vector_3d v);

// Matrix operations

struct matrix_3d {
  struct vector_3d row_1;
  struct vector_3d row_2;
  struct vector_3d row_3;
};

// addition operations
// struct matrix_3d mat_sum_3d(struct matrix_3d left, struct matrix_3d right);
// struct matrix_3d mat_sum_scalar_l_3d(float left, struct matrix_3d right);
// struct matrix_3d mat_sum_scalar_r_3d(struct matrix_3d left, float right);

// product operations
// struct matrix_3d mat_scale_l_3d(float left, struct matrix_3d right);
// struct matrix_3d mat_scale_r_3d(struct matrix_3d left, float right);

struct vector_3d mat_product_vec_3d(struct matrix_3d left,
                                    struct vector_3d right);
// struct vector_3d vec_product_mat_3d(struct vector_3d left, struct matrix_3d
// right);

// struct matrix_3d mat_transpose_3d(struct matrix_3d mat);

// same but for 4D vectors and matrices

// define the vector structures
struct vector_4d {
  float x;
  float y;
  float z;
  float w;
};

// addition operations
// struct vector_4d vec_sum_4d(struct vector_4d left, struct vector_4d right);
// struct vector_4d vec_sum_scalar_l_4d(float left, struct vector_4d right);
// struct vector_4d vec_sum_scalar_r_4d(struct vector_4d left, float right);

// product operations
float vec_dot_product_4d(struct vector_4d left, struct vector_4d right);
// struct vector_4d vec_scale_l_4d(float left, struct vector_4d right);
// struct vector_4d vec_scale_r_4d(struct vector_4d left, float right);

// struct vector_4d vec_normalize_4d(struct vector_4d left);

// Matrix operations

struct matrix_4d {
  struct vector_4d row_1;
  struct vector_4d row_2;
  struct vector_4d row_3;
  struct vector_4d row_4;
};

// addition operations
// struct matrix_4d mat_sum_4d(struct matrix_4d left, struct matrix_4d right);
// struct matrix_4d mat_sum_scalar_l_4d(float left, struct matrix_4d right);
// struct matrix_4d mat_sum_scalar_r_4d(struct matrix_4d left, float right);

// product operations
// struct matrix_4d mat_scale_l_4d(float left, struct matrix_4d right);
// struct matrix_4d mat_scale_r_4d(struct matrix_4d left, float right);

struct vector_4d mat_product_vec_4d(struct matrix_4d left,
                                    struct vector_4d right);
// struct vector_4d vec_product_mat_4d(struct vector_4d left, struct matrix_4d
// right);

#endif