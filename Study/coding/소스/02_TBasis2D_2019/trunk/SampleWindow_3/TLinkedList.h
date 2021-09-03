#pragma once
#include "TNode.h"

template < class T >
class TLinkedList
{
public:
	int		m_iCount;
	TNode<T>*		m_pHead;
	TNode<T>*		m_pTail;
	TNode<T>*		GetHead();
	T*				GetData(int iIndex)
	{
		TNode<T>* pNode = m_pHead;
		while (pNode != NULL)
		{
			if (pNode->m_pData->m_iIndex
				== iIndex)
			{
				return pNode->m_pData;
			}
			pNode = pNode->m_pNext;
		}
		return NULL;
	};
	T*				GetData(TCHAR* pszData)
	{
		TNode<T>* pNode = m_pHead;
		while (pNode != NULL)
		{
			if (_tcsicmp(pNode->m_pData->m_szName,
				pszData) == 0)
			{
				return pNode->m_pData;
			}
			pNode = pNode->m_pNext;
		}
		return NULL;
	};

	bool	AddLink(T* pLink);
	void	Release();
	void    Delete(int iIndex) {};
	void    Delete(T* iData) {};
	void    Delete(TNode<T>* pNode) {};
public:
	TLinkedList();
	~TLinkedList();
};
#pragma region func
template <class T>
void	TLinkedList<T>::Release()
{
	TNode<T>* pDelNode = 0;
	TNode<T>* pNode = m_pHead;
	while (pNode != NULL)
	{
		pDelNode = pNode;
		pNode = pNode->m_pNext;
		delete pDelNode;
	}
	m_pHead = 0;
	m_pTail = 0;
}
template <class T>
TNode<T>*	TLinkedList<T>::GetHead()
{
	return m_pHead;
}
template <class T>
bool	TLinkedList<T>::AddLink(T* pData)
{
	TNode<T>*  pNode = new TNode<T>();
	pNode->m_pData = pData;
	if (m_pHead == NULL)
	{
		m_pHead = pNode;
		m_pTail = pNode;
	}
	else
	{
		TNode<T>* pTemp = m_pHead;
		m_pHead = pNode;
		m_pHead->m_pNext = pTemp;
		pTemp->m_pPrev = pNode;
	}
	m_iCount++;
	return true;
}
template <class T>
TLinkedList<T>::TLinkedList()
{
	m_iCount = 0;
	m_pHead = NULL;
	m_pTail = NULL;
}
template <class T>
TLinkedList<T>::~TLinkedList()
{
}
#pragma endregion