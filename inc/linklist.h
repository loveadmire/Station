//
// Created by root on 1/15/19.
//

#ifndef STATION_LINKLIST_H
#define STATION_LINKLIST_H

/*data_struct
 * linklist
 * operation:创建链表（前插、后插）、插入节点（前插、后插）、查找节点、删除节点、更改节点、
 * 释放链表、打印链表、翻转打印链表、冒泡排序、快速排序、求链表长度（递归和非递归）、
 * 反转链表（递归和非递归）、合并有序链表（递归和非递归）、查找中间节点、判断是否有环、
 */

typedef int mytype;

typedef struct link_node{
    mytype data;
    struct link_node *pnext;
}LIST_NODE,*PLIST_NODE;

LIST_NODE *init_node(LIST_NODE *phead, mytype val);
LIST_NODE *add_node_back(LIST_NODE *phead, mytype val);
LIST_NODE *add_node_pre(LIST_NODE *phead, mytype val);
LIST_NODE *insert_node_back(LIST_NODE *phead, mytype val, mytype key);
LIST_NODE *insert_node_pre(LIST_NODE *phead, mytype val, mytype key);
LIST_NODE *search_node(LIST_NODE *phead, mytype key);
LIST_NODE *delete_node(LIST_NODE *phead, mytype key);
LIST_NODE *change_node(LIST_NODE *phead, mytype val, mytype key);
void destory_linklist(LIST_NODE *phead);
void show_linklist(LIST_NODE *phead);
void revshow_linklist(LIST_NODE *phead);
void bubblesort_linklist(LIST_NODE *phead);
void quicksort_linklist(LIST_NODE *phead, LIST_NODE *ptail);
LIST_NODE *divide_two_Qsort(LIST_NODE *phead, LIST_NODE *ptail);
int getlistlen(LIST_NODE *phead);
int getlistlen_recursion(LIST_NODE *phead);
LIST_NODE *invert_linklist(LIST_NODE *phead);
LIST_NODE *invert_linklist_recursion(LIST_NODE *phead);
LIST_NODE *merge_linklist(LIST_NODE *phead1, LIST_NODE *phead2);
LIST_NODE *merge_linklist_recursion(LIST_NODE *phead1, LIST_NODE *phead2);
LIST_NODE *find_midnode(LIST_NODE *phead);
int is_circularlinklist(LIST_NODE *phead);

int Signal_list_test();

#endif //STATION_LINKLIST_H
