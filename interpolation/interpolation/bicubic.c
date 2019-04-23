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
		<date> | <ver> | <discription>
		190408 | 0.1.0 | 初始化
		190423 | 0.2.0 | 完成全部程序

 *
 **********************************************************************/
#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "bicubic.h"

/*
 * @ brief: 
		用于测试的main函数，实际使用时注释掉即可
		测试方式：命令行参数/控制台输入
		输入：文件名(char *)， 横坐标x（ElemType），纵坐标y（ElemType），插值函数类型（Enum/Int）
		插值函数类型：1-线性分布，2-Bell分布，3-三次样条插值
 */
int main(int args, char* argv[]) {
	char filename[20] = "";
	enum InterpolationType type;
	ElemType* coordinate = (ElemType *)malloc(2 * sizeof(ElemType));
	ElemType answer;
	if (args >= 4) {
		*filename = *(argv[1]);
		*coordinate = *(argv[2]);
		*(coordinate + 1) = *(argv[3]);
		type = *(argv[4]);
	}
	else {
		printf("输入格式：\n文件名\n横坐标,纵坐标\n插值函数类型\n");
		scanf_s("%s", filename, 20);
		if (sizeof(float) == sizeof(ElemType)) scanf("%f,%f", coordinate, coordinate + 1);
		else if (sizeof(double) == sizeof(ElemType)) scanf("%lf,%lf", coordinate, coordinate + 1);
		scanf("%d", &type);
	}
	answer = Bicubic(filename, coordinate, type);
	if (sizeof(float) == sizeof(ElemType)) printf("插值结果为%f", answer);
	else if (sizeof(double) == sizeof(ElemType)) printf("插值结果为%lf", answer);
	return 0;
}

/*
 * @ brief:
		读取excel表并储存至三个数组中。mesh_x数组储存x方向坐标，mesh_y数组储存y方向坐标，
		mesh_value二维数组储存对应(mesh_x, mesh_y)网格每一点的已知函数值。根据读取的excel
		表中的数据计算待求值节点的插值结果。
		输入：excel表，待插值结点的坐标，插值函数类型(TRIANGLE or BELL or BSPLINE)
		输出：插值结果
   @ change history:
		<date> | <discription>

 */
ElemType Bicubic(char* filename, ElemType* coordinate, enum InterpolationType type) {
	ElemType *mesh_x = NULL, *mesh_y = NULL, *mesh_value = NULL;
	ElemType buff = 0., bicubic_answer = 0.;
	int dim1 = 1, dim2 = 1, i = 0;
	FILE *file = NULL;
	const char* filename_c = filename;
	//file = fopen(filename_c, 'r');
	fopen_s(file, filename_c, 'r');
	fseek(file, 1L, SEEK_SET);
	if (sizeof(float) == sizeof(ElemType)) {
		while (-1 != fscanf(file, "%f", &buff)) {
			++dim2;
			fseek(file, 1L, SEEK_CUR);
		}
		fseek(file, 1L * (dim2 + 1), SEEK_SET);
		while (-1 != fscanf(file, "%f", &buff)) {
			++dim1;
			fseek(file, 1L * (dim2 + 1), SEEK_CUR);
		}
	}
	else if (sizeof(double) == sizeof(ElemType)) {
		while (-1 != fscanf(file, "%lf", &buff)) {
			++dim2;
			fseek(file, 1L, SEEK_CUR);
		}
		fseek(file, 1L * (dim2 + 1), SEEK_SET);
		while (-1 != fscanf(file, "%lf", &buff)) {
			++dim1;
			fseek(file, 1L * (dim2 + 1), SEEK_CUR);
		}
	}
	mesh_x = (ElemType *)malloc(dim1 * sizeof(ElemType));
	mesh_y = (ElemType *)malloc(dim2 * sizeof(ElemType));
	mesh_value = (ElemType *)malloc(dim1*dim2 * sizeof(ElemType));
	fseek(file, 1L, SEEK_SET);
	fread(mesh_y, sizeof(ElemType), dim2, file);
	fseek(file, 1L * (dim2 + 1), SEEK_SET);
	while (i < dim1 && 0 != fread(mesh_x + i, sizeof(ElemType), 1, file)) {
		fseek(file, 1L * (dim2 + 1), SEEK_CUR);
		++i;
	}
	i = 0;
	fseek(file, 1L * (dim2 + 1) + 1L, SEEK_SET);
	while (i < dim1 && 0 != fread(mesh_value + i * dim2, sizeof(ElemType), dim2, file)) {
		fseek(file, 1L * (dim2 + 1), SEEK_CUR);
	}
	fclose(file);
	/*
	 * 以下部分用于测试Excel是否正确读取
	 */
	file = fopen("mesh_x.txt", 'w');
	fwrite(mesh_x, sizeof(ElemType), dim1, file);
	fclose(file);
	file = fopen("mesh_y.txt", 'w');
	fwrite(mesh_y, sizeof(ElemType), dim2, file);
	fclose(file);
	file = fopen("mesh_value.txt", 'w');
	fwrite(mesh_value, sizeof(ElemType), dim1*dim2, file);
	fclose(file);
	 //*/
	bicubic_answer = InterpolateKernal(*coordinate, *(coordinate + 1), mesh_x, mesh_y,
		mesh_value,	type);
	free(mesh_x);
	free(mesh_y);
	free(mesh_value);
	mesh_x = NULL;
	mesh_y = NULL;
	mesh_value = NULL;
	return bicubic_answer;
}


/* 
 * @ brief:
		双三次插值函数，返回在给定点的函数值估计 
		F(i', j') = sum_{m = -1}^{2} sum_{n = -1}^{2} F(i+m, j+n)R(m-dx)R(dy-n)
		输入： 待求值的点，插值区域网格点和函数值，要使用的插值函数
			   type = { TRIANGEL: 线性分布, BELL: Bell分布, BSP: B样条曲线 }
		输出： 待求值点的函数值估计或精确值（查表）
 * @ change history:
		<date> | <discription>
		190416 | 更改错误处理，给定值在列表中时直接查表返回值
		190417 | 添加枚举变量type用于确定使用哪一种插值函数

 */
ElemType InterpolateKernal(ElemType point_x, ElemType point_y, ElemType *mesh_x, ElemType *mesh_y, 
						   ElemType *mesh_value, enum InterpolationType type) {
	ElemType* position_in_mesh = NULL;
	ElemType bicubic_answer = 0., position_in_mesh_decimal[2] = { 0, 0 };
	int ierr = 0, m = 0, n = 0, position_in_mesh_integer[2] = { 0, 0 };
	ElemType(*ptrInterpolaionFuncion)(ElemType) = NULL;
	size_t dim2 = sizeof(mesh_y) / sizeof(ElemType);
	position_in_mesh_integer[0] = (int)floor(*position_in_mesh);
	position_in_mesh_integer[1] = (int)floor(*(position_in_mesh + 1));
	position_in_mesh_decimal[0] = *position_in_mesh - (ElemType)position_in_mesh_integer[0];
	position_in_mesh_decimal[1] = *(position_in_mesh + 1) - (ElemType)position_in_mesh_integer[1];
	free(position_in_mesh);
	position_in_mesh = NULL;
	position_in_mesh = FindPointPosition(point_x, point_y, mesh_x, mesh_y, ierr);
	if (-1 == ierr) {
		bicubic_answer = *(mesh_value + position_in_mesh_integer[0] + dim2 * position_in_mesh_integer[1]);
		return bicubic_answer;
	}
	switch (type) {
	case TRIANGEL:
		ptrInterpolaionFuncion = TriangelInterpolation;
		break;
	case BELL:
		ptrInterpolaionFuncion = BellInterpolation;
		break;
	case BSPLINE:
		ptrInterpolaionFuncion = BsplineInterpolation;
		break;
	default:
		break;
	}
	for (m = -1; m < 3; ++m) {
		for (n = -1; n < 3; ++n) {
			bicubic_answer += *(mesh_value + position_in_mesh_integer[0] + m + dim2 * (position_in_mesh_integer[1] + n))
				* ptrInterpolaionFuncion(m - position_in_mesh_decimal[0]) 
				* ptrInterpolaionFuncion(position_in_mesh_decimal[1] - n);
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
ElemType* FindPointPosition(ElemType point_x, ElemType point_y, ElemType *mesh_x, 
							ElemType *mesh_y, int flag) {
	ElemType* position = NULL;
	int ierr_x = 0, ierr_y = 0;
	int position_int_x = 0, position_int_y = 0;
	flag = 0;
	position_int_x = FindValuePositionInList(point_x, mesh_x, ierr_x);
	position_int_y = FindValuePositionInList(point_y, mesh_y, ierr_y);
	position = (ElemType *)malloc(sizeof(ElemType) * 2);
	position[0] = (ElemType)position_int_x;
	position[1] = (ElemType)position_int_y;
	if (-1 == ierr_x && -1 == ierr_y) {
		flag = -1;
		return position;
	}
	position[0] +=  point_x / (mesh_x[position_int_x] + mesh_x[position_int_x + 1]);
	position[1] +=  point_y / (mesh_y[position_int_y] + mesh_y[position_int_y + 1]);
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
		190415 | 更改返回值类型：ElemType -> int
		190416 | 更改错误处理，增加flag，给定值在列表
				 中时直接返回-1改为flag返回-1

 */
int FindValuePositionInList(ElemType value, ElemType* list, int flag) {
	int cnt = 0;
	flag = 0;
	size_t len = sizeof(list) / sizeof(ElemType);
	while (value > list[cnt]) ++cnt;
	if (value == list[cnt]) flag = -1;
	return cnt;
}

/*
 * @ brief:
		线性分布函数
		R(x) = x+1, if(-1 <= x < 0); 
			   1-x, if(0 <= x < 1);
		输入：x
		输出：R(x)
 * @ change history:
		<date> | <discription>

 */
inline ElemType TriangelInterpolation(ElemType x) {
	x /= 2.0;
	return (x < 0.) ? x + 1. : 1. - x;
}

/*
 * @ brief:
		Bell分布函数
		R(x) = 0.5*(x+1.5)^2, if(-1.5 <= x <= -0.5); 
			   0.75-x^2, if(-0.5 < x <= 0.5);
			   0.5*(x-1.5)^2, if(0.5 < x <= 1.5);
		输入：x
		输出：R(x)
 * @ change history:
		<date> | <discription>

*/
inline ElemType BellInterpolation(ElemType x) {
	x = x / 2. + 1.5;
	if (x > -1.5 && x < -0.5) {
		return 0.5*pow(x + 1.5, 2.);
	}
	else if (x > -0.5 && x < 0.5) {
		return 3. / 4. - pow(x, 2.);
	}
	else if (x > 0.5 && x < 1.5) {
		return 0.5*pow(x - 1.5, 2.);
	}
	return 0.;
}

/*
 * @ brief:
		三次样条函数
		R(x) = 2/3+0.5*|x|^3-x^2, if(0 <= |x| <= 1);
			   (2-|x|)^3/6, if(1 < |x| <= 2)
		输入：x
		输出：R(x)
 * @ change history:
		<date> | <discription>

*/
inline ElemType BsplineInterpolation(ElemType x) {
	x = (x > 0) ? x : -x;
	if (x >= 0. && x <= 1.) {
		return 1.5 + 0.5*pow(x, 3.) - x*x;
	}
	else if (x > 1. && x <= 2.) {
		return 1. / 6.*pow(2. - x, 3.);
	}
	return 1.;
}