#pragma once
#include "TNode.h"

template < class T >
class TLinkedList
{
public:
	int				m_iCount;
	TNode<T>*		m_pHead;
	TNode<T>*		m_pTail;
public:
	int		Size() { return m_iCount; }
	bool	AddLink(T* pStud);
	bool	DelLink(T* pStud);
	void    Delete();
	T*		GetData( int iIndex );
	TNode<T>*		GetHead() { return m_pHead; };
	bool	Sort( bool bUp );
public:
	TLinkedList(void);
	~TLinkedList(void);
};
template < class T >
T* TLinkedList<T>::GetData(int iIndex)
{
	TNode<T>*  pNode = m_pHead;
	while( pNode != NULL )
	{
		if( pNode->m_pData->GetIndex() 
			== iIndex )
		{
			T*  pData = pNode->m_pData;
			return pData;
		}
		pNode = pNode->m_pNext;
	}
	return NULL;
}
template < class T >
bool TLinkedList<T>::AddLink(T* pData)
{	
	TNode<T>*  pNode = new TNode<T>();
	pNode->m_pData = pData;

	if( m_pHead == NULL )
	{
		m_pHead = pNode;
		m_pTail = pNode;
	}
	else
	{
		TNode<T>* pFirst = m_pHead;
		m_pHead   = pNode;
		m_pHead->m_pNext = pFirst;
		pFirst->m_pPrev = pNode;
	}
	m_iCount++;
	return true;
}
template < class T >
bool TLinkedList<T>::DelLink(T* pStud)
{
	return true;
};
template < class T >
void TLinkedList<T>::Delete()
{
	TNode<T>* pDelNode=0;
	TNode<T>* pNode = m_pHead;
	while( pNode != NULL )
	{
		pDelNode = pNode;
		pNode = pNode->m_pNext;
		delete pDelNode;
	}
	m_pHead = 0;
	m_pTail  = 0;
};
template < class T >
TLinkedList<T>::TLinkedList(void)
{
	m_pHead = NULL;
	m_pTail	= NULL;
	m_iCount = 0;
};
template < class T >
TLinkedList<T>::~TLinkedList(void)
{
	Delete();
	//cout << "linkedlist end" << endl;
};
template < class T >
bool TLinkedList<T>::Sort(bool bUp)
{
	//  Á¤·Ä	
}