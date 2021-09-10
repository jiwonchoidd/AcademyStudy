#include "mySet.h"
template<class Key>
mySet<Key>::mySet()
{
    rbt = new RBTree<Key, Key>();
}

template<class Key>
mySet<Key>::~mySet()
{
    delete rbt;
}

template<class Key>
RBTNode<Key, Key>* mySet<Key>::begin()
{
    return rbt->begin();
}

template<class Key>
RBTNode<Key, Key>* mySet<Key>::end()
{
    return rbt->end();
}

template<class Key>
void mySet<Key>::clear()
{
    rbt->clear();
}

template<class Key>
void mySet<Key>::insert( Key key )
{
    rbt->insertUnique( key );
}

template<class Key>
void mySet<Key>::erase( Key key )
{
    rbt->erase( key );
}

template<class Key>
unsigned int mySet<Key>::size()
{
    return rbt->size();
}

template<class Key>
RBTNode<Key, Key>* mySet<Key>::find( Key key )
{
    return rbt->find( key );
}

template<class Key>
bool mySet<Key>::empty()
{
    return rbt->empty();
}