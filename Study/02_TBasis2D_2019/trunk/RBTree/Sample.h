#pragma once
#include<iostream>
#include <cassert>

using namespace std;
const int LEAF = 0;
const int BLACK = 0;
const int RED = 1;

struct node
{
	int			key;
	node*		parent;
	char		color;
	node*		left;
	node*		right;
	node() {
		key = 0;
		parent = left = right = 0;
		color = 'r';
	}
};
struct node *parent(struct node *n);

struct node *grandparent(struct node *n);

struct node *sibling(struct node *n);

struct node *uncle(struct node *n);

void rotate_left(struct node *n);

void rotate_right(struct node *n);
//////////////////////////////// Insert ///////////////////////////////////
void insert_repair_tree(struct node *n);
void insert_case1(struct node *n);
void insert_case2(struct node *n);
void insert_case3(struct node *n);
void insert_case4(struct node *n);
void insert_case4step2(struct node *n);
/////////////////////////////// Remove ////////////////////////////////
void delete_one_child(struct node *n);
void delete_case1(struct node *n);
void delete_case2(struct node *n);
void delete_case3(struct node *n);
void delete_case4(struct node *n);
void delete_case5(struct node *n);
void delete_case6(struct node *n);


//////////////////////////////////////////////////////////////////////////
bool is_leaf(node* n)
{
	if (n->left && n->right)
		return false;
	return true;
}
void replace_node(node*n, node* child)
{
	node* temp = n;
	n = child;
	child = temp;
}
struct node *parent(struct node *n)
{
	return n->parent;
}

struct node *grandparent(struct node *n)
{
	struct node *p = parent(n);
	if (p == NULL)
		return NULL; // No parent means no grandparent
	return parent(p);
}

struct node *sibling(struct node *n)
{
	struct node *p = parent(n);
	if (p == NULL)
		return NULL; // No parent means no sibling
	if (n == p->left)
		return p->right;
	else
		return p->left;
}

struct node *uncle(struct node *n)
{
	struct node *p = parent(n);
	struct node *g = grandparent(n);
	if (g == NULL)
		return NULL; // No grandparent means no uncle
	return sibling(p);
}

void rotate_left(struct node *n)
{
	struct node *nnew = n->right;
	assert(nnew != LEAF); // since the leaves of a red-black tree are empty, they cannot become internal nodes
	n->right = nnew->left;
	nnew->left = n;
	nnew->parent = n->parent;
	n->parent = nnew;
	// (the other related parent and child links would also have to be updated)
}

void rotate_right(struct node *n)
{
	struct node *nnew = n->left;
	assert(nnew != LEAF); // since the leaves of a red-black tree are empty, they cannot become internal nodes
	n->left = nnew->right;
	nnew->right = n;
	nnew->parent = n->parent;
	n->parent = nnew;
	// (the other related parent and child links would also have to be updated)
}
//////////////////////////////// Insert ///////////////////////////////////
void insert_repair_tree(struct node *n)
{
	if (parent(n) == NULL) {
		insert_case1(n);
	}
	else if (parent(n)->color == BLACK) {
		insert_case2(n);
	}
	else if (uncle(n)->color == RED) {
		insert_case3(n);
	}
	else {
		insert_case4(n);
	}
}
void insert_case1(struct node *n)
{
	if (parent(n) == NULL)
		n->color = BLACK;
}
void insert_case2(struct node *n)
{
	return; /* Do nothing since tree is still valid */
}
void insert_case3(struct node *n)
{
	parent(n)->color = BLACK;
	uncle(n)->color = BLACK;
	grandparent(n)->color = RED;
	insert_repair_tree(grandparent(n));
}
void insert_case4(struct node *n)
{
	struct node *p = parent(n);
	struct node *g = grandparent(n);

	if (n == g->left->right) {
		rotate_left(p);
		n = n->left;
	}
	else if (n == g->right->left) {
		rotate_right(p);
		n = n->right;
	}

	insert_case4step2(n);
}
void insert_case4step2(struct node *n)
{
	struct node *p = parent(n);
	struct node *g = grandparent(n);

	if (n == p->left)
		rotate_right(g);
	else
		rotate_left(g);
	p->color = BLACK;
	g->color = RED;
}
/////////////////////////////// Remove ////////////////////////////////
void delete_one_child(struct node *n)
{
	/*
	* Precondition: n has at most one non-leaf child.
	*/
	struct node *child = is_leaf(n->right) ? n->left : n->right;

	replace_node(n, child);
	if (n->color == BLACK) {
		if (child->color == RED)
			child->color = BLACK;
		else
			delete_case1(child);
	}
	free(n);
}
void delete_case1(struct node *n)
{
	if (n->parent != NULL)
		delete_case2(n);
}
void delete_case2(struct node *n)
{
	struct node *s = sibling(n);

	if (s->color == RED) {
		n->parent->color = RED;
		s->color = BLACK;
		if (n == n->parent->left)
			rotate_left(n->parent);
		else
			rotate_right(n->parent);
	}
	delete_case3(n);
}
void delete_case3(struct node *n)
{
	struct node *s = sibling(n);

	if ((n->parent->color == BLACK) &&
		(s->color == BLACK) &&
		(s->left->color == BLACK) &&
		(s->right->color == BLACK)) {
		s->color = RED;
		delete_case1(n->parent);
	}
	else
		delete_case4(n);
}
void delete_case4(struct node *n)
{
	struct node *s = sibling(n);

	if ((n->parent->color == RED) &&
		(s->color == BLACK) &&
		(s->left->color == BLACK) &&
		(s->right->color == BLACK)) {
		s->color = RED;
		n->parent->color = BLACK;
	}
	else
		delete_case5(n);
}
void delete_case5(struct node *n)
{
	struct node *s = sibling(n);

	if (s->color == BLACK) { /* this if statement is trivial,
							 due to case 2 (even though case 2 changed the sibling to a sibling's child,
							 the sibling's child can't be red, since no red parent can have a red child). */
							 /* the following statements just force the red to be on the left of the left of the parent,
							 or right of the right, so case six will rotate correctly. */
		if ((n == n->parent->left) &&
			(s->right->color == BLACK) &&
			(s->left->color == RED)) { /* this last test is trivial too due to cases 2-4. */
			s->color = RED;
			s->left->color = BLACK;
			rotate_right(s);
		}
		else if ((n == n->parent->right) &&
			(s->left->color == BLACK) &&
			(s->right->color == RED)) {/* this last test is trivial too due to cases 2-4. */
			s->color = RED;
			s->right->color = BLACK;
			rotate_left(s);
		}
	}
	delete_case6(n);
}
void delete_case6(struct node *n)
{
	struct node *s = sibling(n);

	s->color = n->parent->color;
	n->parent->color = BLACK;

	if (n == n->parent->left) {
		s->right->color = BLACK;
		rotate_left(n->parent);
	}
	else {
		s->left->color = BLACK;
		rotate_right(n->parent);
	}
}