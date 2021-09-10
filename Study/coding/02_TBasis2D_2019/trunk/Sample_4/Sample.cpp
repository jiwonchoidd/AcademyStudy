//////////////////////////////////////////////////////////////
// 헤드노드(g_pNodeHead)부터 실제 데이터 저장하는 이중연결리스트(Doubly Linked List)
// 주의 : visual studio 2015이전에는 _CRT_SECURE_NO_WARNINGS 경고메세지
// 주)TNode*  g_pNodeHead 노드부터 실제 데이터가 저장된다.
// #define BACKWARD_LINK  후위연결리스트 
// #undef BACKKWARD_LINK  전위연결리스트
//////////////////////////////////////////////////////////////
#include "Sample.h"
KGCA	SUBJECT   kor;	SUBJECT   eng;	SUBJECT   mat;  END

bool CreateData()
{
	FILE* fp = fopen("data.txt", "w");
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
	if (g_pNodeHead == NULL)
	{
		g_pNodeHead = pStudent;
	}
	else
	{
		g_pNodeTail->pNext = pStudent;
		pStudent->pPrev = g_pNodeTail;
	}
	g_pNodeTail = pStudent;
}
void FrontAddLink(TNode* pStudent)
{
	if (g_pNodeHead == NULL)
	{
		g_pNodeTail = pStudent;
	}
	pStudent->pNext = g_pNodeHead;
	if (g_pNodeHead != NULL)
	{
		g_pNodeHead->pPrev = pStudent;
	}
	g_pNodeHead = pStudent;
}
bool DataRead()
{
	FILE* fp = fopen("data.txt", "r");
	if (fp == NULL) return false;
	int iCount = 0;
	fscanf(fp, "%d\n", &iCount);
	for (int iCnt = 0; iCnt < iCount; iCnt++)
	{
		char strBuffer[256];// = { 0, };
		memset(strBuffer, 0, sizeof(char) * 256);

		TNode* pStudent = (TNode*)malloc(sizeof(TNode));
		memset(pStudent, 0, sizeof(TNode));
		// 메모리로 부터 입출력
		fgets(strBuffer, 256, fp);
		sscanf(strBuffer, "%s%d%d%d",
			pStudent->m_szName,
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
void DeleteAllNode()
{
	if (g_pNodeHead == NULL) return;
	TNode* pNode = g_pNodeHead;
	while (pNode->pNext)
	{
		TNode* pDelNode = pNode->pNext;
		pNode->pNext = pDelNode->pNext;
		pDelNode->pNext->pPrev = pNode;
		free(pDelNode);
	}
	free(g_pNodeHead);
	g_pNodeTail = g_pNodeHead = 0;
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
//#undef  BACKWARD_LINK  // 반전
#ifdef BACKWARD_LINK
	for (TNode* pNode = g_pNodeHead;
		pNode != NULL;
		pNode = pNode->pNext)
#else
	for (TNode* pNode = g_pNodeTail;
		pNode != NULL;
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
	printf("====   성적 관리 프로그램      ====\n");
	printf("===================================\n");
	printf("이름\t국어\t영어\t수학\t총점\t평균\n");
}
void  Initialize()
{
	srand(time(NULL));
}
void main()
{
	Initialize();

	while (1)
	{
		int iSelect = 0;
		printf("\n->Select=>0:Create,1:Load,2:Print,3:Find,Quit:999  : ");
		scanf("%d", &iSelect);
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
			printf("\n 검색하려는 이름을 입력하시오: ");
			scanf("%s", strFind);
			TNode* pNode = FindLink(strFind);
			PrintNode(pNode);
		}break;
		}

		if (!Execute()) return;
	}

	DeleteAllNode();
}