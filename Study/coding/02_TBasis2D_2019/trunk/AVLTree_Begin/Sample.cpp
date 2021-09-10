//https://www.geeksforgeeks.org/avl-tree-set-2-deletion/
#include "TAvlTree.h"
#define AVL_DELETE
#ifndef AVL_DELETE
int main()
{
	TAvlTree avl;
	Node* root = nullptr;
	// 최상위 루트를 반환한다.
	root = avl.insert(root, 1);
	root = avl.insert(root, 2);
	root = avl.insert(root, 3);
	root = avl.insert(root, 4);
	root = avl.insert(root, 5);
	root = avl.insert(root, 6);
	root = avl.insert(root, 7);
	root = avl.insert(root, 8);
	// 중위순회로 출력.
	avl.printAll(root);
	return 0;
}
#else
int main()
{
	TAvlTree avl;
	Node *root = NULL;
	root = avl.insert(root, 9);
	root = avl.insert(root, 5);
	root = avl.insert(root, 10);
	root = avl.insert(root, 0);
	root = avl.insert(root, 6);
	root = avl.insert(root, 11);
	root = avl.insert(root, -1);
	root = avl.insert(root, 1);
	root = avl.insert(root, 2);
	
	avl.preOrder(root);

	root = avl.deleteNode(root, 10);
	avl.preOrder(root);
	return 0;
}

#endif