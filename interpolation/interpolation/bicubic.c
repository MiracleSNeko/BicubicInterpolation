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
 * @ change history:
		<date> | <discription>

 */
ElemType Bicubic(ElemType point_x, ElemType point_y, ElemType *mesh_x, ElemType *mesh_y, 
				 ElemType *mesh_value) {


}

/*
 * @ brief:
		ȷ�������ڵ�����֪�����еĴ�С������
		���룺����ֵ�ĵ㣬��ֵ���������
		�������С������
 * @ change history:
		<date> | <discription>

 */
ElemType* FindPointPosition(ElemType point_x, ElemType point_y, ElemType *mesh_x, 
							ElemType *mesh_y) {
	ElemType* position;
	position = (ElemType *)malloc(sizeof(ElemType) * 2);
	*position = FindValueInList(point_x, mesh_x);
	*(position + 1) = FindValueInList(point_y, mesh_y);
}

/*
 * @ brief:
		�������б��в���ֵ�����ڸ����������Ԫ�ص�λ��
		���룺 �������������б�
		����� �����ڸ����������Ԫ��������λ��
		�������ֵ�Ѿ����б��У�����-1
 * @ change history:
		<date> | <discription>
		190415 | ���ķ���ֵ���ͣ�int -> ElemType

 */
ElemType FindValueInList(ElemType value, ElemType* list) {
	int cnt = 0;

}
