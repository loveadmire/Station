#ifndef __AVLTREE_H_
#define __AVLTREE_H_

#define MAX(a, b)	(a > b ? a : b)
#define GET_HEIGHT(T)	(T == NULL ? -1 : T->height)

/*
 * if use this please in queuue.h add noe line
 *
 * typedef AVLtree *elementType;
 *
 */

typedef int myType;

typedef struct _AVLtreeNode
{
	myType element;
	struct _AVLtreeNode *lchild;
	struct _AVLtreeNode *rchild;
	int height;
}AVLtree,*PAVLtree;

//void createTree(AVLtree **T);
void AVL_preOrder(AVLtree *T);
void AVL_inOrder(AVLtree *T);
void AVL_postOrder(AVLtree *T);
void AVL_levelOrder(AVLtree *T);

AVLtree *AVL_find(myType data, AVLtree *T);
AVLtree *AVL_findMin(AVLtree *T);
AVLtree *AVL_findMax(AVLtree *T);
AVLtree *AVL_insert(myType data, AVLtree *T);
AVLtree *AVL_erase(myType data, AVLtree *T);


int AVL_tree_test(void);

#endif  //__AVLTREE_H_
