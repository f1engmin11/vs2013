#pragma once

#include <stdlib.h>
#include <stdio.h>

#define EmptyTOS    -1  //��ջ
#define MinStackSize    5   //ջ����Сֵ

typedef int elementType;    //��������

typedef struct stackRecord
{
	int capacity;//����
	int stackTop;//ջ��
	elementType *array;
}STACK;

int isEmpty(STACK *s);  //�п�
int isFull(STACK *s);   //����
STACK *createStack(int maxElements);    //ջ�Ĵ���
void disposeStack(STACK *s);        //ջ���ͷ�
void makeEmpty(STACK *s);       //������ջ
void push(elementType element, STACK *s);//��ջ
elementType top(STACK *s);      //����ջ��Ԫ��
void pop(STACK *s);         //��ջ
elementType topAndTop(STACK *s);    //��ջ������ջ��Ԫ��