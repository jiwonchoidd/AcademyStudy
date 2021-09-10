#include "TRBTree.h"


Node::Node(int data) 
{
	this->data = data;
	color = RED;
	left = right = parent = nullptr;
}

RBTree::RBTree() 
{
	m_pRoot = nullptr;
}
int  RBTree::search(int iValue)
{
	return 0;
}
int RBTree::getColor(Node *node) 
{
	if (node == nullptr)
		return BLACK;

	return node->color;
}

void RBTree::setColor(Node *node, int color) 
{
	if (node == nullptr)
		return;

	node->color = color;
}

Node* RBTree::insertBST(Node * node, Node *ptr) 
{
	if (node == nullptr)
		return ptr;

	if (ptr->data < node->data)
	{
		node->left = insertBST(node->left, ptr);
		node->left->parent = node;
	}
	else if (ptr->data > node->data)
	{
		node->right = insertBST(node->right, ptr);
		node->right->parent = node;
	}

	return node;
}

void RBTree::insertValue(int n)
{
	Node *node = new Node(n);
	m_pRoot = insertBST(m_pRoot, node);
	fixInsertRBTree(node);
}

void RBTree::rotateLeft(Node *ptr) 
{
	Node *right_child = ptr->right;
	ptr->right = right_child->left;

	if (ptr->right != nullptr)
		ptr->right->parent = ptr;

	right_child->parent = ptr->parent;

	if (ptr->parent == nullptr)
		m_pRoot = right_child;
	else if (ptr == ptr->parent->left)
		ptr->parent->left = right_child;
	else
		ptr->parent->right = right_child;

	right_child->left = ptr;
	ptr->parent = right_child;
}

void RBTree::rotateRight(Node *ptr) 
{
	Node *left_child = ptr->left;
	ptr->left = left_child->right;

	if (ptr->left != nullptr)
		ptr->left->parent = ptr;

	left_child->parent = ptr->parent;

	if (ptr->parent == nullptr)
		m_pRoot = left_child;
	else if (ptr == ptr->parent->left)
		ptr->parent->left = left_child;
	else
		ptr->parent->right = left_child;

	left_child->right = ptr;
	ptr->parent = left_child;
}

void RBTree::fixInsertRBTree(Node *ptr) 
{
	Node *parent = nullptr;
	Node *grandparent = nullptr;
	while (ptr != m_pRoot && getColor(ptr) == RED && getColor(ptr->parent) == RED)
	{
		parent = ptr->parent;
		grandparent = parent->parent;
		if (parent == grandparent->left) 
		{
			Node *uncle = grandparent->right;
			if (getColor(uncle) == RED) 
			{
				setColor(uncle, BLACK);
				setColor(parent, BLACK);
				setColor(grandparent, RED);
				ptr = grandparent;
			}
			else 
			{
				if (ptr == parent->right) 
				{
					rotateLeft(parent);
					ptr = parent;
					parent = ptr->parent;
				}
				rotateRight(grandparent);
				swap(parent->color, grandparent->color);
				ptr = parent;
			}
		}
		else 
		{
			Node *uncle = grandparent->left;
			if (getColor(uncle) == RED)
			{
				setColor(uncle, BLACK);
				setColor(parent, BLACK);
				setColor(grandparent, RED);
				ptr = grandparent;
			}
			else 
			{
				if (ptr == parent->left) 
				{
					rotateRight(parent);
					ptr = parent;
					parent = ptr->parent;
				}
				rotateLeft(grandparent);
				swap(parent->color, grandparent->color);
				ptr = parent;
			}
		}
	}
	setColor(m_pRoot, BLACK);
}

void RBTree::fixDeleteRBTree(Node *node)
{
	if (node == nullptr)
		return;

	if (node == m_pRoot) 
	{
		m_pRoot = nullptr;
		return;
	}

	if (getColor(node) == RED || getColor(node->left) == RED || getColor(node->right) == RED) 
	{
		Node *child = node->left != nullptr ? node->left : node->right;

		if (node == node->parent->left) 
		{
			node->parent->left = child;
			if (child != nullptr)
				child->parent = node->parent;
			setColor(child, BLACK);
			delete (node);
		}
		else 
		{
			node->parent->right = child;
			if (child != nullptr)
				child->parent = node->parent;
			setColor(child, BLACK);
			delete (node);
		}
	}
	else {
		Node *sibling = nullptr;
		Node *parent = nullptr;
		Node *ptr = node;
		setColor(ptr, DOUBLE_BLACK);
		while (ptr != m_pRoot && getColor(ptr) == DOUBLE_BLACK) 
		{
			parent = ptr->parent;
			if (ptr == parent->left) 
			{
				sibling = parent->right;
				if (getColor(sibling) == RED) 
				{
					setColor(sibling, BLACK);
					setColor(parent, RED);
					rotateLeft(parent);
				}
				else {
					if (getColor(sibling->left) == BLACK && getColor(sibling->right) == BLACK) 
					{
						setColor(sibling, RED);
						if (getColor(parent) == RED)
							setColor(parent, BLACK);
						else
							setColor(parent, DOUBLE_BLACK);
						ptr = parent;
					}
					else 
					{
						if (getColor(sibling->right) == BLACK) 
						{
							setColor(sibling->left, BLACK);
							setColor(sibling, RED);
							rotateRight(sibling);
							sibling = parent->right;
						}
						setColor(sibling, parent->color);
						setColor(parent, BLACK);
						setColor(sibling->right, BLACK);
						rotateLeft(parent);
						break;
					}
				}
			}
			else 
			{
				sibling = parent->left;
				if (getColor(sibling) == RED) 
				{
					setColor(sibling, BLACK);
					setColor(parent, RED);
					rotateRight(parent);
				}
				else 
				{
					if (getColor(sibling->left) == BLACK && getColor(sibling->right) == BLACK)
					{
						setColor(sibling, RED);
						if (getColor(parent) == RED)
							setColor(parent, BLACK);
						else
							setColor(parent, DOUBLE_BLACK);
						ptr = parent;
					}
					else {
						if (getColor(sibling->left) == BLACK) 
						{
							setColor(sibling->right, BLACK);
							setColor(sibling, RED);
							rotateLeft(sibling);
							sibling = parent->left;
						}
						setColor(sibling, parent->color);
						setColor(parent, BLACK);
						setColor(sibling->left, BLACK);
						rotateRight(parent);
						break;
					}
				}
			}
		}
		if (node == node->parent->left)
			node->parent->left = nullptr;
		else
			node->parent->right = nullptr;
		delete(node);
		setColor(m_pRoot, BLACK);
	}
}

Node* RBTree::deleteBST(Node *node, int data)
{
	if (node == nullptr)
		return node;

	if (data < node->data)
		return deleteBST(m_pRoot->left, data);

	if (data > node->data)
		return deleteBST(m_pRoot->right, data);

	if (node->left == nullptr || m_pRoot->right == nullptr)
		return node;

	Node *temp = minValueNode(node->right);
	node->data = temp->data;
	return deleteBST(node->right, temp->data);
}

void RBTree::deleteValue(int data) 
{
	Node *node = deleteBST(m_pRoot, data);
	fixDeleteRBTree(node);
}

void RBTree::inorderBST(Node *ptr) 
{
	if (ptr == nullptr)
		return;

	inorderBST(ptr->left);
	cout << ptr->data << " " << ptr->color << endl;
	inorderBST(ptr->right);
}

void RBTree::inorder() 
{
	inorderBST(m_pRoot);
}

void RBTree::preorderBST(Node *ptr) 
{
	if (ptr == nullptr)
		return;

	cout << ptr->data << " " << ptr->color << endl;
	preorderBST(ptr->left);
	preorderBST(ptr->right);
}

void RBTree::preorder() 
{
	preorderBST(m_pRoot);
	cout << "-------" << endl;
}

Node *RBTree::minValueNode(Node *node) 
{

	Node *ptr = node;

	while (ptr->left != nullptr)
		ptr = ptr->left;

	return ptr;
}

Node* RBTree::maxValueNode(Node *node) 
{
	Node *ptr = node;

	while (ptr->right != nullptr)
		ptr = ptr->right;

	return ptr;
}

int RBTree::getBlackHeight(Node *node) 
{
	int blackheight = 0;
	while (node != nullptr) {
		if (getColor(node) == BLACK)
			blackheight++;
		node = node->left;
	}
	return blackheight;
}

// Test case 1 : 5 2 9 1 6 8 0 20 30 35 40 50 0
// Test case 2 : 3 0 5 0
// Test case 3 : 2 1 3 0 8 9 4 5 0

void RBTree::merge(RBTree rbTree2) 
{
	int temp;
	Node *c;
	Node *temp_ptr = nullptr;
	Node *root1 = m_pRoot;
	Node *root2 = rbTree2.m_pRoot;
	int initialblackheight1 = getBlackHeight(root1);
	int initialblackheight2 = getBlackHeight(root2);
	if (initialblackheight1 > initialblackheight2) 
	{
		c = maxValueNode(root1);
		temp = c->data;
		deleteValue(c->data);
		root1 = m_pRoot;
	}
	else if (initialblackheight2 > initialblackheight1) 
	{
		c = minValueNode(root2);
		temp = c->data;
		rbTree2.deleteValue(c->data);
		root2 = rbTree2.m_pRoot;
	}
	else 
	{
		c = minValueNode(root2);
		temp = c->data;
		rbTree2.deleteValue(c->data);
		root2 = rbTree2.m_pRoot;
		if (initialblackheight1 != getBlackHeight(root2)) 
		{
			rbTree2.insertValue(c->data);
			root2 = rbTree2.m_pRoot;
			c = maxValueNode(root1);
			temp = c->data;
			deleteValue(c->data);
			root1 = m_pRoot;
		}
	}
	setColor(c, RED);
	int finalblackheight1 = getBlackHeight(root1);
	int finalblackheight2 = getBlackHeight(root2);
	if (finalblackheight1 == finalblackheight2) 
	{
		c->left = root1;
		root1->parent = c;
		c->right = root2;
		root2->parent = c;
		setColor(c, BLACK);
		c->data = temp;
		m_pRoot = c;
	}
	else if (finalblackheight2 > finalblackheight1) 
	{
		Node *ptr = root2;
		while (finalblackheight1 != getBlackHeight(ptr)) 
		{
			temp_ptr = ptr;
			ptr = ptr->left;
		}
		Node *ptr_parent;
		if (ptr == nullptr)
			ptr_parent = temp_ptr;
		else
			ptr_parent = ptr->parent;
		c->left = root1;
		if (root1 != nullptr)
			root1->parent = c;
		c->right = ptr;
		if (ptr != nullptr)
			ptr->parent = c;
		ptr_parent->left = c;
		c->parent = ptr_parent;
		if (getColor(ptr_parent) == RED) 
		{
			fixInsertRBTree(c);
		}
		else if (getColor(ptr) == RED) 
		{
			fixInsertRBTree(ptr);
		}
		c->data = temp;
		m_pRoot = root2;
	}
	else {
		Node *ptr = root1;
		while (finalblackheight2 != getBlackHeight(ptr)) 
		{
			ptr = ptr->right;
		}
		Node *ptr_parent = ptr->parent;
		c->right = root2;
		root2->parent = c;
		c->left = ptr;
		ptr->parent = c;
		ptr_parent->right = c;
		c->parent = ptr_parent;
		if (getColor(ptr_parent) == RED)
		{
			fixInsertRBTree(c);
		}
		else if (getColor(ptr) == RED) 
		{
			fixInsertRBTree(ptr);
		}
		c->data = temp;
		m_pRoot = root1;
	}
	return;
}
