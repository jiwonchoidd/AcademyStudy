#include "TAvlTree.h"

Node* TAvlTree::newNode(int iKey)
{
	Node* node = new Node();
	node->m_iKey = iKey;
	node->m_pLeft = NULL;
	node->m_pRight = NULL;
	node->m_iHeight = 1;
	return(node);
}
int TAvlTree::getBalance(Node *N)
{
	if (N == NULL)
		return 0;
	return getHeight(N->m_pLeft) - getHeight(N->m_pRight);
}
int TAvlTree::getHeight(Node* node)
{
	if (node == nullptr)	return 0;
	return node->m_iHeight;
}
Node* TAvlTree::updateHeight(Node* node)
{
	node->m_iHeight = 1 + max(getHeight(node->m_pLeft), getHeight(node->m_pRight));
	return node;
}
Node* TAvlTree::insert(Node* root, int iKey)
{
	if (root == nullptr)
		return new Node(iKey);

	if (root->m_iKey < iKey)
	{
		root->m_pRight = insert(root->m_pRight, iKey);
	}
	else if (root->m_iKey == iKey)
	{
		cout << "m_iKey: " << iKey << " No duplicate vertex allowed." << endl;
		return root;
	}
	else
	{
		root->m_pLeft = insert(root->m_pLeft, iKey);
	}

	root = updateHeight(root);

	int balance_factor = getBalance(root);

	// LR rotation
	if (balance_factor > 1 && root->m_pLeft->m_iKey < iKey)
	{
		root->m_pLeft = leftRotate(root->m_pLeft);
		return rightRotate(root);
	}
	// RR rotation
	else if (balance_factor > 1 && root->m_pLeft->m_iKey > iKey)
	{
		return rightRotate(root);
	}
	// LL rotation
	else if (balance_factor < -1 && root->m_pRight->m_iKey < iKey)
	{
		return leftRotate(root);
	}
	// RL rotation
	else if (balance_factor < -1 && root->m_pRight->m_iKey > iKey)
	{
		root->m_pRight = rightRotate(root->m_pRight);
		return leftRotate(root);
	}

	return root;
}
Node* TAvlTree::leftRotate(Node* root)
{
	Node* x = root->m_pRight;
	Node* t = x->m_pLeft;

	root->m_pRight = t;
	x->m_pLeft = root;

	// update m_iHeight	
	root = updateHeight(root);
	x = updateHeight(x);
	return x;
}
Node* TAvlTree::rightRotate(Node* root)
{
	Node *x = root->m_pLeft;
	Node *t = x->m_pRight;

	root->m_pLeft = t;
	x->m_pRight = root;

	// update m_iHeight
	root = updateHeight(root);
	x = updateHeight(x);

	return x;
}
Node * TAvlTree::minValueNode(Node* node)
{
	Node* current = node;

	/* loop down to find the leftmost leaf */
	while (current->m_pLeft != NULL)
		current = current->m_pLeft;

	return current;
}

Node* TAvlTree::deleteNode(Node* root, int iKey)
{

	// STEP 1: PERFORM STANDARD BST DELETE  
	if (root == NULL)
		return root;

	// ���� ��尡 �θ𺸴� ���� ���
	if (iKey < root->m_iKey)
		root->m_pLeft = deleteNode(root->m_pLeft, iKey);

	// ���� ��尡 �θ� ���� Ŭ ���
	else if (iKey > root->m_iKey)
		root->m_pRight = deleteNode(root->m_pRight, iKey);

	// Root ���� ������ ������ ��尡 ��.
	else
	{
		// �ڽ��� �ϳ��� �ִ� ���  
		if ((root->m_pLeft == NULL) || (root->m_pRight == NULL))
		{
			Node *temp = root->m_pLeft ? root->m_pLeft : root->m_pRight;
			// �ڽ��� ���� ���  
			if (temp == NULL)
			{
				temp = root;
				root = NULL;
			}
			else
			{   // �ڽ��� �ϳ� �ִ� ���  
				*root = *temp;
			}
			free(temp);
		}
		else
		{
			// m_pRight ���� Ʈ������ ���� ���� �� ��� 
			Node* temp = minValueNode(root->m_pRight);
			root->m_iKey = temp->m_iKey;
			root->m_pRight = deleteNode(root->m_pRight, temp->m_iKey);
		}
	}
	// Ʈ���� ��尡 �ϳ��� �ִ� ��� ��ȯ��.
	if (root == NULL)
		return root;

	// STEP 2: ���� ���� 
	root->m_iHeight = 1 + max(getHeight(root->m_pLeft), getHeight(root->m_pRight));

	// STEP 3: �����μ� ���
	int balance = getBalance(root);

	// Left Left Case  
	if (balance > 1 && getBalance(root->m_pLeft) >= 0)
		return rightRotate(root);

	// Left Right Case  
	if (balance > 1 && getBalance(root->m_pLeft) < 0)
	{
		root->m_pLeft = leftRotate(root->m_pLeft);
		return rightRotate(root);
	}

	// Right Right Case  
	if (balance < -1 && getBalance(root->m_pRight) <= 0)
		return leftRotate(root);

	// Right Left Case  
	if (balance < -1 && getBalance(root->m_pRight) > 0)
	{
		root->m_pRight = rightRotate(root->m_pRight);
		return leftRotate(root);
	}
	return root;
}
void TAvlTree::printAll(Node* root)
{
	if (root->m_pLeft != nullptr)
		printAll(root->m_pLeft);

	cout << root->m_iKey << " ";

	if (root->m_pRight != nullptr)
		printAll(root->m_pRight);
}
void TAvlTree::preOrder(Node *root)
{
	if (root != NULL)
	{
		cout << root->m_iKey << " ";
		preOrder(root->m_pLeft);
		preOrder(root->m_pRight);
	}
}
TAvlTree::TAvlTree()
{
}
TAvlTree::~TAvlTree()
{
}
