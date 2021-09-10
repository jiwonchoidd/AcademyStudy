//////////////////////////////////////////////////////////////
// �����(g_pNodeHead) Next���� ���� ������ �����ϴ� ���Ḯ��Ʈ
// ������ ����带 ������ �ΰ� ó���Ѵ�.
// ��)g_pNodeTail�� ����Ͽ� g_pNodeHead���� ������ �����Ͱ� ����ȴ�.
//////////////////////////////////////////////////////////////
#include "Sample.h"
bool InsertLink(TNode* pFindNode, char* pName, int m_iAge, char* pAddr, int* pData)
{
	if (pFindNode == NULL) return false;
	TNode* pNewData = new TNode;
	strcpy(pNewData->m_szName, pName);
	pNewData->m_iAge = m_iAge;
	strcpy(pNewData->m_strAddress, pAddr);
	pNewData->m_iKor = pData[0];
	pNewData->m_iEng = pData[1];
	pNewData->m_iMat = pData[2];
	pNewData->pNext = 0;
	// �߰� ����
	TNode* pTempNode = pFindNode->pNext;
	pFindNode->pNext = pNewData;
	pNewData->pNext = pTempNode;
	return true;
};
TNode* UpdateData(TNode* pFindNode)
{
	if (g_pFindNode == NULL)  return NULL;
	return pFindNode;
}
// �˻����� ��ȯ��尡 ���� ���� ����̹Ƿ� 
TNode* DelLink(TNode* pPrevNode)
{
	TNode* pDelNode = pPrevNode->pNext;
	TNode* pNextNode = pDelNode->pNext;
	delete pDelNode;
	g_iNumCounter--;

	pPrevNode->pNext = pNextNode;
	if (g_pNodeHead->pNext == g_pNodeTail)
	{
		g_pNodeTail = g_pNodeHead;
	}
	return pNextNode;
}
void AllDeleteLink()
{	
	while(g_pNodeHead->pNext != NULL)
	{
		DelLink(g_pNodeHead);
	}
	g_pNodeTail = g_pNodeHead;
}
void  LoadLink(char* pLoadFileName)
{
	FILE* fp_dest = fopen(pLoadFileName, "r");
	if (fp_dest != NULL)
	{
		int iCounter;
		fscanf(fp_dest, "%d", &iCounter);
		for (int i = 0; i < iCounter; i++)
		{
			TNode   nodeRead;
			nodeRead.pNext = 0;
			fscanf(fp_dest, "%s %d %d %d %d %s",
				nodeRead.m_szName,
				&nodeRead.m_iAge,
				&nodeRead.m_iKor,
				&nodeRead.m_iEng,
				&nodeRead.m_iMat,
				nodeRead.m_strAddress);

			int data[3];
			data[0] = nodeRead.m_iKor;
			data[1] = nodeRead.m_iEng;
			data[2] = nodeRead.m_iMat;

			NewLink(nodeRead.m_szName,
				nodeRead.m_iAge,
				nodeRead.m_strAddress,
				data);
		}
	}
	fclose(fp_dest);
}
void  NewLink(char* pName, int m_iAge, char* pAddr, int* pData)
{
	TNode* pNewData = new TNode;
	strcpy(pNewData->m_szName, pName);
	pNewData->m_iAge = m_iAge;
	strcpy(pNewData->m_strAddress, pAddr);
	pNewData->m_iKor = pData[0];
	pNewData->m_iEng = pData[1];
	pNewData->m_iMat = pData[2];
	pNewData->pNext = 0;
	g_pNodeTail->pNext = pNewData;
	g_pNodeTail = pNewData;
	g_iNumCounter++;
}
void SaveData(char* pSaveFileName)
{
	FILE* fp_dest = fopen(pSaveFileName, "w");
	TNode*   nodeWrite = 0;
	if (g_iNumCounter <= 0)
	{
		RandData();
	}
	fprintf(fp_dest, "%d", g_iNumCounter);

	for (nodeWrite = g_pNodeHead->pNext;
		nodeWrite != NULL;
		nodeWrite = nodeWrite->pNext)
	{
		fprintf(fp_dest, "\n%s %d %d %d %d %s ", nodeWrite->m_szName,
			nodeWrite->m_iAge,
			nodeWrite->m_iKor, nodeWrite->m_iEng, nodeWrite->m_iMat,
			nodeWrite->m_strAddress);
	}
	fclose(fp_dest);
}
void PrintAllData()
{
	TNode*   nodeWrite = g_pNodeHead->pNext;
	if (nodeWrite == NULL)
	{
		printf("\n ������ ����!!! ");
		return;
	}
	while (nodeWrite != NULL)
	{
		nodeWrite->m_iTotal = nodeWrite->m_iKor +
			nodeWrite->m_iEng +
			nodeWrite->m_iMat;
		nodeWrite->m_fAverage = nodeWrite->m_iTotal / 3.0f;
		printf("\n%s %d %d %d %d %d %10.4f %s", nodeWrite->m_szName,
			nodeWrite->m_iAge,
			nodeWrite->m_iKor,
			nodeWrite->m_iEng,
			nodeWrite->m_iMat,
			nodeWrite->m_iTotal,
			nodeWrite->m_fAverage,
			nodeWrite->m_strAddress);

		nodeWrite = nodeWrite->pNext;
	}	
}
void PrintData(TNode* pFindNode)
{
	if (pFindNode == NULL) return;
	pFindNode->m_iTotal = pFindNode->m_iKor +
		pFindNode->m_iEng +
		pFindNode->m_iMat;
	pFindNode->m_fAverage = pFindNode->m_iTotal / 3.0f;
	printf("\n%s %d %d %d %d %d %10.4f %s", pFindNode->m_szName,
		pFindNode->m_iAge,
		pFindNode->m_iKor,
		pFindNode->m_iEng,
		pFindNode->m_iMat,
		pFindNode->m_iTotal,
		pFindNode->m_fAverage,
		pFindNode->m_strAddress);
}
// �˻������ �ٷ� �� ��� ��ȯ
TNode* FindName(char* pFindName)
{
	TNode*   nodeWrite = 0;
	TNode*   nodePrev = 0;
	for (nodeWrite = g_pNodeHead;
		nodeWrite != NULL;
		nodeWrite = nodeWrite->pNext)
	{
		// a == b -> ��ȯ���� 0
		// a > b -> ��ȯ���� 1
		// a < b -> ��ȯ���� -1
		// ���ڴ����� �ƽ�Ű�ڵ尪 �� ��� ��ȯ
		if (nodeWrite->pNext != NULL && !strcmp(nodeWrite->pNext->m_szName, pFindName))
		{
			return nodeWrite;
		}
	}
	return NULL;
}
void RandData()
{
	srand(time(NULL));
	for (int iStd = 0; iStd < 5; iStd++)
	{
		int subject[3];
		for (int i = 0; i < 3; i++)
		{
			subject[i] = rand() % 101;
		}
		int iNameA = 65 + rand() % 26; // �빮��:A(65), �ҹ���a(97):
		int iNameB = 65 + rand() % 26;
		int iNameC = 65 + rand() % 26;
		char name[4] = { 0 };
		name[0] = iNameA;
		name[1] = iNameB;
		name[2] = iNameC;
		NewLink(name, rand() % 100, "none", subject);
	}
}
bool  Initialize()
{
	g_pNodeHead = new TNode;
	memset(g_pNodeHead, 0, sizeof(TNode));
	g_pNodeTail = g_pNodeHead;
	srand(time(NULL));
	if (g_pNodeHead != NULL) return true;
	return false;
}
void  Clear()
{
	delete g_pNodeHead;
	g_pNodeHead = 0;
}
void MenuClear()
{
	system("cls");
	printf("\n=========================================");
	printf("\n============ �����������α׷�============");
	printf("\n=========================================");
}
int main(int argc, char* argv[])
{
	//if (argc < 2)
	//{
	//	printf("����� [��������] [������ϸ�]  �����Ͻÿ�.!");
	//	return 1;
	//}
	if (!Initialize())
	{
		printf("�ʱ�ȭ ����.!");
		return 1;
	}
	bool bRunning = true;
	while (bRunning)
	{
		int iWork = 0;
		printf("\n����(0),���(1),����(2),�߰�(3),�ε�(4),����(5),�˻�(6),����(7),����(99) :");
		scanf("%d", &iWork);
		switch (iWork)
		{
			//�ʱⵥ���� ����
		case 0:
		{
			MenuClear();
			RandData();
		}break;
		//ȭ�����
		case 1:
		{
			MenuClear();
			PrintAllData();
		}break;
		// ���� ����
		case 2:
		{
			MenuClear();
			SaveData("save.txt");
		}break;
		// �ű� ������ �߰�
		case 3:
		{
			MenuClear();
			int subject[3] = { 0 };
			subject[0] = rand() % 100;
			subject[1] = rand() % 100;
			subject[2] = rand() % 100;
			if (g_pFindNode != NULL)
			{
				InsertLink(g_pFindNode->pNext, "new", rand() % 100, "none", subject);
			}
			else
			{
				NewLink("new", rand() % 100, "none", subject);
			}
		}break;
		//���� �ε�
		case 4:
		{
			MenuClear();
			LoadLink("save.txt");
		}break;
		//���õ� ��� ���� //���Ḯ��Ʈ ����
		case 5:
		{
			MenuClear();
			DelLink(g_pFindNode);
			//AllDeleteLink();
		}break;
		case 6:
		{
			MenuClear();
			char strFileName[20] = { 0 };
			printf("\n �̸��� �Է��Ͻÿ� : ");
			scanf("%s", strFileName);
			g_pFindNode = FindName(strFileName);
			if (g_pFindNode != NULL)
			{
				PrintData(g_pFindNode->pNext);
			}
			else
			{
				printf("�˻�����!");
			}
		}break;
		case 7:
		{
			if (g_pFindNode == NULL)
			{
				char strFileName[20] = { 0 };
				printf("\n �̸��� �Է��Ͻÿ� : ");
				scanf("%s", strFileName);
				g_pFindNode = FindName(strFileName);
			}
			MenuClear();
			UpdateData(g_pFindNode->pNext);
		}break;
		case 99:
		{
			printf("10�а� �޽�.!");
			for (int i = 0; i < 1000000000; i++);
			bRunning = false;
			break;
		}break;
		default:
		{
			printf("���������� �Է��ϼ���.!");
		}
		}
	}
	AllDeleteLink();
	
	Clear();
	return 1;
}