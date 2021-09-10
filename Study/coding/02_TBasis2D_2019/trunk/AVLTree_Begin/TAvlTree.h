#pragma once
#include <iostream>
#include <math.h>
using namespace std;

#ifndef max
#define max(a,b)            (((a) > (b)) ? (a) : (b))
#endif

class Node
{
public:
	Node *	m_pLeft;
	Node*	m_pRight;
	int		m_iKey;
	int		m_iHeight;
	Node(int iKey)
	{
		m_iKey = m_iKey;
		m_pLeft = m_pRight = nullptr;
		m_iHeight = 1;
	}
	Node(){}
};

class TAvlTree
{
public:
	int		getHeight(Node* node);
	int		getBalance(Node *N);
	Node*	updateHeight(Node* node);
	Node*   newNode(int m_iKey);
	Node*	insert(Node* root, int iKey);
	Node*	leftRotate(Node* root);
	Node*	rightRotate(Node* root);

	Node *	minValueNode(Node* node);
	Node*	deleteNode(Node* root, int iKey);
	void	printAll(Node* root);
	void    preOrder(Node *root);
public:
	TAvlTree();
	~TAvlTree();
};

