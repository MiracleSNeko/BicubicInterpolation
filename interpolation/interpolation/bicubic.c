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
		���룺 �ļ���(.txt), ����ֵ�Ĳ�ֵ��(x, y)
		����� ��ֵ�����ֵ��ĺ���ֵf(x, y)
 * @ change history: 
		<date> | <ver> | <discription>
		190408 | 0.1.0 | ��ʼ��
		190423 | 0.2.0 | ���ȫ������
		190424 | 0.3.0 | �޸�ϸ�ڴ���
						 ��Ϊ��ȡ����txt�ļ�

 *
 **********************************************************************/
#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "bicubic.h"

/*
 * @ brief: 
		���ڲ��Ե�main������ʵ��ʹ��ʱע�͵�����
		���Է�ʽ�������в���/����̨����
		���룺�ļ���(char *)�� ����ά�ȣ�int, int����������x��ElemType����������y��ElemType������ֵ�������ͣ�Enum/Int��
		��ֵ�������ͣ�1-���Էֲ���2-Bell�ֲ���3-����������ֵ
 */
int main(int args, char* argv[]) {
	char filename_x[20] = "";
	char filename_y[20] = "";
	char filename_mesh[20] = "";
	enum InterpolationType type = 1;
	int dim1 = 0, dim2 = 0;
	ElemType* coordinate = (ElemType *)malloc(2 * sizeof(ElemType));
	ElemType answer = 0.;
	if (args >= 8) {
		*filename_x = *(argv[1]);
		*filename_y = *(argv[2]);
		*filename_mesh = *(argv[3]);
		dim1 = *(argv[4]);
		dim2 = *(argv[5]);
		*coordinate = *(argv[6]);
		*(coordinate + 1) = *(argv[7]);
		type = *(argv[8]);
	}
	else {
		printf("�����ʽ��\n�ļ���(x,y,mesh)\n����ά��(int ,int)\n������,������\n��ֵ��������\n");
		scanf_s("%s", filename_x, 20);
		scanf_s("%s", filename_y, 20);
		scanf_s("%s", filename_mesh, 20);
		scanf("%d,%d", &dim1, &dim2);
		if (sizeof(float) == sizeof(ElemType)) scanf("%f,%f", coordinate, coordinate + 1);
		else if (sizeof(double) == sizeof(ElemType)) scanf("%lf,%lf", coordinate, coordinate + 1);
		scanf("%d", &type);
	}
	answer = Bicubic(filename_x, filename_y, filename_mesh, dim1, dim2, coordinate, type);
	if (sizeof(float) == sizeof(ElemType)) printf("��ֵ���Ϊ%f", answer);
	else if (sizeof(double) == sizeof(ElemType)) printf("��ֵ���Ϊ%lf", answer);
	system("pause");
	return 0;
}

/*
 * @ brief:
		��ȡ�ļ������������������С�mesh_x���鴢��x�������꣬mesh_y���鴢��y�������꣬
		mesh_value��ά���鴢���Ӧ(mesh_x, mesh_y)����ÿһ�����֪����ֵ�����ݶ�ȡ��excel
		���е����ݼ������ֵ�ڵ�Ĳ�ֵ�����
		���룺����x���ꡢy���ꡢ��㴦����ֵ�������ļ�������ֵ�������꣬��ֵ��������(TRIANGLE or BELL or BSPLINE)
		�������ֵ���
   @ change history:
		<date> | <discription>
		190424 | ����ȡһ��excel�ļ�ǿ�Ƹ�Ϊ��ȡ�����ֿ����ļ�
		190424 | ǿ��Ҫ���ṩ�����ά��

 */
ElemType Bicubic(char* filename_x, char* filename_y, char* filename_mesh, int dim1, int dim2,
				 ElemType* coordinate, enum InterpolationType type) {
	ElemType *mesh_x = NULL, *mesh_y = NULL, *mesh_value = NULL;
	ElemType buff = 0., bicubic_answer = 0.;
	int i = 0;
	FILE *file_x = NULL, *file_y = NULL, *file_mesh = NULL, *file = NULL;
	const char* C_filename_x = filename_x;
	const char* C_filename_y = filename_y;
	const char* C_filename_mesh = filename_mesh;
	fopen_s(&file_x, C_filename_x, "r");
	fseek(file_x, 0L, SEEK_SET);
	mesh_x = (ElemType *)malloc(dim1 * sizeof(ElemType));
	fread(mesh_x, sizeof(ElemType), dim1, file_x);
	fclose(file_x);
	file_x = NULL;
	fopen_s(&file_y, C_filename_y, "r");
	fseek(file_y, 0L, SEEK_SET);
	mesh_y = (ElemType *)malloc(dim2 * sizeof(ElemType));
	fread(mesh_y, sizeof(ElemType), dim2, file_y);
	fclose(file_y);
	file_y = NULL;
	fopen_s(&file_mesh, C_filename_mesh, "r");
	fseek(file_mesh, 0L, SEEK_SET);
	mesh_value = (ElemType *)malloc(dim1 * dim2 * sizeof(ElemType));
	fread(mesh_value, sizeof(ElemType), dim1 * dim2, file_mesh);
	fclose(file_mesh);
	file_mesh = NULL;
	/*
	 * ���²������ڲ����ļ��Ƿ���ȷ��ȡ
	 */
	fopen_s(&file, "mesh_x.txt", "w");
	fwrite(mesh_x, sizeof(ElemType), dim1, file);
	fclose(file);
	fopen_s(&file, "mesh_y.txt", "w");
	fwrite(mesh_y, sizeof(ElemType), dim2, file);
	fclose(file);
	fopen_s(&file, "mesh_value.txt", "w");
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
	ElemType* position_in_mesh = NULL;
	ElemType bicubic_answer = 0., position_in_mesh_decimal[2] = { 0, 0 };
	int ierr = 0, m = 0, n = 0, position_in_mesh_integer[2] = { 0, 0 };
	ElemType(*ptrInterpolaionFuncion)(ElemType) = NULL;
	size_t dim2 = sizeof(mesh_y) / sizeof(ElemType);
	position_in_mesh = FindPointPosition(point_x, point_y, mesh_x, mesh_y, ierr);
	position_in_mesh_integer[0] = (int)floor(*position_in_mesh);
	position_in_mesh_integer[1] = (int)floor(*(position_in_mesh + 1));
	position_in_mesh_decimal[0] = *position_in_mesh - (ElemType)position_in_mesh_integer[0];
	position_in_mesh_decimal[1] = *(position_in_mesh + 1) - (ElemType)position_in_mesh_integer[1];
	free(position_in_mesh);
	position_in_mesh = NULL;
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