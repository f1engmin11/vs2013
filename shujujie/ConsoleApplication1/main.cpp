#include<stdio.h>
#include<malloc.h>
#include<stdlib.h>
#include "List.h"
//    主函数 
int main() {
	PNode List = CreateList();    //创建一个指针，使其指向新创建的链表的头指针    
	TraverseList(List);//遍历
	InsertList(List, 2, 22);
	TraverseList(List);//遍历
	DeleteList(List, 3);
	TraverseList(List);//遍历
	FindList(List);        //    链表查询  
	DeleteTheList(List);    //    删除整个链表
	TraverseList(List);
	return 0;
}