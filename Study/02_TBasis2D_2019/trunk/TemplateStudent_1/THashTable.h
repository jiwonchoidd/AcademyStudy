#pragma once
#include "TLinkedList.h"
//#define GetRValue(rgb)      (LOBYTE(rgb))
//#define GetGValue(rgb)      (LOBYTE(((WORD)(rgb)) >> 8))
//#define GetBValue(rgb)      (LOBYTE((rgb)>>16))
//#define LOWORD(l)           ((WORD)(((DWORD_PTR)(l)) & 0xffff))
//#define HIWORD(l)           ((WORD)((((DWORD_PTR)(l)) >> 16) & 0xffff))
//#define LOBYTE(w)           ((BYTE)(((DWORD_PTR)(w)) & 0xff))
//#define HIBYTE(w)           ((BYTE)((((DWORD_PTR)(w)) >> 8) & 0xff))
#define MAKECOLOR_ARGB(a, r, g, b)			(((a)&0xff)<<24)|(((r)&0xff)<<16)|(((g)&0xff)<<8)|((b)&0xff)
template < class T >
class THashTable
{
public:
	TLinkedList<T>*		m_pArray;
	int					m_iLength;
	int					Hash(TCHAR* pName);
	void				Insert(T* newItem);
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
	int iIndex = Hash(const_cast<TCHAR*>(newItem->GetName().c_str()));
	m_pArray[iIndex].AddLink(newItem);
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