#include "TLinkedList.h"

#pragma region 생성자 소멸자
TLinkedList::TLinkedList()
{
	//초기화
	m_pHead = new TNode;
	m_pHead->m_iValue = 999;
	m_pHead->m_pNext = nullptr;

	m_pTail = new TNode;
	m_pTail->m_iValue = 666;
	m_pTail->m_pNext = nullptr;

	//머리 뒤에 꼬리 붙이기
	m_pHead->m_pNext = m_pTail;
	//끝이 머리
	m_pEnd = m_pHead;
}

TLinkedList::~TLinkedList()
{
	for (TNode* pNode = m_pHead->m_pNext;
		pNode != m_pTail;)
	{
		TNode* pNext = pNode->m_pNext;
		delete pNode;
		pNode = pNext;
	}
	delete m_pHead;
	delete m_pTail;
}
#pragma endregion

#pragma region 헤더함수
void TLinkedList::AddForwardLink(TNode* pNew)
{
	TNode* pTemp = m_pHead->m_pNext;
	m_pHead->m_pNext= pNew;
	pNew->m_pNext = pTemp;
}
void TLinkedList::AddBackwardLink(TNode* pNew)
{
	//끝에 하나 만들고
	m_pEnd->m_pNext= pNew;
	pNew->m_pNext = m_pTail;
	m_pEnd = pNew;
}
TNode* TLinkedList::Search(int i)
{
	int temp = 0;
	for (TNode* pNode = m_pHead->m_pNext; pNode->m_pNext != m_pTail; pNode = pNode->m_pNext)
	{
		temp++;
		if (temp == i)
		{
			return pNode;
		}
	}
}
int TLinkedList::Killed(TNode* pSel,int i)
{
	//삭제 값이 헤드일 경우
	if (m_pHead->m_pNext==m_pEnd)
	{
		return 999995;
	}
	else if (m_pHead->m_pNext== pSel)
	{
		TNode* pTemp =pSel->m_pNext;
		m_pHead->m_pNext = pTemp;
		delete pSel;
		return m_pHead->m_pNext->m_iValue;
	}
	else if (m_pEnd == pSel)
	{
		TNode* pTemp = m_pEnd->m_pNext;
		m_pEnd = pTemp;
		return m_pEnd->m_iValue;
	}
	else
	{
		if (i > 1)
		{
			TNode* temp = pSel->m_pNext;
			Search(i - 1)->m_pNext = temp;
			delete pSel;
			return temp->m_iValue;
		}
	}
}
int TLinkedList::Pop()
{
	//머리 뒤값
	TNode* pDelNode = m_pHead->m_pNext;
	int iRet = pDelNode->m_iValue;
	m_pHead->m_pNext = pDelNode->m_pNext;
	delete pDelNode;
	return iRet;
}
void TLinkedList::Show()
{
	for (TNode* pNode = m_pHead->m_pNext; 
		pNode != m_pTail;
		pNode=pNode->m_pNext)
	{
		std::cout << pNode->m_iValue << "\n";
	}
}
#pragma endregion

