#pragma once
#include <iostream>
#include <tchar.h>

template < class T >
class TNode
{
public:
	T*			m_pData;
	TNode*		m_pNext;
	TNode*		m_pPrev;
	void	  Delete()
	{
		delete m_pData;
	}
public:
	TNode();
	~TNode();
};
template < class T >
TNode<T>::TNode(void)
{
	m_pData = NULL;
	m_pNext = m_pPrev = 0;
};
template < class T >
TNode<T>::~TNode(void)
{
	delete m_pData;
	//cout << "TNode end" << endl;
};