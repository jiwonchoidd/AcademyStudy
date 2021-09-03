#pragma once
//데이터 취급

#include<iostream>
template <class T>
class DNode
{
public :
	T m_iValue;
	DNode* m_pNext;
};
class DLinkedList
{
	public:
		DNode* m_pHead;
		DNode* m_pTail;
		DNode* m_pEnd;
public:
	void	Show();
	int		Pop();
	void   AddForwardLink(DNode* pNew);
	void   AddBackwardLink(DNode* pNew);
		//생성자
		DLinkedList();
		//해제지
		~DLinkedList();
};

