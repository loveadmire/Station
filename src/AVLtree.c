#include <stdio.h>
#include <stdlib.h>
#include "AVLtree.h"
#include "queue.h"

AVLtree *AVL_find(myType data, AVLtree *T)
{
	if(T == NULL) 
		return NULL;
	if(data < T->element)
		return find(data, T->lchild);
	else if(data > T->element)
		return find(data, T->rchild);
	else 
		return T;
}

AVLtree *AVL_findMin(AVLtree *T)
{
	if(T == NULL) 
		return NULL;
	else if(T->lchild == NULL)
		return T;
	else 
		return findMin(T->lchild);
}
AVLtree *AVL_findMax(AVLtree *T)
{
	if(T != NULL) 
		while(T->rchild != NULL)
			T = T->rchild;
	return T;
	
}

static int getHeight(AVLtree *T)
{
	return GET_HEIGHT(T);
}

static AVLtree *singleRotateWithLeft(AVLtree *T)
{
	AVLtree *TT;
	
	TT = T->lchild;
	T->lchild = TT->rchild;
	TT->rchild = T;

	T->height = MAX(getHeight(TT->lchild), getHeight(T->rchild)) + 1;
	TT->height = MAX(getHeight(TT->lchild), T->height) + 1;

	return TT;
}

static AVLtree *singleRotateWithRight(AVLtree *T)
{
	AVLtree *TT;

	TT = T->rchild;
	T->rchild = TT->lchild;
	TT->lchild = T;

	T->height = MAX(getHeight(TT->lchild), getHeight(T->rchild)) + 1;
	TT->height = MAX(getHeight(TT->lchild), T->height) + 1;
	
	return TT;
}

static AVLtree *doubleRotateWithLeft(AVLtree *T)
{
	T->lchild = singleRotateWithRight(T->lchild);

	return singleRotateWithLeft(T);
}

static AVLtree *doubleRotateWithRight(AVLtree *T)
{
	T->rchild = singleRotateWithLeft(T->rchild);

	return singleRotateWithRight(T);
}

AVLtree *AVL_insert(myType data, AVLtree *T)
{
	if(T == NULL) {
		T = (AVLtree *)malloc(sizeof( struct _AVLtreeNode));
		T->element = data;
		T->lchild = NULL;
		T->rchild = NULL;
		T->height = 0;
	} else if (data < T->element) {
		T->lchild = insert(data, T->lchild);	
		if(GET_HEIGHT(T->lchild) - GET_HEIGHT(T->rchild) == 2)
			if(data < T->lchild->element)
				T = singleRotateWithLeft(T);
			else 
				T = doubleRotateWithLeft(T);
	} else if (data > T->element) {
		T->rchild = insert(data, T->rchild);	
		if(GET_HEIGHT(T->rchild) - GET_HEIGHT(T->lchild) == 2)
			if(data > T->rchild->element)
				T = singleRotateWithRight(T);
			else 
				T = doubleRotateWithRight(T);
	}

	T->height = MAX(getHeight(T->lchild), getHeight(T->rchild) + 1);

	return T;
}
AVLtree *AVL_erase(myType data, AVLtree *T)
{
	AVLtree *tmpNode;

	if(T == NULL) {
		printf("NOT FOUNT\n");
	} else if (data < T->element) {
		T->lchild = erase(data, T->lchild);	
		if(getHeight(T->rchild) - getHeight(T->lchild) == 2) {
			AVLtree *tmp = T->rchild;
			if(getHeight(tmp->lchild) > getHeight(tmp->rchild))
				T = doubleRotateWithRight(T);
			else
				T = singleRotateWithRight(T);
		}
	} else if (data > T->element) {
		T->rchild = erase(data, T->rchild);
		if(getHeight(T->lchild) - getHeight(T->rchild) == 2) {
			AVLtree *tmp = T->lchild;
			if(getHeight(tmp->rchild) > getHeight(tmp->lchild))
				T = doubleRotateWithLeft(T);
			else
				T = singleRotateWithLeft(T);
		}
	//found element to delete, two children
	} else if (T->lchild && T->rchild){
		if(getHeight(T->rchild) > getHeight(T->lchild)) {
			tmpNode = findMin(T->rchild);//将右子树的最小值代替root
			T->element = tmpNode->element;
			T->rchild = erase(T->element, T->rchild);
		} else {
			tmpNode = findMax(T->lchild);//将左子树的最大值代替root	
			T->element = tmpNode->element;
			T->lchild = erase(T->element, T->lchild);
		}
	} else {
		//one or zero children
		tmpNode = T;	
		T = (T->lchild == NULL ? T->rchild : T->lchild);
		free(tmpNode);
	}	

	return T;
}

void AVL_levelOrder(AVLtree *T)
{
	QUEUE *q = createQueue(100);

	while(T != NULL) {
		printf("%d ", T->element);	
		if(T->lchild != NULL) 
			enQueue(T->lchild, q);	
		if(T->rchild != NULL)
			enQueue(T->rchild, q);
	
		if(!isEmpty(q))
			T = frontAndDequeue(q);
		else
			T = NULL;
	}

	disposeQueue(q);
}

void AVL_preOrder(AVLtree *T)
{
	if(T != NULL) {
		printf("%d ", T->element);	
		preOrder(T->lchild);
		preOrder(T->rchild);
	}
}

void AVL_inOrder(AVLtree *T)
{
	if(T != NULL) {
		inOrder(T->lchild);	
		printf("%d ", T->element);	
		inOrder(T->rchild);
	}
}
void AVL_postOrder(AVLtree *T)
{
	if(T != NULL) {
		postOrder(T->lchild);	
		postOrder(T->rchild);
		printf("%d ", T->element);	
	}
}
/*
void createTree(AVLtree **T)
{
	myType data;
	scanf("%d", &data);

	if(data == -1) {
		*T = NULL;
	} else {
		*T = (AVLtree *)malloc(sizeof(struct treeNode));	
		(*T)->element = data;
		printf("请输入%d的左孩子节点：", data);
		createTree(&((*T)->lchild));
		printf("请输入%d的右孩子节点：", data);
		createTree(&((*T)->rchild));
	}
}
*/
int AVL_tree_test(void)
{
	AVLtree *t = NULL;

	//createTree(&t);
	t = insert(5, t);
	t = insert(2, t);
	t = insert(1, t);
	t = insert(4, t);
	t = insert(3, t);
	t = insert(8, t);
	t = insert(6, t);
	t = insert(7, t);

	printf("层次遍历\n");
	levelOrder(t);
	printf("\n先根序\n");
	preOrder(t);
	printf("\n中根序\n");
	inOrder(t);
	printf("\n后根序\n");
	postOrder(t);
	printf("\nfind min:%d\n", (findMin(t))->element);
	printf("find max:%d\n", (findMax(t))->element);
	printf("find 6: %d\n", (find(6, t))->element);
	t = erase(6, t);
	printf("删除6，层次遍历\n");
	levelOrder(t);
	t = erase(2, t);
	printf("\n删除2，层次遍历\n");
	levelOrder(t);


	return 0;
}
