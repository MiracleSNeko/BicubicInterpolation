#ifndef _BICUBIC_H_
#define _BICUBIC_H_

// 通过更改float为double提高精度
typedef float ElemType;

// TRIANGEL: 线性分布，BELL：Bell分布，BSPLINE：B样条曲线
enum InterpolationType { TRIANGEL = 1, BELL = 2, BSPLINE = 3 };

// 函数申明部分
ElemType Bicubic(char* filename, ElemType* coordinate, enum InterpolateType type);
ElemType InterpolateKernal(ElemType point_x, ElemType point_y, ElemType *mesh_x, ElemType *mesh_y,
	ElemType *mesh_value, enum InterpolationType type);
ElemType* FindPointPosition(ElemType point_x, ElemType point_y, ElemType *mesh_x,
	ElemType *mesh_y, int flag);
int FindValuePositionInList(ElemType value, ElemType* list, int flag);
inline ElemType TriangelInterpolation(ElemType x);
inline ElemType BellInterpolation(ElemType x);
inline ElemType BsplineInterpolation(ElemType x);

#endif
