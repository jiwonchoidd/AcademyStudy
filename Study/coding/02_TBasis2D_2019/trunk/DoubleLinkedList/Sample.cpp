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
// ���� ���Ḯ��Ʈ(�����忡 �տ� �߰��Ѵ�.)
// ���� ���Ḯ��Ʈ(���� �տ� �߰��Ѵ�.)
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
//g_pFindNode->pNext ---> �߰�
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
	// �˻� B -> ����
	free(pCurrentNode);
}
// �ʱ�ȭ �۾�
void Init()
{
	g_pHeadNode = (TStudent*)malloc(sizeof(TStudent));
	g_pTailNode = (TStudent*)malloc(sizeof(TStudent));
	// ���� ���Ḯ��Ʈ
	g_pHeadNode->pNext = g_pTailNode;
	g_pHeadNode->pPrev = nullptr;
	// ���� ���Ḯ��Ʈ
	g_pTailNode->pNext = nullptr;
	g_pTailNode->pPrev = g_pHeadNode;
}
void Work()
{
	while (1)
	{
		int iSelect = 0;
		printf("0=���� 1=��� 2=���̰˻� 3=���� 4=�߰� 9:����");
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
			printf("�˻��� ���̸� �Է��Ͻÿ�: ");
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
				printf("�˻��� ���� -> %s %d %d %d %d\n",
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
				printf("%s\n", "��Ҹ� �����Ͻÿ�!");
			}
			else
			{
				printf("%s\n", "�߰� �Ǿ����ϴ�.1");
			}
		}break;
		}
	}
}
// ������ �۾�
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
		// �۾�����Ʈ
		switch (iSelect)
		{
		case SCREEN_PRINT:
		{
			if (ScreenPrint() > 0)
				printf("\n���������� ���.");
			else
				printf("\n�����Ͱ� �������� �ʽ��ϴ�.");
		}break;
		case NEW_DATA:
		{
			NewData();
		}break;

		case FILE_SAVE:
		{
			if (FileSave() > 0)
				printf("\n���������� ����.");
			else
				printf("\n������ �־� ���� ���� ���߽��ϴ�.");
		}break;
		case FILE_LOAD:
		{
			if (FileLoad() > 0)
				printf("\n���������� �ε��.");
			else
				printf("\n�ε� ����1.");
		}break;
		case DATA_UPDATE:
		{
			while (UpdateData() < 0)
			{
				printf("�˻� ����!");
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
	printf("\n�ƹ�Ű�� �����ø� ����˴ϴ�.");
	_getch();
}

#endif