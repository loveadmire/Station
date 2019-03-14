#include <stdio.h>
#include <stdlib.h>
#include "linklist.h"




int is_circularlinklist(LIST_NODE *phead)
{
	if(phead == NULL || phead->pnext == NULL){
		return 0;	
	}
	LIST_NODE *p1 = phead;
	LIST_NODE *p2 = phead;
	while(p1 != NULL && p2->pnext != NULL){
		p1 = p2->pnext;
		p2 = p2->pnext->pnext;
		if(p1 == p2)
			return 1;  //判断为环
		//p2 = p2->pnext;  //不是则下移
		//p1 = p1->pnext;
	}
	return 0;
}

LIST_NODE *find_midnode(LIST_NODE *phead)
{
	if(phead == NULL || phead->pnext == NULL){
		return phead;
	}

	LIST_NODE *p1 = phead;
	LIST_NODE *p2 = phead;
	while(p2->pnext != NULL){
		p1 = p1->pnext;//p1走一步
		p2 = p2->pnext;
		if(p2->pnext != NULL){
			p2 = p2->pnext;//p2走两步
		}
	}
	return p1;
}

LIST_NODE *merge_linklist_recursion(LIST_NODE *phead1, LIST_NODE *phead2)
{
	if(phead1 == NULL){
		return phead2;	
	}else if(phead2 == NULL){
		return phead1;	
	}	
	LIST_NODE *phead = NULL;
	if(phead1->data < phead2->data){
		phead = phead1;
		phead->pnext = merge_linklist_recursion(phead1->pnext, phead2);
	}else{
		phead = phead2;
		phead->pnext = merge_linklist_recursion(phead1, phead2->pnext);
	}
	return phead;
}


LIST_NODE *merge_linklist(LIST_NODE *phead1, LIST_NODE *phead2)
{
	if(phead1 == NULL){
		return phead2;	
	}else if(phead2 == NULL){
		return phead1;
	}
	
	LIST_NODE *phead = NULL;

	if(phead1->data < phead2->data){
		phead = phead1;
		phead1 = phead1->pnext;
	}else{
		phead = phead2;	
		phead2 = phead2->pnext;
	}//让phead找到最小的头节点

	LIST_NODE *pcur = phead;//要比较节点(phead1 或 phead2)的上一个节点
	while(phead1 != NULL && phead2 != NULL){
		if(phead1->data < phead2->data){
			pcur->pnext = phead1;
			pcur = phead1;
			phead1 = phead1->pnext;
		}else{
			pcur->pnext = phead2;	
			pcur = phead2;
			phead2 = phead2->pnext;
		}	
	}

	pcur->pnext = ((phead1 == NULL) ? phead2 : phead1);

	return phead;
}

LIST_NODE *invert_linklist_recursion(LIST_NODE *phead)
{
	if(phead == NULL || phead->pnext == NULL){
		return phead;	
	}else{
		LIST_NODE *p = phead;//保存头节点
		LIST_NODE *q = p->pnext;//保存下一个节点
		phead = invert_linklist_recursion(q);//递归到最后一个几点,返回转置后链表的地址
		q->pnext = p;//让后面的节点指向前一个节点
		p->pnext = NULL;//每次递归返回都赋值为空，最后一次返回将转置后的节点的pnext赋值为空
		return phead;
	}
}

LIST_NODE *invert_linklist(LIST_NODE *phead)
{
	if(phead == NULL || phead->pnext == NULL){
		return phead;
	}else{
		LIST_NODE *p = phead;
		LIST_NODE *q = NULL;
		LIST_NODE *r = NULL;
		while(p != NULL){
			q = p->pnext;//保存下一个节点
			p->pnext = r;//让该节点指向上一个节点
			r = p;//上一个节点走到当前节点
			p = q;//当前节点走到下一个节点
		}
		return r;
	}
}

int getlistlen_recursion(LIST_NODE *phead)
{
	if(phead == NULL)
		return 0;
	else
		return 1+getlistlen_recursion(phead->pnext);
}

int getlistlen(LIST_NODE *phead)
{
	int n = 0;

	while(phead != NULL){
		n++;
		phead = phead->pnext;
	}

	return n;
}

void destory_linklist(LIST_NODE *phead)
{
	LIST_NODE *ptmp;	
	while(phead != NULL){
		ptmp = phead;
		phead = phead->pnext;
		phead->data = 0;
		free(ptmp);
		ptmp = NULL;
	}
}

LIST_NODE *divide_two_Qsort(LIST_NODE *phead, LIST_NODE *ptail)
{
	int key = phead->data;
	LIST_NODE *p = phead;
	LIST_NODE *q = phead->pnext;
	
	while(q != ptail){
		if(q->data < key){
			p = p->pnext;
			int tmp;
			tmp = p->data;
			p->data = q->data;
			q->data = tmp;
		}	
		q = q->pnext;
	}
	int tmp = p->data;
	p->data = phead->data;
	phead->data = tmp;

	return p;
}

void quicksort_linklist(LIST_NODE *phead, LIST_NODE *ptail)
{
	if(phead != ptail){
		LIST_NODE *pmid = divide_two_Qsort(phead, ptail);	
		quicksort_linklist(phead, pmid);
		quicksort_linklist(pmid->pnext, ptail);
	}
}

void bubblesort_linklist(LIST_NODE *phead)
{
	LIST_NODE *p, *q;
	mytype tmp;
	
	for(p = phead; p != NULL; p=p->pnext){
		for(q = phead; q != NULL; q=q->pnext){
			if(p->data < q->data){
				tmp = p->data;
				p->data = q->data;
				q->data = tmp;
			}	
		}	
	}
}

LIST_NODE *change_node(LIST_NODE *phead, mytype val, mytype key)
{
	LIST_NODE *ptmp = search_node(phead, key);
	if(ptmp != NULL){
		ptmp->data = val;		
		return phead;
	}
	return NULL;
}

LIST_NODE *delete_node(LIST_NODE *phead, mytype key)
{
	LIST_NODE *ptmp1 = phead;
	LIST_NODE *ptmp2 = phead;
	LIST_NODE *ptmp;

	if(phead == NULL){
		printf("delete failed: linklist is tempty!\n");	
		return NULL;
	}else{
		while(ptmp1->data != key && ptmp1->pnext != NULL){
			ptmp2 = ptmp1;
			ptmp1 = ptmp1->pnext;
		}
		if(ptmp1->data == key){
			ptmp = ptmp1;
			ptmp2->pnext = ptmp1->pnext;
			free(ptmp1);
			ptmp1 = NULL;
			return (ptmp == phead ? ptmp2->pnext : phead);
		}
		if(ptmp1->pnext == NULL){
			printf("delete failed: key not found!!\n");
			return phead;
		}
	}
}

LIST_NODE *search_node(LIST_NODE *phead, mytype key)
{
	LIST_NODE *ptmp = phead;	

	if(phead == NULL){
		printf("search failed: linklist is empty!\n");
		return NULL;
	}else{
		while(ptmp->data != key && ptmp->pnext != NULL)
			ptmp = ptmp->pnext;
		if(ptmp->data == key){
			return ptmp;	
		}
		if(ptmp->pnext == NULL){
			printf("key not found!!!\n");	
			return NULL;
		}
	}
}


LIST_NODE *insert_node_pre(LIST_NODE *phead, mytype val, mytype key)
{
	LIST_NODE *pnode = init_node(pnode, val);	
	LIST_NODE *ptmp = phead;

	if(phead == NULL){
		return pnode;	
	}else if(phead->data == key){
		pnode->pnext = phead;
		return pnode;
	}else{
		while(ptmp->pnext != NULL && ptmp->pnext->data != key) 	
			ptmp = ptmp->pnext;
		if(ptmp->pnext == NULL){
			printf("insert fail:key not found!!!\n");
			free(pnode);
			pnode = NULL;
			return phead;
		}
		if(ptmp->pnext->data == key){
			pnode->pnext = ptmp->pnext;	
			ptmp->pnext = pnode;
			return phead;
		}
	}
}

LIST_NODE *insert_node_back(LIST_NODE *phead, mytype val, mytype key)
{
	LIST_NODE *pnode = init_node(pnode, val);
	LIST_NODE *ptmp = phead;

	if(phead == NULL){
		return pnode;
	}else{
		while(ptmp->data != key && ptmp->pnext != NULL){
			ptmp = ptmp->pnext;	
		}	
		if(ptmp == NULL){
			printf("insert fail:key not found!!!\n");
			free(pnode);
			pnode = NULL;
			return phead;
		}
		if(ptmp->data == key){
			pnode->pnext = ptmp->pnext;
			ptmp->pnext = pnode;
			return phead;
		}
	}
}

LIST_NODE *add_node_pre(LIST_NODE *phead, mytype val)
{
	LIST_NODE *pnode = init_node(pnode, val);	
	LIST_NODE *ptmp = phead;

	if(phead == NULL){
		return pnode;	
	}else{
		phead = pnode;
		pnode->pnext = ptmp;
		return phead;
	}
}


void show_linklist(LIST_NODE *phead)
{
	if(phead == NULL){
		return;	
	}else{
		printf("%d\t", phead->data);	
		show_linklist(phead->pnext);
	}
}

void revshow_linklist(LIST_NODE *phead)
{
	if(phead == NULL){
		return;	
	}else{
		revshow_linklist(phead->pnext);	
		printf("%d\t", phead->data);
	}
}

LIST_NODE *init_node(LIST_NODE *phead, mytype val)
{
	phead = (LIST_NODE *)malloc(sizeof(LIST_NODE));
	phead->data = val;
	phead->pnext = NULL;

	return phead;
}

LIST_NODE *add_node(LIST_NODE *phead, mytype val)
{
	LIST_NODE *pnode = init_node(pnode, val);	
	LIST_NODE *ptmp = phead;

	if(phead == NULL){
		return pnode;	
	}else{
		while(ptmp->pnext != NULL)	
			ptmp = ptmp->pnext;
		ptmp->pnext = pnode;
		return phead;
	}
}


int Signal_list_test()
{
	LIST_NODE *phead = NULL;	
	LIST_NODE *pfind = NULL;
	phead = add_node(phead, 1);
	phead = add_node(phead, 3);
	phead = add_node(phead, 5);
	phead = add_node(phead, 7);
	phead = add_node(phead, 9);
	phead = add_node(phead, 11);

	LIST_NODE *phead2 = NULL;
	phead2 = add_node(phead2, 2);
	phead2 = add_node(phead2, 4);
	phead2 = add_node(phead2, 6);
	phead2 = add_node(phead2, 8);
	phead2 = add_node(phead2, 10);
	phead2 = add_node(phead2, 12);

	LIST_NODE *NEW_HEAD = merge_linklist(phead,phead2);
	show_linklist(NEW_HEAD);
	printf("\n");

	show_linklist(phead);
	printf(" \n");

	pfind = search_node(phead, 8);
	if (pfind){
		phead = insert_node_pre(phead, 7, 8);
		phead = insert_node_back(phead,9, 8);
	}
	else{
		phead = add_node(phead,13);
	}
	show_linklist(phead);
	printf(" \n");
	phead = delete_node(phead, 3);
	show_linklist(phead);
	printf("\n");
	phead = change_node(phead, 10, 4);
	show_linklist(phead);
	bubblesort_linklist(phead);
	quicksort_linklist(phead, NULL);
	printf(" \nlen %d\n", getlistlen(phead));
	show_linklist(phead);
	printf("\n");
	printf("is_circularlinklist %d\n", is_circularlinklist(phead));
	phead = invert_linklist(phead);
	show_linklist(phead);
	printf("\n");
	destory_linklist(phead);
	return 0;
}
