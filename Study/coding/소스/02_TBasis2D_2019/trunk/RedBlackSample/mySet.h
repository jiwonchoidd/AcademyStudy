#pragma once
#include "rbTree.cpp"
template<class Key>
class mySet
{
private:
    RBTree<Key, Key>* rbt;

public:
    mySet();
    ~mySet();

public:
    RBTNode<Key, Key>* begin();
    RBTNode<Key, Key>* end();
    void clear();
    void insert( Key key );
    void erase( Key key );
    unsigned int size();
    RBTNode<Key, Key>* find( Key key );
    bool empty();
};
//#include "mySet.cpp"
