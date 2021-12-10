#pragma once
#include "KNode.h"
template <class Type>
class KTemplateLinkedList
{
public:
	KNode<Type>*	m_pHead;
	KNode<Type>*	m_pTail;
	KNode<Type>*	m_pEnd;
public:
	void	AddLink(Type* pValue);
	void	AddLinkBack(Type* pValue);
	int		DeleteLink(Type* pNode, int sel);
	void	DeletePop();
	Type*	FindData(int sel);
	void	PrintAll();

public:
	//������ �ʱ�ȭ �ܰ迡�� �Ӹ� �ڿ� ���� ���̰�, ���� �Ӹ��̰�
	KTemplateLinkedList(void);
	~KTemplateLinkedList(void);
};
//�߰�
template < class Type >
void KTemplateLinkedList<Type>::AddLink(Type* pValue)
{
	KNode<Type>* pNode = new KNode<Type>();
	pNode->m_pValue = pValue;

	KNode<Type>* pTemp = m_pHead->m_pNext;
	m_pHead->m_pNext = pNode;
	pNode->m_pNext = pTemp;
	std::cout << "";
}

//�ӽ÷� ��� next ���� - > ��� next�� �� ��� -> �� ��� ������ �ӽ÷� �����ߴ� next
template < class Type >
void KTemplateLinkedList<Type>::AddLinkBack(Type* pNode)
{
	m_pEnd->m_pNext = pNode;
	pNode->m_pNext = m_pTail;
	m_pEnd = pNode;
}

//����
template < class Type >
int	KTemplateLinkedList<Type>::DeleteLink(Type* pNode, int sel)
{
	//���� ���� ����� ���
	if (m_pHead->m_pNext == m_pEnd)
	{
		return 999;
	}
	//�������� ��� �����϶�
	else if (m_pHead->m_pNext == pNode)
	{
		KNode<Type>* pTemp = pNode->m_pNext;
		m_pHead->m_pNext = pTemp;
		delete pNode;
		return 1;
	}
	//�������� ������ ���϶�
	else if (m_pEnd == pNode)
	{
		KNode<Type>* pTemp = m_pEnd->m_pNext;
		m_pEnd = pTemp;
		return 1;
	}
	else
	{
		if (sel > 1)
		{
			KNode<Type>* pTemp = pNode->m_pNext;
			FindData(sel - 1)->m_pNext = pTemp;
			delete pNode;
			return pTemp->m_pValue;
		}
	}
}

template < class Type >
void KTemplateLinkedList<Type>::DeletePop()
{
	//�Ӹ� �ڰ�
	KNode<Type>* pTemp = m_pHead->m_pNext;
	if (pTemp->m_pNext != m_pTail)
	{
		m_pHead->m_pNext = pTemp->m_pNext;
		delete pTemp;
	}
}

//�˻�
template < class Type >
Type* KTemplateLinkedList<Type>::FindData(int sel)
{
	int temp = 0;
	for (KNode<Type>* pNode = m_pHead->m_pNext; pNode->m_pNext != m_pTail; pNode = pNode->m_pNext)
	{
		temp++;
		if (temp == sel)
		{
			return pNode;
		}
	}
};

//���
template < class Type >
void KTemplateLinkedList<Type>::PrintAll()
{
	for (KNode<Type>* pNode = m_pHead->m_pNext;
		pNode != m_pTail;
		pNode = pNode->m_pNext)
	{
		std::cout << pNode->m_pValue->m_Name<< "\n";
	}
}

//������ �ʱ�ȭ �ܰ迡�� �Ӹ� �ڿ� ���� ���̰�, ���� �Ӹ��̰�
template < class Type >
KTemplateLinkedList<Type>::KTemplateLinkedList()
{
	m_pHead = new KNode<Type>;
	m_pTail = new KNode<Type>;
	m_pEnd = new KNode<Type>;

	m_pHead->m_pValue =nullptr;
	m_pHead->m_pNext = nullptr;
	m_pTail->m_pValue = nullptr;
	m_pTail->m_pNext = nullptr;

	m_pHead->m_pNext = m_pTail;
	m_pEnd = m_pHead;
}

template < class Type >
KTemplateLinkedList<Type>::~KTemplateLinkedList()
{
	for (KNode<Type>* pNode = m_pHead->m_pNext; pNode != m_pTail;)
	{
		KNode<Type>* pNext = pNode->m_pNext;
		delete pNode;
		pNode = pNext;
	}
	delete m_pHead;
	delete m_pTail;
	delete m_pEnd;
}


