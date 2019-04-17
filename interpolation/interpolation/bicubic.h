#ifndef _BICUBIC_H_
#define _BICUBIC_H_

// ͨ������floatΪdouble��߾���
typedef float ElemType;

// TRIANGEL: ���Էֲ���BELL��Bell�ֲ���BSPLINE��B��������
enum InterpolationType { TRIANGEL = 1, BELL = 2, BSPLINE = 3 };

// ������������
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
