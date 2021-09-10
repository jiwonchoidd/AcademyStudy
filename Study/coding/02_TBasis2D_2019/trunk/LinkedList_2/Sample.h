#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include <time.h>

struct TStudent
{
	char m_szName[4];
	int  m_iIndex;
	int  m_iKor;
	int  m_iEng;
	int  m_iMat;
	int  m_iTotal;
	float m_fAverage;
	TStudent* m_pNext;
	TStudent* m_pPrev;
};

TStudent* g_pHead = 0;
TStudent* g_pPreNode = 0;
TStudent* g_pTail = 0;
int       g_iNumTotal = 0;

enum WORK {
	SCREEN_PRINT = 0,
	NEW_DATA,
	FILE_SAVE,
	FILE_LOAD,
	DATA_UPDATE,
	CREATE_SAMPLE_DATA = 9,
	EXIT = 99
};

int(*SORTFUNCTION) (TStudent* pA, TStudent* pB);

//// 학생 정보 화면 출력
int  ScreenPrint();
//// 신규 학생 입력
int NewData();
//// 파일 저장
int FileSave();
//// 파일 로드
int FileLoad();
//// 학생 정보 수정
int UpdateData();
//// 샘플용 학생 정보 생성(10명)
void CreateSampleData();
//// 인덱스 검색
TStudent* FindIndex(int iIndex);
TStudent* FindName(char* pszName);

void InsertLink(TStudent* pNode);
void DelLink(TStudent* pNode);
void AllDelLink();
int Ascending(TStudent* pA, TStudent* pB);

void InsertLink(TStudent* pNewNode)
{
	g_pPreNode = g_pHead;
	g_pHead = pNewNode;
	pNewNode->m_pNext = g_pPreNode;
}

TStudent* FindPrevIndex(TStudent* pNewNode)
{
	// Head만 검색
	if (g_pHead == NULL ||
		SORTFUNCTION(g_pHead, pNewNode))
	{
		return NULL;
	}
	// g_pHead->m_pNext 부터 검색
	for (TStudent* pNode = g_pHead;
		pNode != NULL;
		pNode = pNode->m_pNext)
	{
		if (pNode->m_pNext == NULL) break;
		if (SORTFUNCTION(pNode->m_pNext, pNewNode))
		{
			return  pNode;
		}
	}	
	return NULL;
}
//  신규노드를 정렬하여 연결리스트 구축한다.
void SortLink(TStudent* pNewNode)
{
	if (g_pHead == NULL)
	{
		g_pHead = pNewNode;
		g_pTail = g_pHead;
		return;
	}
	TStudent* pFind = FindPrevIndex(pNewNode);
	if (pFind != NULL)
	{
		TStudent* pNext = pFind->m_pNext;
		pFind->m_pNext = pNewNode;
		pNewNode->m_pNext = pNext;
	}
	else
	{
		// Head Link
		if (SORTFUNCTION(g_pHead, pNewNode))
		{
			g_pPreNode = g_pHead;
			g_pHead = pNewNode;
			pNewNode->m_pNext = g_pPreNode;			
		}
		else
		{
			// Tail Link
			g_pTail->m_pNext = pNewNode;
			g_pTail = pNewNode;
		}
	}
}
// A->  ->C->null
void DelLink(TStudent* pPreNode)
{
	TStudent* pDel = pPreNode->m_pNext;
	if (pDel != NULL)
	{
		TStudent* pTmp = pDel->m_pNext;
		free(pDel);
		pPreNode->m_pNext = pTmp;
	}
}

void AllDelLink()
{
	TStudent* pLink = g_pHead;
	while (pLink != NULL && pLink->m_pNext != NULL)
	{
		DelLink(pLink);
	}
	free(g_pHead);
	g_pHead = NULL;
	g_pPreNode = NULL;
}

int Ascending(TStudent* pA, TStudent* pB)
{
	if (pA->m_iTotal > pB->m_iTotal)
	{
		return 1;
	}
	return 0;
}
int Descending(TStudent* pA, TStudent* pB)
{
	if (pA->m_iTotal < pB->m_iTotal)
	{
		return 1;
	}
	return 0;
}


//
void MenuPrint()
{
	printf("\n출력(0),신규(1),저장(2),로드(3),수정(4),샘플용(9),종료(99)");
}
//
int ScreenPrint()
{
	if (g_pHead == 0) return -1;
	for (TStudent* pNode = g_pHead;
		pNode != NULL;
		pNode = pNode->m_pNext)
	{
		printf("\n %s %d %d %d %d %d %10.4f",
			pNode->m_szName,
			pNode->m_iIndex,
			pNode->m_iKor,
			pNode->m_iEng,
			pNode->m_iMat,
			pNode->m_iTotal,
			pNode->m_fAverage);
	}
	return 1;
}
//// 파일 저장
int FileSave()
{
	if (g_pHead == 0) return -1;

	FILE* fp = fopen("demo.txt", "w");
	fprintf(fp, "%d", g_iNumTotal);
	for (TStudent* pNode = g_pHead;
		pNode != NULL;
		pNode = pNode->m_pNext)
	{
		fprintf(fp, "\n %s %d %d %d %d %d %10.4f",
			pNode->m_szName,
			pNode->m_iIndex,
			pNode->m_iKor,
			pNode->m_iEng,
			pNode->m_iMat,
			pNode->m_iTotal,
			pNode->m_fAverage);
	}
	fclose(fp);
	return 1;
}
//// 파일 로드
int FileLoad()
{
	AllDelLink();

	FILE* fp = fopen("demo.txt", "r");
	if (fp == NULL) return -1;
	fscanf(fp, "%d", &g_iNumTotal);

	for (int iData = 0; iData < g_iNumTotal; iData++)
	{
		TStudent* pNewData = (TStudent*)malloc(sizeof(TStudent));
		fscanf(fp, "%s %d %d %d %d %d %f",
			pNewData->m_szName,
			&pNewData->m_iIndex,
			&pNewData->m_iKor,
			&pNewData->m_iEng,
			&pNewData->m_iMat,
			&pNewData->m_iTotal,
			&pNewData->m_fAverage);
		InsertLink(pNewData);
	}
	fclose(fp);
	return 1;
}
void CreateSampleData(int(*fp)(TStudent* pA, TStudent* pB))
{
	g_iNumTotal = 10;
	AllDelLink();

	for (int iData = 0; iData < g_iNumTotal; iData++)
	{
		TStudent* pNode = (TStudent*)malloc(sizeof(TStudent));
		pNode->m_szName[0] = rand() % 26 + 65;
		pNode->m_szName[1] = rand() % 26 + 65;
		pNode->m_szName[2] = rand() % 26 + 65;
		pNode->m_szName[3] = 0;
		pNode->m_iIndex = rand() % 100;
		pNode->m_iKor = rand() % 101;
		pNode->m_iEng = rand() % 101;
		pNode->m_iMat = rand() % 101;
		pNode->m_iTotal =
			pNode->m_iKor +
			pNode->m_iEng +
			pNode->m_iMat;

		pNode->m_fAverage =
			pNode->m_iTotal / 3.0f;

		pNode->m_pNext = NULL;

		SORTFUNCTION = fp;
		SortLink(pNode);
	}
}

int NewData()
{
	TStudent newData;
	memset(&newData, 0, sizeof(TStudent));
	printf("\n이름: "); scanf("%s", newData.m_szName);
	printf("\n국어: "); scanf("%d", &newData.m_iKor);
	printf("\n영어: "); scanf("%d", &newData.m_iEng);
	printf("\n수학: "); scanf("%d", &newData.m_iMat);
	newData.m_iTotal = newData.m_iKor +
		newData.m_iEng + newData.m_iMat;
	newData.m_fAverage = newData.m_iTotal / 3.0f;

	FILE* fp = fopen("demo.txt", "r+");
	fprintf(fp, "%d", g_iNumTotal + 1);
	fseek(fp, 0, SEEK_END);
	fprintf(fp, "\n %s %d %d %d %d %d %f",
		newData.m_szName,
		g_iNumTotal + 1,
		newData.m_iKor,
		newData.m_iEng,
		newData.m_iMat,
		newData.m_iTotal,
		newData.m_fAverage);
	fclose(fp);

	FileLoad();
	ScreenPrint();
	return 1;
}
TStudent* FindIndex(int iIndex)
{
	TStudent* pFind = 0;
	// 검색
	for (TStudent* pNode = g_pHead;
		pNode != NULL;
		pNode = pNode->m_pNext)
	{
		if (iIndex == pNode->m_iIndex)
		{
			pFind = pNode;
			break;
		}
	}
	if (pFind == 0) return NULL;
	return pFind;
}
TStudent* FindName(char* pszName)
{
	TStudent* pFind = 0;
	// 검색
	for (TStudent* pNode = g_pHead;
		pNode != NULL;
		pNode = pNode->m_pNext)
	{
		if (!_stricmp(pszName, pNode->m_szName))
		{
			pFind = pNode;
			break;
		}
	}
	if (pFind == 0) return NULL;
	return pFind;
}
int UpdateData()
{
	system("cls");
	char iIndex[5] = { 0, };
	int iResultndex = -1;
	printf("\n이름을 입력 : "); scanf("%s", iIndex);

	TStudent* pFind = FindName(iIndex);
	if (pFind == NULL) return -1;

	printf("\n %s %d %d %d %d %d %f",
		pFind->m_szName,
		pFind->m_iIndex,
		pFind->m_iKor,
		pFind->m_iEng,
		pFind->m_iMat,
		pFind->m_iTotal,
		pFind->m_fAverage);

	// 수정
	printf("\n이름: "); scanf("%s", pFind->m_szName);
	printf("\n국어: "); scanf("%d", &pFind->m_iKor);
	printf("\n영어: "); scanf("%d", &pFind->m_iEng);
	printf("\n수학: "); scanf("%d", &pFind->m_iMat);
	pFind->m_iTotal =
		pFind->m_iKor +
		pFind->m_iEng +
		pFind->m_iMat;
	pFind->m_fAverage = pFind->m_iTotal / 3.0f;

	// 저장
	system("cls");
	ScreenPrint();
	return 1;
}