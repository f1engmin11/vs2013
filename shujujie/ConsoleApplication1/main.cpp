#include<stdio.h>
#include<malloc.h>
#include<stdlib.h>
#include "List.h"
//    ������ 
int main() {
	PNode List = CreateList();    //����һ��ָ�룬ʹ��ָ���´����������ͷָ��    
	TraverseList(List);//����
	InsertList(List, 2, 22);
	TraverseList(List);//����
	DeleteList(List, 3);
	TraverseList(List);//����
	FindList(List);        //    �����ѯ  
	DeleteTheList(List);    //    ɾ����������
	TraverseList(List);
	return 0;
}