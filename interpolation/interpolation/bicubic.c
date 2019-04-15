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
		����� ����ֵ��ĺ���ֵ����
		������ֵ���Ѿ����б��У�����NaN
 * @ change history:
		<date> | <discription>

 */
ElemType Bicubic(ElemType point_x, ElemType point_y, ElemType *mesh_x, ElemType *mesh_y, 
				 ElemType *mesh_value) {
	ElemType* position_in_mesh;
	position_in_mesh = FindPointPosition(point_x, point_y, mesh_x, mesh_y);
	if (NULL == position_in_mesh) return 0. / 0.;
	// ���㲿��
	free(position_in_mesh);
	position_in_mesh = NULL;
}

/*
 * @ brief:
		ȷ�������ڵ�����֪�����еĴ�С������
		���룺����ֵ�ĵ㣬��ֵ���������
		�������С������
		����������Ѿ����б��У����ؿ�ָ��
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
		�������б��в���ֵ�����ڸ����������Ԫ�ص�λ��
		���룺 �������������б�
		����� �����ڸ����������Ԫ��������λ��
		�������ֵ�Ѿ����б��У�����-1
 * @ change history:
		<date> | <discription>
		190415 | ���ķ���ֵ���ͣ�ElemType -> int

 */
int FindValuePositionInList(ElemType value, ElemType* list) {
	int cnt = 0;
	int len = sizeof(list) / sizeof(ElemType);
	while (value > list[cnt]) ++cnt;
	if (value == list[cnt]) cnt = -1;
	return cnt;
}
