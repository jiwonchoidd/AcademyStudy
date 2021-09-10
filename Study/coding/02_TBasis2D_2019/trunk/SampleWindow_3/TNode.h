#pragma once
#include <iostream>
#include <tchar.h>
template < class T >
class TNode
{
public:
	T*		m_pData;
	TNode*  m_pNext;
	TNode*  m_pPrev;
public:
	TNode();
	~TNode();
};
template < class T>
TNode<T>::TNode()
{
	m_pData = NULL;
	m_pNext = NULL;
	m_pPrev = NULL;
}
template < class T>
TNode<T>::~TNode()
{
	delete m_pData;
}