#include "double_list.h"

pNode Buy_node(ElemType x){
    pNode s = (pNode)malloc(sizeof(pNode));
    assert(s != NULL);
    s->data = x;
    s->pPrio = s->pNext = NULL;
    return s;
}
void InitDlist(List *list){
    Node *s = (Node*)malloc(sizeof(Node));
    assert(s != NULL);
    list->first = list->last = s;
    list->first->pPrio = NULL;
    list->last->pNext = NULL;
    list->size = 0;
}
void Insert_in_tail(List *list, ElemType x){
    Node *s = Buy_node(x);
    s->pPrio = list->last;
    list->last->pNext = s;
    list->last = s;
    list->size++;
}

void Insert_in_head(List *list, ElemType x){
    Node *s = Buy_node(x);
    if(list->first == list->last){
        s->pPrio=list->first;
        list->first->pNext = s;
        list->last = s;
    }
    else{
        s->pNext = list->first->pNext;
        s->pNext->pPrio = s;
        s->pPrio = list->first;
        list->first->pNext = s;
    }
    list->size++;
}

void Show_list(List *list){
    Node *p = NULL;
    p = list->first->pNext;
    while (p != NULL)
    {
        printf(" %d ",p->data);
        p = p->pNext;
    }
    printf("\n");



#if 0
    Node *p = list->first->pNext;
    while(p != NULL)
    {
        printf("%d ->",p->data);
    }
    p = p->pNext;
#endif
}

void Delete_tail(List *list){
    if (list->size == 0) return;
      Node *p = list->first;
      while (p->pNext != list->last)
        p = p->pNext;
      free(list->last);
      list->last = p;
      list->last->pNext = NULL;
      list->size--;
}

void Delete_head(List *list){
    if (list->size == 0) return;
     Node *p = list->first->pNext;
     if (list->first->pNext == list->last) {
       list->last = list->first;
       list->last->pNext = NULL;
     }
     else {
       list->first->pNext = p->pNext;
       p->pNext->pPrio = list->first;
     }
     free(p);
     list->size--;
}

void Insert_node(List *list, ElemType x){
    Node *p = list->first;
      while (p->pNext != NULL && p->pNext->data < x)
        p = p->pNext;
      if (p->pNext == NULL)
        Insert_in_tail(list, x);
      else {
        Node *s = Buy_node(x);
        s->pNext = p->pNext;
        s->pNext->pPrio= s;
        s->pPrio = p;
        p->pNext = s;
        list->size++;
      }
}

Node *Find_value(List *list, ElemType x){
    Node *p = list->first->pNext;
      while (p!=NULL && p->data != x)
        p = p->pNext;
      return p;
    }
    int length(List *list) {
      return list->size;
}

int  Get_dlist_length(List *list)
{
    return list->size;
}


void Delete_val(List *list, ElemType x){
    if (list->size == 0) return;
      Node *p = Find_value(list, x);
      if (p == NULL) {
        printf("要删除的数据不存在！\n");
        return;
      }
      if (p == list->last) {
        list->last = p->pPrio;
        list->last->pNext = NULL;
      }
      else {
        p->pNext->pPrio = p->pPrio;
        p->pPrio->pNext = p->pNext;
      }
      free(p);
      list->size--;
}


void Sort_list(List *list){
    if (list->size == 0 || list->size == 1) return;
      Node *s = list->first->pNext;
      Node *q = s->pNext;
      list->last = s;
      list->last->pNext = NULL;
      while (q != NULL) {
        s = q;
        q = q->pNext;
        Node *p = list->first;
        while (p->pNext != NULL && p->pNext->data < s->data)
          p = p->pNext;
        if (p->pNext == NULL) {
          s->pNext = NULL;
          s->pPrio = list->last;
          list->last->pNext = s;
          list->last = s;
        }
        else {
          s->pNext = p->pNext;
          s->pNext->pPrio = s;
          s->pPrio = p;
          p->pNext = s;
        }
      }
}

void Reverse_list(List *list){
    if (list->size == 0 || list->size == 1) return;
     Node *p = list->first->pNext;
     Node *q = p->pNext;
     list->last = p;
     list->last->pNext = NULL;
     while (q != NULL) {
       p = q;
       q = q->pNext;
       p->pNext = list->first->pNext;
       p->pNext->pPrio = p;
       p->pPrio = list->first;
       list->first->pNext = p;
     }
}

void Clear_list(List *list){
    if (list->size == 0) return;
     Node *p = list->first->pNext;
     while (p != NULL) {
       if (p == list->last) {
         list->last = list->first;
         list->last->pNext = NULL;
       }
       else {
         p->pNext->pPrio = p->pPrio;
         p->pPrio->pNext = p->pNext;
       }
       free(p);
       p = list->first->pNext;
     }
     list->size = 0;
}

void Destory_list(List *list){
    Clear_list(list);
      free(list->first);
      list->first = list->last = NULL;
}
void testdlist() {
  List mylist;
  InitDlist(&mylist);


  Insert_in_head(&mylist,1);
  Insert_in_head(&mylist,7);
  Insert_in_head(&mylist,5);
  Insert_in_tail(&mylist,4);
  printf("list size 0  is %d \n",Get_dlist_length(&mylist));
  Sort_list(&mylist);
  Show_list(&mylist);
  Insert_node(&mylist,3);
  printf("list size 1  is %d \n",Get_dlist_length(&mylist));
   Show_list(&mylist);
   Insert_in_tail(&mylist,2);
    printf("list size 2 is %d \n",Get_dlist_length(&mylist));
    Show_list(&mylist);
   Delete_val(&mylist,7);
   printf("list size 3 is %d \n",Get_dlist_length(&mylist));
  Reverse_list(&mylist);
  Show_list(&mylist);

  Destory_list(&mylist);
}






