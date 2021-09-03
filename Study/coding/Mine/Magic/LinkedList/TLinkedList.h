#pragma once
//�������

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

	//�Լ�
	void Show();
	int Pop();
	void AddForwardLink(TNode* pNew);
	void AddBackwardLink(TNode* pNew);
	int Killed(TNode* pNew, int i);
	TNode* Search(int i);
	//������
	TLinkedList();
	~TLinkedList();
};