//////////////////////////////////////////////////////////////
// �����(g_pNodeHead) Next���� ���� ������ �����ϴ� ���Ḯ��Ʈ
// ������ ����带 ������ �ΰ� ó���Ѵ�.
// ��)�ű� ���� ���� ���Ḯ��Ʈ�� g_pNodeHead->pNext�� ����ȴ�.
//////////////////////////////////////////////////////////////
#include "Sample.h"
// pDelNode�� �����Ѵ�.
TNode* DelLink(TNode* pDelNode)
{
	TNode* pNextNode = pDelNode->pNext;
	delete pDelNode;
	g_pNodeHead->pNext = pNextNode;
	return pNextNode;
}
void AllDeleteLink()
{
	while (g_pNodeHead->pNext != NULL)
	{
		DelLink(g_pNodeHead->pNext);
	}
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

	TNode* pNext = g_pNodeHead->pNext;
	g_pNodeHead->pNext = pNewData;
	pNewData->pNext = pNext;

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
void PrintData()
{
	TNode*   nodeWrite = g_pNodeHead->pNext;
	if (nodeWrite == NULL)
	{
		printf("\n ������ ����!!! ");
		return;
	}
	while (nodeWrite!=NULL)
	{
		printf("\n%s %d %s", nodeWrite->m_szName,
			nodeWrite->m_iAge,
			nodeWrite->m_strAddress);
		nodeWrite = nodeWrite->pNext;
	}
	/*for ( nodeWrite = g_pNodeHead->pNext;
		  nodeWrite != NULL;
		  nodeWrite = nodeWrite->pNext)
	{
		printf("\n%s %d %s", nodeWrite->m_szName,
			nodeWrite->m_iAge,
			nodeWrite->m_strAddress);
	}*/
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
void  Initialize()
{
	g_pNodeHead = new TNode;
	memset(g_pNodeHead, 0, sizeof(TNode));
	srand(time(NULL));
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
	Initialize();

	bool bRunning = true;
	while (bRunning)
	{
		int iWork = 0;
		printf("\n����(0),���(1),����(2),�߰�(3),�ε�(4),����(5),����(99) :");
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
			PrintData();
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
			subject[0] = 65;	subject[1] = 65;	subject[2] = 65;
			NewLink("new", rand() % 100, "none", subject);
		}break;
		//���� �ε�
		case 4:
		{
			MenuClear();
			LoadLink("save.txt");
		}break;
			//���Ḯ��Ʈ ����
		case 5:
		{
			MenuClear();
			AllDeleteLink();
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