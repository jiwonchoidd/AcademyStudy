#pragma once
#include "TLinkedList.h"

enum WORK {
	SCREEN_PRINT = 0,
	NEW_DATA,
	FILE_SAVE,
	FILE_LOAD,
	DATA_UPDATE,
	DEL_DATA,
	SORT_DATA,
	CREATE_SAMPLE_DATA = 9,
	EXIT = 99
};

//
//
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
// 정렬
void SortData(int(*fp)(TStudent* pA, TStudent* pB));
//// 샘플용 학생 정보 생성(10명)
void CreateSampleData();
//// 인덱스 검색
TStudent* FindIndex(int iIndex);
TStudent* FindName(char* pszName);

int(*SortFunction)(TStudent* pA, TStudent* pB);

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
	printf("\n출력(0),신규(1),저장(2),로드(3),수정(4),삭제(5),정렬(6),샘플용(9),종료(99)");
}
//
int ScreenPrint()
{
	for (TStudent* pNode = g_pHead->m_pNext;
		pNode != g_pTail;
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
	FILE* fp = fopen("demo.txt", "w");
	fprintf(fp, "%d", g_iNumTotal);
	for (TStudent* pNode = g_pHead->m_pNext;
		pNode != g_pTail;
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
		TStudent* pNewData = 0;
		SAFE_NEW(pNewData, TStudent);
		fscanf(fp, "%s %d %d %d %d %d %f",
			pNewData->m_szName,
			&pNewData->m_iIndex,
			&pNewData->m_iKor,
			&pNewData->m_iEng,
			&pNewData->m_iMat,
			&pNewData->m_iTotal,
			&pNewData->m_fAverage);
		AddLink(pNewData);
	}
	fclose(fp);
	return 1;
}
void CreateSampleData()
{
	g_iNumTotal = 10;
	AllDelLink();

	for (int iData = 0; iData < g_iNumTotal; iData++)
	{
		TStudent* pNode = NULL;
		SAFE_NEW_CLEAR(pNode, TStudent);
		pNode->m_szName[0] = rand() % 26 + 65;
		pNode->m_szName[1] = rand() % 26 + 65;
		pNode->m_szName[2] = rand() % 26 + 65;
		pNode->m_szName[3] = 0;
		pNode->m_iIndex = iData;
		pNode->m_iKor = rand() % 101;
		pNode->m_iEng = rand() % 101;
		pNode->m_iMat = rand() % 101;
		pNode->m_iTotal =
			pNode->m_iKor +
			pNode->m_iEng +
			pNode->m_iMat;

		pNode->m_fAverage =
			pNode->m_iTotal / 3.0f;

		AddLink(pNode);
	}
}
void DelData()
{
	system("cls");
	int iIndex;
	printf("\n번호을 입력 : "); scanf("%d", &iIndex);
	DelLink(FindIndex(iIndex));
}
int NewData()
{
	TStudent* newData = 0;
	SAFE_NEW_CLEAR(newData, TStudent);
	newData->m_iIndex = g_iNumTotal++;
	printf("\n이름: "); scanf("%s", newData->m_szName);
	printf("\n국어: "); scanf("%d", &newData->m_iKor);
	printf("\n영어: "); scanf("%d", &newData->m_iEng);
	printf("\n수학: "); scanf("%d", &newData->m_iMat);
	newData->m_iTotal = newData->m_iKor +
		newData->m_iEng + newData->m_iMat;
	newData->m_fAverage = newData->m_iTotal / 3.0f;

	AddLink(newData);
	ScreenPrint();
	return 1;
}
TStudent* FindIndex(int iIndex)
{
	for (TStudent* pNode = g_pHead->m_pNext;
		pNode != g_pTail;
		pNode = pNode->m_pNext)
	{
		if (iIndex == pNode->m_iIndex)
		{
			return pNode;
		}
	}
	return g_pTail;
}
TStudent* FindName(char* pszName)
{
	// 검색
	for (TStudent* pNode = g_pHead->m_pNext;
		pNode != g_pTail;
		pNode = pNode->m_pNext)
	{
		if (!_stricmp(pszName, pNode->m_szName))
		{
			return pNode;
		}
	}
	return g_pTail;
}
int UpdateData()
{
	system("cls");
	char szName[5] = { 0, };
	int iResultndex = -1;
	printf("\n이름을 입력 : "); scanf("%s", szName);

	TStudent* pFind = FindName(szName);
	if (pFind == g_pTail) return -1;

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
void Swap(TStudent* pA, TStudent* pB)
{
	strcpy(pA->m_szName, pB->m_szName);
	pA->m_iIndex = pB->m_iIndex;
	pA->m_iKor = pB->m_iKor;
	pA->m_iEng = pB->m_iEng;
	pA->m_iMat = pB->m_iMat;
	pA->m_iTotal = pB->m_iTotal;
	pA->m_fAverage = pB->m_fAverage;
}
// NULL ==> 0 정수값 포인터 널 =  nullptr;
void SortData(int iValue)
{
	int k;
}
// 연결리스트 구축 후에 노드들을 정렬한다.
void SortData(int(*fp)(TStudent* pA, TStudent* pB))
{
	if (fp != nullptr) SortFunction = fp;
	TStudent pTmp;
	for (TStudent* pStartNode = g_pHead->m_pNext;
		pStartNode != g_pTail;
		pStartNode = pStartNode->m_pNext)
	{
		TStudent* pMinNode = pStartNode;
		for (TStudent* pNode = pStartNode->m_pNext;
			pNode != g_pTail;
			pNode = pNode->m_pNext)
		{
			if (SortFunction(pMinNode, pNode))
			{
				pMinNode = pNode;
			}
		}
		Swap(&pTmp, pStartNode);
		Swap(pStartNode, pMinNode);
		Swap(pMinNode, &pTmp);
	}
	ScreenPrint();
}
