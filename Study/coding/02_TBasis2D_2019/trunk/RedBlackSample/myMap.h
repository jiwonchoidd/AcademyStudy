#pragma once
#include "rbTree.cpp"
template<class Key, class Value>
class myMap
{
private:
    RBTree<Key, Value>* rbt;

public:
    myMap();
    ~myMap();

public:
    RBTNode<Key, Value>* begin();
    RBTNode<Key, Value>* end();
    void clear();
    void insert( Key key, Value value );
    void erase( Key key );
    unsigned int size();
    RBTNode<Key, Value>* find( Key key );
    bool empty();
    Value& operator[] ( Key key );
};
//#include "myMap.cpp"
