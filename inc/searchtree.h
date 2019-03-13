#ifndef _SEARCHTREE_H_
#define _SEARCHTREE_H_

typedef int myType;

/*
 * if use this please in queuue.h add noe line
 *
 * typedef searchTree *elementType;
 *
 */

typedef struct _srarchtreeNode
{
	myType element;
	struct treeNode *lchild;
	struct treeNode *rchild;
}searchTree,*psearchTree;

void createTree(searchTree **T);
void preOrder(searchTree *T);
void inOrder(searchTree *T);
void postOrder(searchTree *T);
void levelOrder(searchTree *T);

searchTree *find(myType data, searchTree *T);
searchTree *findMin(searchTree *T);
searchTree *findMax(searchTree *T);
searchTree *insert(myType data, searchTree *T);
searchTree *erase(myType data, searchTree *T);

int searchtree_test(void);

#endif
