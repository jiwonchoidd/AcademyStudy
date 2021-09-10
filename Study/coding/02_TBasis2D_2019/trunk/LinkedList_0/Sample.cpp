//////////////////////////////////////////////////////////////
// 헤드노드(g_pNodeHead) Next부터 실제 데이터 저장하는 연결리스트
// 사전에 헤드노드를 생성해 두고 처리한다.
// 주)g_pNodeTail을 사용하여 g_pNodeHead부터 후위로 데이터가 저장된다.
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
	// 중간 삽입
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
// 검색에서 반환노드가 삭제 이전 노드이므로 
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
		printf("\n 데이터 없음!!! ");
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
// 검색노드의 바로 앞 노드 반환
TNode* FindName(char* pFindName)
{
	TNode*   nodeWrite = 0;
	TNode*   nodePrev = 0;
	for (nodeWrite = g_pNodeHead;
		nodeWrite != NULL;
		nodeWrite = nodeWrite->pNext)
	{
		// a == b -> 반환값은 0
		// a > b -> 반환값은 1
		// a < b -> 반환값은 -1
		// 문자단위로 아스키코드값 비교 결과 반환
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
	printf("\n============ 성적관리프로그램============");
	printf("\n=========================================");
}
int main(int argc, char* argv[])
{
	//if (argc < 2)
	//{
	//	printf("사용방법 [실행파일] [대상파일명]  엔터하시오.!");
	//	return 1;
	//}
	if (!Initialize())
	{
		printf("초기화 오류.!");
		return 1;
	}
	bool bRunning = true;
	while (bRunning)
	{
		int iWork = 0;
		printf("\n생성(0),출력(1),저장(2),추가(3),로드(4),삭제(5),검색(6),수정(7),종료(99) :");
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
			PrintAllData();
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
		//파일 로드
		case 4:
		{
			MenuClear();
			LoadLink("save.txt");
		}break;
		//선택된 노드 삭제 //연결리스트 삭제
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
			printf("\n 이름을 입력하시오 : ");
			scanf("%s", strFileName);
			g_pFindNode = FindName(strFileName);
			if (g_pFindNode != NULL)
			{
				PrintData(g_pFindNode->pNext);
			}
			else
			{
				printf("검색오류!");
			}
		}break;
		case 7:
		{
			if (g_pFindNode == NULL)
			{
				char strFileName[20] = { 0 };
				printf("\n 이름을 입력하시오 : ");
				scanf("%s", strFileName);
				g_pFindNode = FindName(strFileName);
			}
			MenuClear();
			UpdateData(g_pFindNode->pNext);
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