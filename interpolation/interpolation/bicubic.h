#ifndef _BICUBIC_H_
#define _BICUBIC_H_

// 如果编译器没有关于size_t类型的定义，取消下面一行的注释即可。
// typedef unsigned int size_t

// 样条函数参数的定义，通常取-0.5或-0.75。
static const float SPLINE_PARA = -.75F;

float fBicubic(float* mesh_x, size_t len_x, float* mesh_y, size_t len_y, float* mesh_value,
			   float coordinate_x, float coordinate_y);
float fInterpolateKernal(float point_x, float point_y, float *mesh_x, size_t len_x,
						 float *mesh_y, size_t len_y, float *mesh_value);
float fFindPointPosition(float value, float* list, size_t len, int* flag);
int fFindValuePositionInList(float value, float* list, size_t len, int* flag);
inline float fSpline(float x);

#endif
