#define SIMPLE
#ifdef SIMPLE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
struct TStudent
{
	char szName[20];
	int  iAge;
	int  iKor;
	int  iEng;
	int  iMat;
	int  iTotal;
	float fAverage;
	TStudent* pNext;
	TStudent* pPrev;
};
TStudent* g_pHeadNode = NULL;
TStudent* g_pTailNode = NULL;
TStudent* g_pFindNode = NULL;

TStudent* CreateNode()
{
	TStudent* pNode =
		(TStudent*)malloc(sizeof(TStudent));
	strcpy_s(pNode->szName, "kgca");
	pNode->iAge = rand() % 100;
	pNode->iKor = rand() % 100;
	pNode->iEng = rand() % 100;
	pNode->iMat = rand() % 100;
	pNode->pNext = 0;
	return pNode;
}
// 전위 연결리스트(헤더노드에 앞에 추가한다.)
// 후위 연결리스트(꼬리 앞에 추가한다.)
void AddNode(TStudent* pAddNode)
{
	// pPreNode -> pAddNode -> g_pTailNode
	TStudent* pPreNode = g_pTailNode->pPrev;
	pPreNode->pNext = pAddNode;
	pAddNode->pNext = g_pTailNode;
	// pPreNode <- pAddNode <- g_pTailNode
	pAddNode->pPrev = pPreNode;
	g_pTailNode->pPrev = pAddNode;
}
//g_pFindNode->pNext ---> 추가
bool InsertNode(TStudent* pAddNode)
{
	if (g_pFindNode == nullptr)
	{
		return false;
	}

	// g_pFindNode -> pAddNode -> NextNode
	TStudent* pNextNode = g_pFindNode->pNext;
	g_pFindNode->pNext = pAddNode;
	pAddNode->pNext = pNextNode;
	// g_pFindNode <- pAddNode <- NextNode
	pNextNode->pPrev = pAddNode;
	pAddNode->pPrev = g_pFindNode;
	return true;
}
void DeleteNode(TStudent* pCurrentNode)
{
	// pPreNode -> pCurrentNode ->  pPostNode
	// pPreNode <- pCurrentNode <-  pPostNode
	TStudent* pPreNode = pCurrentNode->pPrev;
	TStudent* pPostNode = pCurrentNode->pNext;
	// pPreNode ->  pPostNode
	// pPreNode <-  pPostNode
	pPreNode->pNext = pPostNode;
	pPostNode->pPrev = pPreNode;
	// 검색 B -> 삭제
	free(pCurrentNode);
}
// 초기화 작업
void Init()
{
	g_pHeadNode = (TStudent*)malloc(sizeof(TStudent));
	g_pTailNode = (TStudent*)malloc(sizeof(TStudent));
	// 단일 연결리스트
	g_pHeadNode->pNext = g_pTailNode;
	g_pHeadNode->pPrev = nullptr;
	// 이중 연결리스트
	g_pTailNode->pNext = nullptr;
	g_pTailNode->pPrev = g_pHeadNode;
}
void Work()
{
	while (1)
	{
		int iSelect = 0;
		printf("0=생성 1=출력 2=나이검색 3=삭제 4=추가 9:종료");
		scanf_s("%d", &iSelect);
		if (iSelect == 9)
		{
			break;
		}
		switch (iSelect)
		{
		case 0:
		{
			for (int i = 0; i < 10; i++)
			{
				TStudent* pNode = CreateNode();
				AddNode(pNode);
			}
		}break;
		case 1:
		{
			for (TStudent* pNode = g_pHeadNode->pNext;
				pNode != g_pTailNode;
				pNode = pNode->pNext)
			{
				printf("%s %d %d %d %d\n",
					pNode->szName,
					pNode->iAge,
					pNode->iKor,
					pNode->iEng,
					pNode->iMat);
			}
		}break;
		case 2:
		{
			printf("검색할 나이를 입력하시오: ");
			int iAge;
			scanf_s("%d", &iAge);
			for (TStudent* pNode = g_pHeadNode;
				pNode != g_pTailNode;
				pNode = pNode->pNext)
			{
				if (pNode->iAge == iAge)
				{
					g_pFindNode = pNode;
					break;
				}
			}
			if (g_pFindNode != nullptr)
			{
				printf("검색된 정보 -> %s %d %d %d %d\n",
					g_pFindNode->szName,
					g_pFindNode->iAge,
					g_pFindNode->iKor,
					g_pFindNode->iEng,
					g_pFindNode->iMat);
			}
		}break;
		case 3:
		{
			DeleteNode(g_pFindNode);
		}break;
		case 4:
		{
			TStudent* pNode = CreateNode();
			bool isAdd = InsertNode(pNode);
			if (isAdd == false)
			{
				printf("%s\n", "장소를 선택하시오!");
			}
			else
			{
				printf("%s\n", "추가 되었습니다.1");
			}
		}break;
		}
	}
}
// 마무리 작업
void Delete()
{
	TStudent* pCurrentNode = g_pHeadNode->pNext;
	while (pCurrentNode != g_pTailNode)
	{
		TStudent* pDeleteNode = pCurrentNode;
		pCurrentNode = pDeleteNode->pNext;
		free(pDeleteNode);
	}

	free(g_pHeadNode);
	free(g_pTailNode);
}
void main()
{
	Init();
	Work();
	Delete();
}
#else
#include "TManager.h"
void Initialize()
{
	SAFE_NEW_CLEAR(g_pHead, TStudent);
	SAFE_NEW_CLEAR(g_pTail, TStudent);
	g_pHead->m_pNext = g_pTail;
	g_pTail->m_pPrev = g_pHead;

	SortFunction = Ascending;
}
void main()
{
	Initialize();

	FileLoad();

	int iSelect = 0;
	while (1)
	{
		MenuPrint();
		scanf("%d", &iSelect);
		if (iSelect == EXIT) break;
		system("cls");
		// 작업리스트
		switch (iSelect)
		{
		case SCREEN_PRINT:
		{
			if (ScreenPrint() > 0)
				printf("\n정상적으로 출력.");
			else
				printf("\n데이터가 존재하지 않습니다.");
		}break;
		case NEW_DATA:
		{
			NewData();
		}break;

		case FILE_SAVE:
		{
			if (FileSave() > 0)
				printf("\n정상적으로 저장.");
			else
				printf("\n문제가 있어 저장 하지 못했습니다.");
		}break;
		case FILE_LOAD:
		{
			if (FileLoad() > 0)
				printf("\n정상적으로 로드됨.");
			else
				printf("\n로드 실폐1.");
		}break;
		case DATA_UPDATE:
		{
			while (UpdateData() < 0)
			{
				printf("검색 실폐!");
			}
		}break;
		case DEL_DATA:
		{
			DelData();
		}break;
		case SORT_DATA:
		{
			//SortFunction = Ascending;
			//Ascending
			SortData(nullptr);
		}break;
		case CREATE_SAMPLE_DATA:
		{
			CreateSampleData();
		}break;
		}
	}

	AllDelLink();

	SAFE_DEL(g_pHead);
	SAFE_DEL(g_pTail);
	printf("\n아무키나 누르시면 종료됩니다.");
	_getch();
}

#endif