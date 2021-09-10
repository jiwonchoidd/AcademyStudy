#pragma once
#include <stdlib.h>
#include <stdio.h>
#include <algorithm>
#include <queue>
#include <iostream>
#include <string>

enum RBTColor
{
    RED,
    BLACK,
};


template<class Key, class Value>
class RBTNode
{
public:
    RBTNode( RBTColor col, Key k, Value val, RBTNode* lch, RBTNode* rch, RBTNode* fa )
    : color( col ), key( k ), value( val ), blackNum( 0 ), leftChild( lch ), rightChild( rch ), father( fa ) {}

    RBTColor color;         
    Key key;                
    Value value;            
    int blackNum;           
    RBTNode *leftChild;     
    RBTNode *rightChild;    
    RBTNode *father;        
};


template<class Key, class Value>
class NLPair
{
public:
    NLPair( RBTNode<Key, Value>* node, int layer )
    : node( node ), layer( layer ) {}

    RBTNode<Key, Value>* node;
    unsigned int layer;
};

template<class Key, class Value>
class RBTree
{
    
private:
    RBTNode<Key, Value> *root;

public:
    RBTree();
    ~RBTree();
    bool isRBT();
    RBTNode<Key, Value>* insert( Key key );
    RBTNode<Key, Value>* insert( Key key, Value value );
    RBTNode<Key, Value>* insertUnique( Key key );
    RBTNode<Key, Value>* insertUnique( Key key, Value value );
    void erase( Key key );
    void clear();
    RBTNode<Key, Value>* find( Key key );
    void draw();
    RBTNode<Key, Value>* begin();
    RBTNode<Key, Value>* end();
    unsigned int size();
    bool empty();

private:
    bool isBST( RBTNode<Key, Value> *node );
    bool hasTwoRed( RBTNode<Key, Value> *node );
    bool hasSameBlack( RBTNode<Key, Value> *node );
    void calBlackNum( RBTNode<Key, Value> *node, int blackNum );
    void leftRotate( RBTNode<Key, Value> *root );
    void rightRotate( RBTNode<Key, Value> *root );
    void insert( RBTNode<Key, Value> *node );
    void insertFix( RBTNode<Key, Value> *node );
    void erase( RBTNode<Key, Value> *node );
    void eraseFix( RBTNode<Key, Value> *node );
    void eraseNode( RBTNode<Key, Value> *node );
    void clear( RBTNode<Key, Value> *node );
    RBTNode<Key, Value>* find( RBTNode<Key, Value> *node, Key key );
    unsigned int size( RBTNode<Key, Value> *node );
};
template<class Key, class Value>
RBTree<Key, Value>::RBTree()
    : root(NULL) {}


template<class Key, class Value>
RBTree<Key, Value>::~RBTree()
{
    clear();
}


template<class Key, class Value>
void RBTree<Key, Value>::clear()
{
    clear(root);
    root = NULL;
}


template<class Key, class Value>
void RBTree<Key, Value>::clear(RBTNode<Key, Value>* node)
{
    if (node == NULL)
    {
        return;
    }
    if (node->leftChild != NULL)
    {
        clear(node->leftChild);
    }
    if (node->rightChild != NULL)
    {
        clear(node->rightChild);
    }
    delete node;
}

/*
 *
 *      px                              px
 *     /                               /
 *    x                               y
 *   /  \      ---->           / \
 *  lx   y                          x  ry
 *     /   \                       /  \
 *    ly   ry                     lx  ly
 *
 * px->Child = y
 * x->father = y
 * x->rightChild = ly
 * y->father = px
 * y->leftChild = x
 * ly->father = x
 */
template<class Key, class Value>
void RBTree<Key, Value>::leftRotate(RBTNode<Key, Value>* node)
{
    RBTNode<Key, Value>* x = node;
    RBTNode<Key, Value>* y = x->rightChild;

    // x->rightChild = ly
    x->rightChild = y->leftChild;

    // ly->father = x, 
    if (y->leftChild != NULL)
    {
        y->leftChild->father = x;
    }

    // y->father = px
    y->father = x->father;

    // px-Child = y，
    if (x->father == NULL)   {        root = y;    }
    if (x->father == NULL) {
        root = y;
    }
    else
    {
        if (x->father->leftChild == x)
        {
            x->father->leftChild = y;
        }
        else
        {
            x->father->rightChild = y;
        }
    }

    // y->leftChild = x
    y->leftChild = x;

    // x->father = y
    x->father = y;
}

template<class Key, class Value>
void RBTree<Key, Value>::rightRotate(RBTNode<Key, Value>* node)
{
    RBTNode<Key, Value>* y = node;
    RBTNode<Key, Value>* x = y->leftChild;

    y->leftChild = x->rightChild;

    if (x->rightChild != NULL)
    {
        x->rightChild->father = y;
    }

    x->father = y->father;

    if (y->father == NULL)
    {
        root = x;
    }
    else
    {
        if (y->father->leftChild == y)
        {
            y->father->leftChild = x;
        }
        else
        {
            y->father->rightChild = x;
        }
    }

    x->rightChild = y;

    y->father = x;
}


template<class Key, class Value>
RBTNode<Key, Value>* RBTree<Key, Value>::insert(Key key) {
    return insert(key, key);
}


template<class Key, class Value>
RBTNode<Key, Value>* RBTree<Key, Value>::insert(Key key, Value value) {
    RBTNode<Key, Value>* node = NULL;

    if ((node = new RBTNode<Key, Value>(BLACK, key, value, NULL, NULL, NULL)) == NULL)
    {
        return NULL;
    }

    insert(node);

    return node;
}


template<class Key, class Value>
RBTNode<Key, Value>* RBTree<Key, Value>::insertUnique(Key key) {
    return insertUnique(key, key);
}


template<class Key, class Value>
RBTNode<Key, Value>* RBTree<Key, Value>::insertUnique(Key key, Value value) {
    RBTNode<Key, Value>* node = find(key);
    if (node == NULL)
    {
        return insert(key, value);
    }
    return node;
}


template<class Key, class Value>
void RBTree<Key, Value>::insert(RBTNode<Key, Value>* node)
{
    RBTNode<Key, Value>* y = NULL;
    RBTNode<Key, Value>* x = root;


    while (x != NULL)
    {
        y = x;
        if (node->key < x->key)
        {
            x = x->leftChild;
        }
        else
        {
            x = x->rightChild;
        }
    }

    node->father = y;
    if (y != NULL)
    {
        if (node->key < y->key)
        {
            y->leftChild = node;
        }
        else
        {
            y->rightChild = node;
        }
    }
    else
    {
        root = node;
    }

    node->color = RED;

    insertFix(node);
}


template<class Key, class Value>
void RBTree<Key, Value>::insertFix(RBTNode<Key, Value>* node)
{
    if (node == NULL)
    {
        return;
    }


    RBTNode<Key, Value>* fa, * gfa, * un;


    while (((fa = node->father) != NULL) && (fa->color == RED))
    {
        gfa = fa->father;


        if (fa == gfa->leftChild)
        {
            un = gfa->rightChild;

            if (un != NULL && un->color == RED)
            {
                fa->color = BLACK;
                un->color = BLACK;
                gfa->color = RED;
                node = gfa;
                continue;
            }
            else
            {

                if (fa->rightChild == node)
                {
                    leftRotate(fa);
                    std::swap(fa, node);
                }


                fa->color = BLACK;
                gfa->color = RED;
                rightRotate(gfa);
            }
        }

        else
        {
            un = gfa->leftChild;

            if (un != NULL && un->color == RED)
            {
                fa->color = BLACK;
                un->color = BLACK;
                gfa->color = RED;
                node = gfa;
                continue;
            }
            else
            {

                if (fa->leftChild == node)
                {
                    rightRotate(fa);
                    std::swap(fa, node);
                }


                fa->color = BLACK;
                gfa->color = RED;
                leftRotate(gfa);
            }
        }
    }

    root->color = BLACK;
}


template<class Key, class Value>
void RBTree<Key, Value>::erase(Key key)
{
    RBTNode<Key, Value>* node = find(key);
    if (node != NULL)
    {
        erase(node);
    }
    else
    {
        std::cout << "Can't find node" << std::endl;
    }
}


template<class Key, class Value>
void RBTree<Key, Value>::erase(RBTNode<Key, Value>* node)
{
    if (node == NULL)
    {
        return;
    }

    RBTNode<Key, Value>* child, * father;
    RBTColor color;


    if (node->leftChild == NULL && node->rightChild == NULL)
    {

        if (node->color == BLACK)
        {
            if (node == root)
            {
                eraseNode(node);
                return;
            }
            eraseFix(node);
        }
    }

    else if (node->leftChild == NULL || node->rightChild == NULL)
    {
        child = node->leftChild ? node->leftChild : node->rightChild;

        std::swap(node->key, child->key);
        std::swap(node->value, child->value);

        erase(child);
        return;
    }

    else
    {

        RBTNode<Key, Value>* replace = node;


        replace = replace->rightChild;
        while (replace->leftChild != NULL)
        {
            replace = replace->leftChild;
        }

        std::swap(node->key, replace->key);
        std::swap(node->value, replace->value);

        erase(replace);
        return;
    }
    eraseNode(node);
}


template<class Key, class Value>
void RBTree<Key, Value>::eraseFix(RBTNode<Key, Value>* node)
{
    if (node == NULL || node == root)
    {
        return;
    }

    RBTNode<Key, Value>* brother, * father;
    father = node->father;

    while ((node == NULL || node->color == BLACK) && node != root)
    {

        if (node = father->leftChild)
        {
            brother = father->rightChild;


            if (brother != NULL && brother->color == RED)
            {
                brother->color = BLACK;
                father->color = RED;
                leftRotate(father);
                brother = father->rightChild;
            }
            if ((brother->leftChild == NULL || brother->leftChild->color == BLACK) &&
                (brother->rightChild == NULL || brother->rightChild->color == BLACK))
            {
                brother->color = RED;
                node = father;
                father = node->father;
            }
            else
            {
                if (brother->rightChild == NULL || brother->rightChild->color == BLACK)
                {
                    if (brother->leftChild != NULL)
                    {
                        brother->leftChild->color = BLACK;
                    }
                    brother->color = RED;
                    rightRotate(brother);
                    brother = father->rightChild;
                }

                brother->color = father->color;
                father->color = BLACK;
                if (brother->rightChild)
                {
                    brother->rightChild->color = BLACK;
                }
                leftRotate(father);
                node = root;
                break;
            }
        }
        else
        {
            brother = father->leftChild;

            // 1. 兄弟是红色的
            if (brother != NULL && brother->color == RED)
            {
                brother->color = BLACK;
                father->color = RED;
                rightRotate(father);
                brother = father->leftChild;
            }
            if ((brother->leftChild == NULL || brother->leftChild->color == BLACK) &&
                (brother->rightChild == NULL || brother->rightChild->color == BLACK))
            {
                brother->color = RED;
                node = father;
                father = node->father;
            }
            else
            {
                if (brother->leftChild == NULL || brother->leftChild->color == BLACK)
                {
                    if (brother->rightChild != NULL)
                    {
                        brother->rightChild->color = BLACK;
                    }
                    brother->color = RED;
                    leftRotate(brother);
                    brother = father->leftChild;
                }

                brother->color = father->color;
                father->color = BLACK;
                if (brother->leftChild)
                {
                    brother->leftChild->color = BLACK;
                }
                rightRotate(father);
                node = root;
                break;
            }
        }
    }
    if (node)
        node->color = BLACK;
}

template<class Key, class Value>
void RBTree<Key, Value>::eraseNode(RBTNode<Key, Value>* node)
{
    if (node == root)
    {
        root = NULL;
    }
    else
    {
        if (node->father->leftChild == node)
        {
            node->father->leftChild = NULL;
        }
        else
        {
            node->father->rightChild = NULL;
        }
    }

    delete node;
}

template<class Key, class Value>
RBTNode<Key, Value>* RBTree<Key, Value>::find(Key key)
{
    return find(root, key);
}

template<class Key, class Value>
RBTNode<Key, Value>* RBTree<Key, Value>::find(RBTNode<Key, Value>* node, Key key)
{
    if (node == NULL || node->key == key)
    {
        return node;
    }
    if (key < node->key)
    {
        return find(node->leftChild, key);
    }
    else
    {
        return find(node->rightChild, key);
    }
}

template<class Key, class Value>
void RBTree<Key, Value>::draw()
{
    if (root == NULL)
    {
        std::cout << "NULL" << std::endl;
        return;
    }

    NLPair<Key, Value> Now(NULL, 0), Next(NULL, 0);
    std::queue<NLPair<Key, Value>> Que;
    int layer = 0;

    Que.push(NLPair<Key, Value>(root, layer));
    while (!Que.empty())
    {
        Now = Que.front();
        Que.pop();
        if (Now.node == NULL)
        {
            continue;
        }
        if (Now.layer != layer)
        {
            layer = Now.layer;
            std::cout << std::endl;
        }
        std::cout << Now.node->key << "(";
        if (Now.node->father != NULL)
        {
            std::cout << Now.node->father->key << " ";
        }
        std::cout << (Now.node->color == RED ? "RED" : "BLACK") << ")" << "\t";
        Next = NLPair<Key, Value>(Now.node->leftChild, Now.layer + 1);
        Que.push(Next);
        Next = NLPair<Key, Value>(Now.node->rightChild, Now.layer + 1);
        Que.push(Next);
    }
    std::cout << std::endl;
}

template<class Key, class Value>
RBTNode<Key, Value>* RBTree<Key, Value>::begin()
{
    if (root == NULL)
    {
        return NULL;
    }
    RBTNode<Key, Value>* ret = root;
    while (ret->leftChild)
    {
        ret = ret->leftChild;
    }
    return ret;
}

template<class Key, class Value>
RBTNode<Key, Value>* RBTree<Key, Value>::end()
{
    if (root == NULL)
    {
        return NULL;
    }
    RBTNode<Key, Value>* ret = root;
    while (ret->rightChild)
    {
        ret = ret->rightChild;
    }
    return ret;
}

template<class Key, class Value>
unsigned int RBTree<Key, Value>::size()
{
    return size(root);
}

template<class Key, class Value>
unsigned int RBTree<Key, Value>::size(RBTNode<Key, Value>* node)
{
    if (node == NULL)
    {
        return 0;
    }
    return (size(node->leftChild) + size(node->rightChild) + 1);
}

template<class Key, class Value>
bool RBTree<Key, Value>::empty()
{
    if (root == NULL)
    {
        return true;
    }
    return false;
}


template<class Key, class Value>
bool RBTree<Key, Value>::isBST(RBTNode<Key, Value>* node)
{
    if (node == NULL)
    {
        return true;
    }

    bool flagL = true, flagR = true;
    if (node->leftChild != NULL)
    {
        if (node->leftChild->key >= node->key)
        {
            flagL = false;
        }
    }

    if (node->rightChild != NULL)
    {
        if (node->key >= node->rightChild->key)
        {
            flagR = false;
        }
    }

    if (flagL == false || flagR == false)
    {
        return false;
    }

    if (isBST(node->leftChild) && isBST(node->rightChild))
    {
        return true;
    }

    return false;
}


template<class Key, class Value>
bool RBTree<Key, Value>::isRBT()
{
    if (root == NULL)
    {
        return true;
    }

    if (!isBST(root))
    {
        std::cout << "Not a BST!" << std::endl;
        return false;
    }

    if (root->color == RED)
    {
        std::cout << "Root is red!" << std::endl;
        return false;
    }

    if (hasTwoRed(root))
    {
        return false;
    }

    if (!hasSameBlack(root))
    {
        return false;
    }

    return true;
}


template<class Key, class Value>
bool RBTree<Key, Value>::hasTwoRed(RBTNode<Key, Value>* node)
{
    if (node == NULL)
    {
        return false;
    }

    if (node != root)
    {
        if (node->father->color == RED && node->color == RED)
        {
            std::cout << "Has tow red node!" << std::endl;
            std::cout << "node key = " << node->key << std::endl;
            return true;
        }
    }

    if (hasTwoRed(node->leftChild) || hasTwoRed(node->rightChild))
    {
        return true;
    }

    return false;
}


template<class Key, class Value>
bool RBTree<Key, Value>::hasSameBlack(RBTNode<Key, Value>* node)
{
    if (node == NULL)
    {
        return true;
    }

    node->blackNum = -1;

    if (node->leftChild == NULL && node->rightChild == NULL)
    {
        calBlackNum(node, 0);
        return true;
    }

    if (node->leftChild)
    {
        if (!hasSameBlack(node->leftChild))
        {
            return false;
        }
    }

    if (node->rightChild)
    {
        if (!hasSameBlack(node->rightChild))
        {
            return false;
        }
    }

    if (node->blackNum == -2)
    {
        std::cout << "Don't has same black node!" << std::endl;
        std::cout << "node key = " << node->key << std::endl;
        return false;
    }

    return true;
}


template<class Key, class Value>
void RBTree<Key, Value>::calBlackNum(RBTNode<Key, Value>* node, int blackNum)
{
    if (node == NULL)
    {
        return;
    }

    if (node->color == BLACK)
    {
        if (node->blackNum == -1)
        {
            node->blackNum = ++blackNum;
        }
        else if (node->blackNum != -2)
        {
            if (node->blackNum != ++blackNum)
            {
                node->blackNum = -2;
            }
        }
    }

    if (node->father != NULL)
    {
        calBlackNum(node->father, blackNum);
    }
}
