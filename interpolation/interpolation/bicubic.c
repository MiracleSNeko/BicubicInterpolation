/***********************************************************************
 *
 * Bicubic Interpolation
 * 双三次插值的实现
 * 
 * @ filename: bicubic.c
 * @ date: 2019-4-8
 * @ brief: 
		输入： excel文件(.xls), 待求值的插值点(x, y)
		输出： 插值后待求值点的函数值f(x, y)
 * @ change history: 
  		<date> | <discription>	

 *
 **********************************************************************/

#include <stdio.h>

// 通过更改float为double提高精度
typedef float ElemType;

int main(int args, char *argv[]) {

}

/* 
 * @ brief:
		双三次插值函数，返回在给定点的函数值估计 
		输入： 待求值的点，插值区域网格点和函数值
		输出： 待求值点的函数值估计
 * @ change history:
		<date> | <discription>

 */
ElemType Bicubic(ElemType point_x, ElemType point_y, ElemType *mesh_x, ElemType *mesh_y, 
				 ElemType *mesh_value) {

}
