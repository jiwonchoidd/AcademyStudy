#include "myMap.h"
template<class Key, class Value>
myMap<Key, Value>::myMap()
{
    rbt = new RBTree<Key, Value>();
}

template<class Key, class Value>
myMap<Key, Value>::~myMap()
{
    delete rbt;
}

template<class Key, class Value>
RBTNode<Key, Value>* myMap<Key, Value>::begin()
{
    return rbt->begin();
}

template<class Key, class Value>
RBTNode<Key, Value>* myMap<Key, Value>::end()
{
    return rbt->end();
}

template<class Key, class Value>
void myMap<Key, Value>::clear()
{
    rbt->clear();
}

template<class Key, class Value>
void myMap<Key, Value>::insert( Key key, Value value )
{
    rbt->insert( key, value );
}

template<class Key, class Value>
void myMap<Key, Value>::erase( Key key )
{
    rbt->erase( key );
}

template<class Key, class Value>
unsigned int myMap<Key, Value>::size()
{
    return rbt->size();
}

template<class Key, class Value>
RBTNode<Key, Value>* myMap<Key, Value>::find( Key key )
{
    return find( key );
}

template<class Key, class Value>
bool myMap<Key, Value>::empty()
{
    return rbt->empty();
}

template<class Key, class Value>
Value& myMap<Key, Value>::operator[] ( Key key )
{
    return rbt->insertUnique( key )->value;
}
/*
#include <string>
#include <iostream>
using namespace std;

int main( int argc, char* argv[] )
{
    myMap<int, int> m;
    string s = "5";
    m.insert(1, 100);
    m.insert(2, 32);
    m.insert(3, 11);
    m.insert(11, 44);
    cout << m.end()->value << endl;
    m.erase(2);
    return 0;
}
*/