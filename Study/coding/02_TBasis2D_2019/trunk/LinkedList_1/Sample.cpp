//////////////////////////////////////////////////////////////
// 헤드노드(g_pNodeHead) Next부터 실제 데이터 저장하는 연결리스트
// 사전에 헤드노드를 생성해 두고 처리한다.
// 주)신규 노드는 전위 연결리스트로 g_pNodeHead->pNext에 연결된다.
//////////////////////////////////////////////////////////////
#include "Sample.h"
// pDelNode를 삭제한다.
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
		printf("\n 데이터 없음!!! ");
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
		int iNameA = 65 + rand() % 26; // 대문자:A(65), 소문자a(97):
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
	printf("\n============ 성적관리프로그램============");
	printf("\n=========================================");

}
int main(int argc, char* argv[])
{
	Initialize();

	bool bRunning = true;
	while (bRunning)
	{
		int iWork = 0;
		printf("\n생성(0),출력(1),저장(2),추가(3),로드(4),삭제(5),종료(99) :");
		scanf("%d", &iWork);
		switch (iWork)
		{
			//초기데이터 생성
		case 0: 
		{
			MenuClear();
			RandData();
		}break;
			//화면출력
		case 1: 
		{
			MenuClear();
			PrintData();
		}break;
			// 파일 저장
		case 2: 
		{
			MenuClear();
			SaveData("save.txt");
		}break;
			// 신규 데이터 추가
		case 3:
		{
			MenuClear();
			int subject[3] = { 0 };
			subject[0] = 65;	subject[1] = 65;	subject[2] = 65;
			NewLink("new", rand() % 100, "none", subject);
		}break;
		//파일 로드
		case 4:
		{
			MenuClear();
			LoadLink("save.txt");
		}break;
			//연결리스트 삭제
		case 5:
		{
			MenuClear();
			AllDeleteLink();
		}break;
		case 99:
		{
			printf("10분간 휴식.!");
			for (int i = 0; i < 1000000000; i++);
			bRunning = false;
			break;
		}break;
		default:
		{
			printf("정상적으로 입력하세여.!");
		}		
		}
	}

	AllDeleteLink();	
	Clear();
	return 1;
}