#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include <assert.h>

#define SAFE_ZERO(A) { A = NULL;}
#define SAFE_NEW(A, B) {if (!A) A = new B;}
#define SAFE_DEL(A)  {if (A) delete A; (A) = NULL; }
#define SAFE_NEW_CLEAR(A, B) {if (!A) A = new B; if(A) memset(A, 0, sizeof(B));}
#define SAFE_DEL(A)  {if (A) delete A; (A) = NULL;}
#define SAFE_NEW_ARRAY(A, B, C) {if (!A) A = new B[C];}
#define SAFE_NEW_ARRAY_CLEAR(A, B, C) {if (!A) A = new B[C];if(A) memset(A, 0, sizeof(B)*C);}
#define SAFE_DEL_ARRAY(A)  {if (A) delete [] A; (A) = NULL;}


struct TStudent
{
	char m_szName[4];
	int  m_iIndex;
	int  m_iKor;
	int  m_iEng;
	int  m_iMat;
	int  m_iTotal;	
	float m_fAverage;
	TStudent* m_pPrev;
	TStudent* m_pNext;
};

TStudent* g_pHead = 0;
TStudent* g_pTail = 0;

int       g_iNumTotal = 0;
// 연결리스트 추가
void AddLink(TStudent* pNode);
TStudent* DelLink(TStudent* pNode);
void AllDelLink();
void AddLink(TStudent* pNewNode)
{
	//head->newnode->tail;
	TStudent* pPreLink = g_pTail->m_pPrev;
	pPreLink->m_pNext = pNewNode;
	pNewNode->m_pNext = g_pTail;
	
	//tail-> newnode->head;
	g_pTail->m_pPrev = pNewNode;
	pNewNode->m_pPrev = pPreLink;
}
void InsertLink(TStudent* pPostNode, TStudent* pNewNode)
{
	//head->newnode->tail;
	TStudent* pPreLink = pPostNode->m_pPrev;
	pPreLink->m_pNext = pNewNode;
	pNewNode->m_pNext = pPostNode;

	//tail-> newnode->head;
	pPostNode->m_pPrev = pNewNode;
	pNewNode->m_pPrev = pPreLink;
}
// A->  ->C
TStudent* DelLink(TStudent* pNode)
{
	TStudent* pPreLink = pNode->m_pPrev; // A
	TStudent* pPostLink = pNode->m_pNext;// C

	//head->newnode->tail;
	pPreLink->m_pNext = pPostLink;
	//tail-> newnode->head;
	pPostLink->m_pPrev = pPreLink;
	SAFE_DEL(pNode);
	return pPostLink;
}
void AllDelLink()
{
	TStudent* pDelNode = g_pHead->m_pNext;
	while (pDelNode != g_pTail)
	{
		pDelNode = DelLink(pDelNode);
	}
}

/*pCurrent노드의 앞에 pNode를 삽입한다.
  단,pCurrent == nullptr 가장 앞에 추가된다.
  만약 pNode==nullptr 이면 초기화된 TNode를 생성하여 연결한다.
*/
TStudent* FrontInsert(
	TStudent* pNewNode,
	TStudent* pCurrent = nullptr)
{
	if (pNewNode == nullptr)
	{
		pNewNode = new TStudent;
		memset(pNewNode, 0, sizeof(TStudent));
		assert(pNewNode);
	}
	if (pCurrent == g_pHead) return nullptr;

	if (pCurrent == nullptr)
	{
		pCurrent = g_pHead->m_pNext;
	}
	// A<-New<-C
	TStudent* pCurrentPrev = pCurrent->m_pPrev;
	pNewNode->m_pPrev = pCurrentPrev;
	pCurrent->m_pPrev = pNewNode;

	// A(g_pPrevFind)->New->C(pCurrent)
	pCurrentPrev->m_pNext = pNewNode;
	pNewNode->m_pNext = pCurrent;
	return pNewNode;
}
/*pCurrent노드의 뒤에 pNode를 삽입한다.
단,pCurrent == nullptr 가장 뒤에 추가된다.
만약 pNode==nullptr 이면 초기화된 TNode를 생성하여 연결한다.
*/
TStudent* BackInsert(TStudent* pNewNode, TStudent* pCurrent = nullptr)
{
	if (pNewNode == nullptr)
	{
		pNewNode = new TStudent;
		memset(pNewNode, 0, sizeof(TStudent));
		assert(pNewNode);
	}
	if (pCurrent == g_pTail) return nullptr;

	if (pCurrent == nullptr)
	{
		pCurrent = g_pTail->m_pPrev;
		assert(pCurrent);
	}
	// H->new->A->T
	TStudent* pNext = pCurrent->m_pNext;
	pCurrent->m_pNext = pNewNode;
	pNewNode->m_pNext = pNext;

	// H<-new<-A<-T
	pNewNode->m_pPrev = pCurrent;
	pNext->m_pPrev = pNewNode;
	return pNewNode;
}
