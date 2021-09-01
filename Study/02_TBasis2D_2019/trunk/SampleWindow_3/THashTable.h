#pragma once
#include "TLinkedList.h"
template < class T >
class THashTable
{
public:
	TLinkedList<T>*		m_pArray;
	int					m_iLength;
	int					Hash(TCHAR* pName);
	void				Insert(T* newItem);
	T*					Find(TCHAR* pszName);
public:
	THashTable(int iLength = 20);
	~THashTable();
};
template < class T >
int	THashTable<T>::Hash(TCHAR* pName)
{
	int iKey = 5381;
	int c;
	size_t hash = 0x811c9dc5;
	while (c = *pName++)
	{
		hash ^= c;
		hash *= 0x01000193;
	}
	/*int c;
	while (c = *pName++)
	{
	iKey = ((iKey << 5) + iKey) + c;
	}*/
	/*int iValue = 0;
	int iLen = _tcslen(pName);
	for (int i = 0; i < iLen; i++)
	{
	iValue += pName[i];
	}
	int iKey = (iValue * iValue) % m_iLength;*/
	return hash % m_iLength;
}
template < class T >
void THashTable<T>::Insert(T* newItem)
{
	int iIndex = Hash(newItem->m_szName);
	m_pArray[iIndex].AddLink(newItem);
};
template < class T >
T* THashTable<T>::Find(TCHAR* pszName)
{
	int iIndex = Hash(pszName);
	return m_pArray[iIndex].GetData(pszName);
};

template < class T >
THashTable<T>::THashTable(int iLength)
{
	m_pArray = new TLinkedList<T>[iLength];
	m_iLength = iLength;
}
template < class T >
THashTable<T>::~THashTable()
{
	delete[] m_pArray;
}