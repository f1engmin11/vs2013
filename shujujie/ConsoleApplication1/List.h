#pragma once
typedef int ElementType;        //    ������������,�ɸ�����Ҫ�����������Ͷ���
//    ����ڵ�Ķ���
typedef struct ListNode {
	ElementType  Element;        //    �����򣬴������
	ListNode* Next;        //    ָ����һ������ڵ�
}Node, *PNode;
PNode CreateList(void);
void TraverseList(PNode List);
PNode FindList(PNode List);
void InsertList(PNode List, int pos, int val);
void DeleteList(PNode List, int pos);
//����ɾ������������
void DeleteTheList(PNode List);
