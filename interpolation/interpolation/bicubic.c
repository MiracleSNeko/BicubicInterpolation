/***********************************************************************
 *
 * Bicubic Interpolation
 * 双三次插值的实现
 * 
 * 
 * @ filename: bicubic.c
 * @ date: 2019-4-8
 * @ brief: 
		双三次插值计算涉及16个像素点，通过将16个像素点进行权重卷积得到新的像素值。
		输入： excel文件(.xls), 待求值的插值点(x, y)
		输出： 插值后待求值点的函数值f(x, y)
 * @ change history: 
  		<date> | <discription>	

 *
 **********************************************************************/

#include <stdio.h>

// 通过更改float为double提高精度
typedef float ElemType;

/* 
 * @ brief:
		双三次插值函数，返回在给定点的函数值估计 
		输入： 待求值的点，插值区域网格点和函数值
		输出： 待求值点的函数值估计
		若待求值点已经在列表中，返回NaN
 * @ change history:
		<date> | <discription>

 */
ElemType Bicubic(ElemType point_x, ElemType point_y, ElemType *mesh_x, ElemType *mesh_y, 
				 ElemType *mesh_value) {
	ElemType* position_in_mesh;
	position_in_mesh = FindPointPosition(point_x, point_y, mesh_x, mesh_y);
	if (NULL == position_in_mesh) return 0. / 0.;
	// 计算部分
	free(position_in_mesh);
	position_in_mesh = NULL;
}

/*
 * @ brief:
		确定给出节点在已知网格中的带小数坐标
		输入：待求值的点，插值区域网格点
		输出：带小数坐标
		如果给定点已经在列表中，返回空指针
 * @ change history:
		<date> | <discription>

 */
ElemType* FindPointPosition(ElemType point_x, ElemType point_y, ElemType *mesh_x, 
							ElemType *mesh_y) {
	ElemType* position;
	int position_int_x, position_int_y;
	position_int_x = FindValuePositionInList(point_x, mesh_x);
	position_int_y = FindValuePositionInList(point_y, mesh_y);
	if (-1 == position_int_x || -1 == position_int_y) {
		return NULL;
	}
	position = (ElemType *)malloc(sizeof(ElemType) * 2);
	position[0] = (ElemType)position_int_x + point_x / (mesh_x[position_int_x] + mesh_x[position_int_x + 1]);
	position[1] = (ElemType)position_int_y + point_y / (mesh_y[position_int_y] + mesh_y[position_int_y + 1]);
	return position;
}

/*
 * @ brief:
		在有序列表中查找值不大于给定数的最大元素的位置
		输入： 给定数，给定列表
		输出： 不大于给定数的最大元素索引的位置
		如果给定值已经在列表中，返回-1
 * @ change history:
		<date> | <discription>
		190415 | 更改返回值类型：ElemType -> int

 */
int FindValuePositionInList(ElemType value, ElemType* list) {
	int cnt = 0;
	int len = sizeof(list) / sizeof(ElemType);
	while (value > list[cnt]) ++cnt;
	if (value == list[cnt]) cnt = -1;
	return cnt;
}
