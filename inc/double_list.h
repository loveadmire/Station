#ifndef DOUBLE_LIST
#define DOUBLE_LIST
#include <stdio.h>
#include <malloc.h>
#include <assert.h>

#define swp(x,y) {x = x+y;y = x-y,x = x-y}

//typedef char* ElemType;
typedef int ElemType;

typedef struct _Node{
    ElemType data;
    struct _Node *pPrio;
    struct _Node *pNext;
}Node,*pNode;

typedef struct List{
    pNode first;
    pNode last;
    size_t size;
}List,*pList;
void InitDlist(List *list);//初始化双链表
void Insert_in_tail(List *list, ElemType x);//在双链表的末尾插入元素
void Insert_in_head(List *list, ElemType x);//在双链表的头部插入元素
void Show_list(List *list);//打印双链表
void Delete_tail(List *list);//删除双链表的最后一个元素
void Delete_head(List *list);//删除双链表的第一个元素
void Insert_node(List *list, ElemType x);//将数据元素插入到双链表中（要求此时双链表中的数据元素顺序排列）
Node* Find_value(List *list, ElemType x);//查找双链表中数据值为x的结点
int  Get_dlist_length(List *list);//求双链表的长度
void Delete_val(List *list, ElemType x);//按值删除双链表中的某个数据元素
void Sort_list(List *list);//对双链表进行排序
void Reverse_list(List *list);//逆置双链表
void Clear_list(List *list);//清除双链表
void Destory_list(List *list);//摧毁双链表
//优化
pNode Buy_node(ElemType x);//创建结点

void testdlist();

#endif // DOUBLE_LIST

