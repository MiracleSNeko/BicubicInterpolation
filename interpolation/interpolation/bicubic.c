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
		输入： 文件名(.txt), 待求值的插值点(x, y)
		输出： 插值后待求值点的函数值f(x, y)
 * @ change history: 
		<date> | <ver> | <discription>
		190408 | 0.1.0 | 初始化
		190423 | 0.2.0 | 完成全部程序
		190424 | 0.3.0 | 修改细节错误，改为读取三个txt文件
		190509 | 0.4.0 | 将float和double类型分开处理
		190522 | 1.0.0 | 去掉多余的插值函数和类型，修复寻找全局坐标错误的
						 问题，得到最终计算结果。

 * @ author: yjlin2018@nwpu.edu.cn
 *
 **********************************************************************/
//#define _CRT_SECURE_NO_WARNINGS



#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "bicubic.h"

/*
 * @ brief: 
		用于测试的main函数，实际使用时注释掉即可
		测试方式：直接读取
		输入：横坐标x（float），纵坐标y（float）
 */
int main() {
	float answer = 0.F, coordinate_x = 0.F, coordinate_y = 0.F;
	float* mesh_x = NULL;
	float* mesh_y = NULL;
	float* mesh_value = NULL;
	int i, j;
	mesh_x = (float *)malloc(sizeof(float) * 20);
	for (i = 0; i < 20; ++i) *(mesh_x + i) = (float)i;
	mesh_y = (float *)malloc(sizeof(float) * 20);
	for (i = 0; i < 20; ++i) *(mesh_y + i) = (float)i;
	mesh_value = (float *)malloc(sizeof(float) * 20 * 20);
	for (i = 0; i < 20; ++i) {
		for (j = 0; j < 20; ++j) {
			*(mesh_value + i + 20 * j) = (float)(i*i*i*i+j*j*j*j);
		}
	}
	coordinate_x = 5.5F;
	coordinate_y = 6.5F;
	answer = fBicubic(mesh_x, 20, mesh_y, 20, mesh_value, coordinate_x,
					  coordinate_y);
 	printf("result %lf\n", answer);
	system("pause");
	return 0;
}


/* 类型为Float时的函数 */

/*
 * @ brief:
		读取文件并储存至三个数组中。mesh_x数组储存x方向坐标，mesh_y数组储存y方向坐标，
		mesh_value二维数组储存对应(mesh_x, mesh_y)网格每一点的已知函数值。根据数据计算待求值节点的插值结果。
		输入：储存x坐标、y坐标、结点处函数值的三个数组，待插值结点的坐标
		输出：插值结果
   @ change history:
		<date> | <discription>
		190424 | 将读取一个excel文件强制改为读取三个分开的文件
		190424 | 强制要求提供数组的维度
		190509 | 将输入改为数组本身

 */
float fBicubic(float* mesh_x, size_t len_x, float* mesh_y, size_t len_y, float* mesh_value, 
			   float coordinate_x, float coordinate_y) {
	float bicubic_answer = 0.F;
	bicubic_answer = fInterpolateKernal(coordinate_x, coordinate_y, mesh_x, len_x,
		mesh_y, len_y, mesh_value);
	return bicubic_answer;
}


/* 
 * @ brief:
		双三次插值函数，返回在给定点的函数值估计 
		F(i', j') = sum_{m = -1}^{2} sum_{n = -1}^{2} F(i+m, j+n)R(m-dx)R(dy-n)
		输入： 待求值的点，插值区域网格点和函数值，要使用的插值函数
		输出： 待求值点的函数值估计或精确值（查表）
 * @ change history:
		<date> | <discription>
		190416 | 更改错误处理，给定值在列表中时直接查表返回值
		190417 | 添加枚举变量type用于确定使用哪一种插值函数

 */
float fInterpolateKernal(float point_x, float point_y, float *mesh_x, size_t len_x,
						 float *mesh_y, size_t len_y, float *mesh_value) {
	float position_in_mesh[2] = { 0.F, 0.F };
	float bicubic_answer = 0.F, position_in_mesh_decimal[2] = { 0.F, 0.F };
	int ierr = 0, m = 0, n = 0, position_in_mesh_integer[2] = { 0, 0 };
	float weight = 0.F, increment = 0.F, cloestMeshPoint = 0.F;
	size_t dim2 = len_x;
	position_in_mesh[0] = fFindPointPosition(point_x, mesh_x, len_x, &ierr);
	position_in_mesh[1] = fFindPointPosition(point_y, mesh_y, len_y, &ierr);
	position_in_mesh_integer[0] = (int)floor(position_in_mesh[0]);
	position_in_mesh_integer[1] = (int)floor(position_in_mesh[1]);
	position_in_mesh_decimal[0] = position_in_mesh[0] - (float)position_in_mesh_integer[0];
	position_in_mesh_decimal[1] = position_in_mesh[1] - (float)position_in_mesh_integer[1];
	if (-1 == ierr) {
		bicubic_answer = *(mesh_value + position_in_mesh_integer[0] + dim2 * position_in_mesh_integer[1]);
		return bicubic_answer;
	}
	/* TODO: 边缘项的处理
	 * 要求输入时矩阵关于x和y各自多扩展一维
	 * 或许可考虑通过矩形的对称性进行处理
	 * @ author: yjlin2018@mail.nwpu.edu.cn
	 */
	for (m = -1; m < 3; ++m) {
		for (n = -1; n < 3; ++n) {
			weight = fSpline(m - position_in_mesh_decimal[0])*fSpline(n - position_in_mesh_decimal[1]);
			cloestMeshPoint = *(mesh_value + (position_in_mesh_integer[0] + m) + dim2 * (position_in_mesh_integer[1] + n));
			increment = cloestMeshPoint * weight;
			bicubic_answer += increment;
		}
	}
	return bicubic_answer;
}

/*
 * @ brief:
		确定给出节点在已知网格中的带小数坐标
		输入：待求值的点，插值区域网格点
		输出：带小数坐标
		如果给定点已经在列表中，返回整数坐标，flag赋值-1
 * @ change history:
		<date> | <discription>
		190416 | 更改错误处理，增加flag，给定值在列表
				 中时flag赋值-1，返回整数坐标

 */
float fFindPointPosition(float value, float* list, size_t len, int* flag) {
	float position = 0.F;
	int ierr = 0;
	int position_int = 0;
	flag = 0;
	position_int = fFindValuePositionInList(value, list, len, &ierr);
	if (-1 == ierr) return (float)position_int;
	position = (float)position_int + (value - list[position_int]) / (list[position_int + 1] - list[position_int]);
	return position;
}

/*
 * @ brief:
		在有序列表中查找值不大于给定数的最大元素的位置
		输入： 给定数，给定列表
		输出： 不大于给定数的最大元素索引的位置
		如果给定值已经在列表中，flag返回-1
 * @ change history:
		<date> | <discription>
		190415 | 更改返回值类型：float -> int
		190416 | 更改错误处理，增加flag，给定值在列表
				 中时直接返回-1改为flag返回-1

 */
int fFindValuePositionInList(float value, float* list, size_t len, int* flag) {
	unsigned int cnt = 0;
	*flag = 0;
	while (value > list[cnt] && cnt < len) ++cnt;
	if (value == list[cnt]) *flag = -1;
	return cnt - 1; // C数组下标从0开始，因此在数组中的索引应该减一
}

/*
 * @ brief:
		样条函数
		R(x) = (a+2)*abs(x)^3 - (a+3)*abs(x)^2 + 1 for abs(x) <= 1
			   a*abs(x)^3 - 5a*abs(x)^2 + 8a*abs(x) -4a for 1 < abs(x) < 2
			   0 for otherwise
		参数a通常取-0.5或-0.75。在头文件中有参数的宏定义。
		输入：x
		输出：R(x)
 * @ change history:
		<date> | <discription>

*/
inline float fSpline(float x) {
	float answer = 0.F;
	x = (x > 0.F) ? x : -x;
	if (x >= 0.F && x <= 1.F) {
		answer = (SPLINE_PARA + 2.F)*x*x*x - (SPLINE_PARA + 3.F)*x*x + 1;
	}
	else if (x > 1. && x <= 2.) {
		answer = SPLINE_PARA * x*x*x - 5 * SPLINE_PARA*x*x + 8 * SPLINE_PARA*x - 4 * SPLINE_PARA;
	}
	return answer;
}