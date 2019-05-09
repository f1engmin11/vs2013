#pragma once
typedef int ElementType;        //    定义数据类型,可根据需要进行其他类型定义
//    链表节点的定义
typedef struct ListNode {
	ElementType  Element;        //    数据域，存放数据
	ListNode* Next;        //    指向下一个链表节点
}Node, *PNode;
PNode CreateList(void);
void TraverseList(PNode List);
PNode FindList(PNode List);
void InsertList(PNode List, int pos, int val);
void DeleteList(PNode List, int pos);
//定义删除整个链表函数
void DeleteTheList(PNode List);
