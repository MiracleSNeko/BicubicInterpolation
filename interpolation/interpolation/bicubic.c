/***********************************************************************
 *
 * Bicubic Interpolation
 * ˫���β�ֵ��ʵ��
 * 
 * 
 * @ filename: bicubic.c
 * @ date: 2019-4-8
 * @ brief: 
		˫���β�ֵ�����漰16�����ص㣬ͨ����16�����ص����Ȩ�ؾ���õ��µ�����ֵ��
		���룺 excel�ļ�(.xls), ����ֵ�Ĳ�ֵ��(x, y)
		����� ��ֵ�����ֵ��ĺ���ֵf(x, y)
 * @ change history: 
  		<version> | <date> | <discription>

 *
 **********************************************************************/

#include <stdio.h>
#include <math.h>
#include "bicubic.h"

/*
 * @ brief:
		��ȡexcel�����������������С�mesh_x���鴢��x�������꣬mesh_y���鴢��y�������꣬
		mesh_value��ά���鴢���Ӧ(mesh_x, mesh_y)����ÿһ�����֪����ֵ�����ݶ�ȡ��excel
		���е����ݼ������ֵ�ڵ�Ĳ�ֵ�����
		���룺excel������ֵ�������꣬��ֵ��������(TRIANGLE or BELL or BSPLINE)
		�������ֵ���
   @ change history:
		<date> | <discription>

 */
ElemType Bicubic(char* filename, ElemType* coordinate, enum InterpolateType type) {
	ElemType *mesh_x, *mesh_y, *mesh_value;
	ElemType buff;
	int dim1, dim2;
	FILE *file;
	file = fopen(filename, 'r');
	fseek(file, 0, SEEK_SET);
	while()
}


/* 
 * @ brief:
		˫���β�ֵ�����������ڸ�����ĺ���ֵ���� 
		F(i', j') = sum_{m = -1}^{2} sum_{n = -1}^{2} F(i+m, j+n)R(m-dx)R(dy-n)
		���룺 ����ֵ�ĵ㣬��ֵ���������ͺ���ֵ��Ҫʹ�õĲ�ֵ����
			   type = { TRIANGEL: ���Էֲ�, BELL: Bell�ֲ�, BSP: B�������� }
		����� ����ֵ��ĺ���ֵ���ƻ�ȷֵ�����
 * @ change history:
		<date> | <discription>
		190416 | ���Ĵ���������ֵ���б���ʱֱ�Ӳ����ֵ
		190417 | ���ö�ٱ���type����ȷ��ʹ����һ�ֲ�ֵ����

 */
ElemType InterpolateKernal(ElemType point_x, ElemType point_y, ElemType *mesh_x, ElemType *mesh_y, 
						   ElemType *mesh_value, enum InterpolationType type) {
	ElemType* position_in_mesh;
	ElemType bicubic_answer = 0., position_in_mesh_decimal[2];
	int ierr, m, n, position_in_mesh_integer[2];
	ElemType(*ptrInterpolaionFuncion)(ElemType);
	size_t dim2 = sizeof(mesh_y) / sizeof(ElemType);
	position_in_mesh_integer[0] = floor(*position_in_mesh);
	position_in_mesh_integer[1] = floor(*(position_in_mesh + 1));
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
		for (n - 1; n < 3; ++n) {
			bicubic_answer += *(mesh_value + position_in_mesh_integer[0] + m + len_x * (position_in_mesh_integer[1] + n))
				* ptrInterpolaionFuncion(m - position_in_mesh_decimal[0]) * ptrInterpolaionFuncion(position_in_mesh_decimal[1] - n);
		}
	}
	return bicubic_answer;
}

/*
 * @ brief:
		ȷ�������ڵ�����֪�����еĴ�С������
		���룺����ֵ�ĵ㣬��ֵ���������
		�������С������
		����������Ѿ����б��У������������꣬flag��ֵ-1
 * @ change history:
		<date> | <discription>
		190416 | ���Ĵ���������flag������ֵ���б�
				 ��ʱflag��ֵ-1��������������

 */
ElemType* FindPointPosition(ElemType point_x, ElemType point_y, ElemType *mesh_x, 
							ElemType *mesh_y, int flag) {
	ElemType* position;
	int ierr_x, ierr_y;
	int position_int_x, position_int_y;
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
		�������б��в���ֵ�����ڸ����������Ԫ�ص�λ��
		���룺 �������������б�
		����� �����ڸ����������Ԫ��������λ��
		�������ֵ�Ѿ����б��У�flag����-1
 * @ change history:
		<date> | <discription>
		190415 | ���ķ���ֵ���ͣ�ElemType -> int
		190416 | ���Ĵ���������flag������ֵ���б�
				 ��ʱֱ�ӷ���-1��Ϊflag����-1

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
		���Էֲ�����
		R(x) = x+1, if(-1 <= x < 0); 
			   1-x, if(0 <= x < 1);
		���룺x
		�����R(x)
 * @ change history:
		<date> | <discription>

 */
inline ElemType TriangelInterpolation(ElemType x) {
	x /= 2.0;
	return (x < 0.) ? x + 1. : 1. - x;
}

/*
 * @ brief:
		Bell�ֲ�����
		R(x) = 0.5*(x+1.5)^2, if(-1.5 <= x <= -0.5); 
			   0.75-x^2, if(-0.5 < x <= 0.5);
			   0.5*(x-1.5)^2, if(0.5 < x <= 1.5);
		���룺x
		�����R(x)
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
		������������
		R(x) = 2/3+0.5*|x|^3-x^2, if(0 <= |x| <= 1);
			   (2-|x|)^3/6, if(1 < |x| <= 2)
		���룺x
		�����R(x)
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