//////////////////////////////////////////////////////////////
// �����(g_pNodeHead)���� ���� ������ �����ϴ� ���߿��Ḯ��Ʈ(Doubly Linked List)
// ���� : visual studio 2017���� _CRT_SECURE_NO_WARNINGS  ���� �޼���
// ��)TNode*  g_pNodeHead �� TNode*  g_pNodeTail �� ������ ���� Ȱ���Ѵ�.
//////////////////////////////////////////////////////////////
#include "Sample.h"
KGCA	SUBJECT   kor;	SUBJECT   eng;	SUBJECT   mat;  END

bool CreateData()
{
	FILE* fp = 0;
	fopen_s( &fp, "data.txt", "w");
	if (fp == NULL) return false;

	srand((unsigned int)time(NULL));
	int iCount = 10;
	fprintf(fp, "%d\n", iCount);
	for (int iCnt = 0; iCnt < iCount; iCnt++)
	{
		int iNameA = 65 + rand() % 26;
		int iNameB = 65 + rand() % 26;
		int iNameC = 97 + rand() % 26;
		fprintf(fp, "%c%c%c %d %d %d\n",
			iNameA, iNameB, iNameC,
			rand() % 101,
			rand() % 101,
			rand() % 101);
	}
	fclose(fp);
	return true;
}
void BackAddLink(TNode* pStudent)
{
	// ������ �������� ����ȴ�. ��, g_pNodeTail �տ� ����ȴ�.
	if (g_pNodeHead->pNext == g_pNodeTail)
	{
		// ��尡 ���� ���
		g_pNodeCurrent = g_pNodeHead;
	}
	else
	{
		g_pNodeCurrent = g_pNodeTail->pPrev;
	}

	g_pNodeCurrent->pNext = pStudent;
	pStudent->pPrev = g_pNodeCurrent;

	pStudent->pNext = g_pNodeTail;
	g_pNodeTail->pPrev = pStudent;
}
void FrontAddLink(TNode* pStudent)
{
	// ������ ������ ����ȴ�.��, g_pNodeHead �ڿ� ����ȴ�.
	if (g_pNodeHead->pNext == g_pNodeTail)
	{
		// ��尡 ���� ���
		g_pNodeCurrent = g_pNodeTail;
	}
	else
	{
		g_pNodeCurrent = g_pNodeHead->pNext;
	}

	g_pNodeHead->pNext = pStudent;
	pStudent->pPrev = g_pNodeHead;

	pStudent->pNext = g_pNodeCurrent;
	g_pNodeCurrent->pPrev = pStudent;	
}
bool DataRead()
{
	FILE* fp = 0;
	fopen_s(&fp, "data.txt", "r");
	if (fp == NULL) return false;
	int iCount = 0;
	fscanf_s(fp, "%d\n", &iCount);
	for (int iCnt = 0; iCnt < iCount; iCnt++)
	{
		char strBuffer[256];// = { 0, };
		memset(strBuffer, 0, sizeof(char) * 256);

		TNode* pStudent = (TNode*)malloc(sizeof(TNode));
		memset(pStudent, 0, sizeof(TNode));
		// �޸𸮷� ���� �����
		fgets(strBuffer, 256, fp);
		sscanf_s(strBuffer, "%s %d %d %d",
			pStudent->m_szName,
			_countof(pStudent->m_szName),
			&pStudent->m_iKor,
			&pStudent->m_iEng,
			&pStudent->m_iMat);
#ifdef BACKWARD_LINK
		BackAddLink(pStudent);
#else
		FrontAddLink(pStudent);
#endif		
	}
	fclose(fp);
	return true;
}
TNode* RandomNode()
{
	TNode* pStudent = (TNode*)malloc(sizeof(TNode));
	memset(pStudent, 0, sizeof(TNode));	
	strcpy_s(pStudent->m_szName, "new");
	pStudent->m_iKor = rand() % 100;
	pStudent->m_iEng = rand() % 100;
	pStudent->m_iMat = rand() % 100;
	return pStudent;
}
void DeleteAllNode()
{
	TNode* pNode = g_pNodeHead->pNext;
	while (pNode != g_pNodeTail)
	{
		TNode* pDelNode = pNode;
		pNode = pNode->pNext;
		free(pDelNode);
	}
}
void PrintNode(TNode* pNode)
{
	int m_iTotal = pNode->m_iKor +
		pNode->m_iEng +
		pNode->m_iMat;
	float m_fAverage = m_iTotal / 3.0f;
	printf("%s\t%d\t%d\t%d\t%d\t%f\n", pNode->m_szName,
		pNode->m_iKor,
		pNode->m_iEng,
		pNode->m_iMat,
		m_iTotal,
		m_fAverage);
}
bool PrintData()
{
	//#undef  BACKWARD_LINK  // ����
#ifdef BACKWARD_LINK
	for (TNode* pNode = g_pNodeHead->pNext;
		pNode != g_pNodeTail;
		pNode = pNode->pNext)
#else
	for (TNode* pNode = g_pNodeTail->pPrev;
		pNode != g_pNodeHead;
		pNode = pNode->pPrev)
#endif
	{
		PrintNode(pNode);
	}
	return true;
}
TNode* FindLink(char* pName)
{
	for (TNode* pNode = g_pNodeHead;
		pNode != NULL;
		pNode = pNode->pNext)
	{
		if (!strcmp(pNode->m_szName, pName))
		{
			return pNode;
		}
	}
}
void Clear()
{
	system("cls");
	printf("===================================\n");
	printf("====   ���� ���� ���α׷�      ====\n");
	printf("===================================\n");
	printf("�̸�\t����\t����\t����\t����\t���\n");
}
void  Initialize()
{
	// �Ӹ��� ������ ���߿��Ḯ��Ʈ�� �����Ѵ�.
	g_pNodeHead = (TNode*)malloc(sizeof(TNode));
	g_pNodeTail = (TNode*)malloc(sizeof(TNode));
	memset(g_pNodeHead, 0, sizeof(TNode));
	memset(g_pNodeTail, 0, sizeof(TNode));
	g_pNodeHead->pNext = g_pNodeTail;
	g_pNodeTail->pPrev = g_pNodeHead;

	srand(time(NULL));
}
void main()
{
	Initialize();

	while (1)
	{
		int iSelect = 0;
		printf("\n->Select=>Create(0),Load(1),Print(2),Find(3),F_Insert(4),B_Insert(5)Quit:999  : ");
		scanf_s("%d", &iSelect);
		if (iSelect == 999) break;
		Clear();
		switch (iSelect)
		{
		case DATA_CREATE:
		{
			DeleteAllNode();
			PROCESS = CreateData;
		}break;
		case DATA_LOAD:
		{
			DeleteAllNode();
			PROCESS = DataRead;
		}break;
		case DATA_PRINT:
		{
			PROCESS = PrintData;
		}break;

		case DATA_FIND:
		{
			PROCESS = PrintData;
			char strFind[10] = { 0, };
			printf("\n �˻��Ϸ��� �̸��� �Է��Ͻÿ�: ");
			scanf_s("%s", strFind);
			TNode* pNode = FindLink(strFind);
			PrintNode(pNode);
		}break;
		case DATA_FRONT_INSERT:
		{
			BackAddLink(RandomNode());
		}break;
		case DATA_BACK_INSERT:
		{
			FrontAddLink(RandomNode());
		}break;
		}

		if (!Execute()) return;
	}

	DeleteAllNode();

	// �Ӹ�,���� ��带 �����Ѵ�.
	free(g_pNodeHead);
	free(g_pNodeTail);
}