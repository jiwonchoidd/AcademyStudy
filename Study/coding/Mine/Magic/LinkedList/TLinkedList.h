#pragma once
//헤더파일

struct TNode {
	int		m_iValue;
	TNode*	m_pNext;
};
#include<iostream>
class TLinkedList
{
public :
	TNode* m_pHead;
	TNode* m_pTail;
	TNode* m_pEnd;

	//함수
	void Show();
	int Pop();
	void AddForwardLink(TNode* pNew);
	void AddBackwardLink(TNode* pNew);
	int Killed(TNode* pNew, int i);
	TNode* Search(int i);
	//생성자
	TLinkedList();
	~TLinkedList();
};