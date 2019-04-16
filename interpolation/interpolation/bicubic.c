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
  		<date> | <discription>	

 *
 **********************************************************************/

#include <stdio.h>

// ͨ������floatΪdouble��߾���
typedef float ElemType;

/* 
 * @ brief:
		˫���β�ֵ�����������ڸ�����ĺ���ֵ���� 
		���룺 ����ֵ�ĵ㣬��ֵ���������ͺ���ֵ
		����� ����ֵ��ĺ���ֵ���ƻ�ȷֵ�����
 * @ change history:
		<date> | <discription>
		190416 | ���Ĵ���������ֵ���б���ʱֱ�Ӳ����ֵ

 */
ElemType Bicubic(ElemType point_x, ElemType point_y, ElemType *mesh_x, ElemType *mesh_y, 
				 ElemType *mesh_value) {
	ElemType* position_in_mesh;
	int ierr;
	size_t len_x = sizeof(mesh_x) / sizeof(ElemType);
	position_in_mesh = FindPointPosition(point_x, point_y, mesh_x, mesh_y, ierr);
	if (-1 == ierr) return *(mesh_value + (int)position_in_mesh[0] + len_x*(int)position_in_mesh[1]);
	// ���㲿��
	free(position_in_mesh);
	position_in_mesh = NULL;
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
							ElemType *mesh_y,  int flag) {
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
