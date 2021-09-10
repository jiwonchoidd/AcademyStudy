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
//// �л� ���� ȭ�� ���
int  ScreenPrint();
//// �ű� �л� �Է�
int NewData();
//// ���� ����
int FileSave();
//// ���� �ε�
int FileLoad();
//// �л� ���� ����
int UpdateData();
// ����
void SortData(int(*fp)(TStudent* pA, TStudent* pB));
//// ���ÿ� �л� ���� ����(10��)
void CreateSampleData();
//// �ε��� �˻�
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
	printf("\n���(0),�ű�(1),����(2),�ε�(3),����(4),����(5),����(6),���ÿ�(9),����(99)");
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
//// ���� ����
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
//// ���� �ε�
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
	printf("\n��ȣ�� �Է� : "); scanf("%d", &iIndex);
	DelLink(FindIndex(iIndex));
}
int NewData()
{
	TStudent* newData = 0;
	SAFE_NEW_CLEAR(newData, TStudent);
	newData->m_iIndex = g_iNumTotal++;
	printf("\n�̸�: "); scanf("%s", newData->m_szName);
	printf("\n����: "); scanf("%d", &newData->m_iKor);
	printf("\n����: "); scanf("%d", &newData->m_iEng);
	printf("\n����: "); scanf("%d", &newData->m_iMat);
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
	// �˻�
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
	printf("\n�̸��� �Է� : "); scanf("%s", szName);

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

	// ����
	printf("\n�̸�: "); scanf("%s", pFind->m_szName);
	printf("\n����: "); scanf("%d", &pFind->m_iKor);
	printf("\n����: "); scanf("%d", &pFind->m_iEng);
	printf("\n����: "); scanf("%d", &pFind->m_iMat);
	pFind->m_iTotal =
		pFind->m_iKor +
		pFind->m_iEng +
		pFind->m_iMat;
	pFind->m_fAverage = pFind->m_iTotal / 3.0f;

	// ����
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
// NULL ==> 0 ������ ������ �� =  nullptr;
void SortData(int iValue)
{
	int k;
}
// ���Ḯ��Ʈ ���� �Ŀ� ������ �����Ѵ�.
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
