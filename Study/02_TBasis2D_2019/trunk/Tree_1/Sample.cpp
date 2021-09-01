// 2진 트리와 배열의 관계를 사용하는 방법을 제시한다.
#include <iostream>
static const int g_iMaxDepth = 2;
int g_iIndex = 0;
struct TNode
{
	int iData;
	int iIndex;
	int iDepth;
	TNode* pChild[2];
};
typedef TNode* P_TNode;
TNode* g_pRootNode = 0;
TNode* g_pRootNodePtr = 0;
TNode** g_pArrayNode;
TNode** g_pArrayNodePtr;
TNode* NewNode()
{
	TNode* pNode = new TNode;
	pNode->iData = rand() % 100;
	pNode->iIndex = g_iIndex++;
	pNode->iDepth = 0;
	pNode->pChild[0] = NULL;
	pNode->pChild[1] = NULL;
	return pNode;
}
// PreOrder 탐색으로 생성한다.
TNode* AddNode(TNode* pNode)
{
	if (pNode == NULL) return NULL;
	if (pNode->iDepth >= g_iMaxDepth) return NULL;

	// 왼쪽 자식노드 인덱스 = pNode->iIndex * 2 + 1
	// 오른쪽 자식노드 인덱스 = pNode->iIndex * 2 + 2
	pNode->pChild[0] = g_pArrayNode[pNode->iIndex * 2 + 1];
	pNode->pChild[1] = g_pArrayNode[pNode->iIndex * 2 + 2];
	pNode->pChild[0]->iDepth = pNode->iDepth + 1;
	pNode->pChild[1]->iDepth = pNode->iDepth + 1;

	AddNode(pNode->pChild[0]);
	AddNode(pNode->pChild[1]);
}
TNode* AddNodePtr(TNode* pNode)
{
	if (pNode == NULL) return NULL;
	if (pNode->iDepth >= g_iMaxDepth) return NULL;

	pNode->pChild[0] = NewNode();
	pNode->pChild[1] = NewNode();
	pNode->pChild[0]->iDepth = pNode->iDepth + 1;
	pNode->pChild[1]->iDepth = pNode->iDepth + 1;

	// 왼쪽 자식노드 인덱스 = pParentNode->iIndex * 2 + 1
	// 오른쪽 자식노드 인덱스 = pParentNode->iIndex * 2 + 2
	int iIndex = pNode->iIndex * 2 + 1;
	g_pArrayNodePtr[iIndex] = pNode->pChild[0];
	g_pArrayNodePtr[iIndex + 1] = pNode->pChild[1];

	AddNodePtr(pNode->pChild[0]);
	AddNodePtr(pNode->pChild[1]);
}

TNode* PrintAddNodePtr(TNode* pNode)
{
	if (pNode == NULL) return NULL;
	std::cout << pNode->iIndex << "[" << pNode->iData << "]" << std::endl;
	PrintAddNodePtr(pNode->pChild[0]);
	PrintAddNodePtr(pNode->pChild[1]);
}
void DeleteNode(int iNumCount)
{
	for (int iNode = 0; iNode < iNumCount; iNode++)
	{
		if (g_pArrayNode[iNode] != NULL)
		{
			delete g_pArrayNode[iNode];
			g_pArrayNode[iNode] = 0;
		}
	}
}
// Post tree 탐색으로 삭제한다.
void DeleteNodePtr(TNode* pNode)
{
	if (pNode == NULL) return;
	DeleteNodePtr(pNode->pChild[0]);
	DeleteNodePtr(pNode->pChild[1]);
	delete pNode;
	pNode = 0;
}
void main()
{
	// 배열을 할당하고 2진 트리를 구축
	int iNumCount = pow(2.0f, g_iMaxDepth + 1) - 1;
	g_pArrayNode = new P_TNode[iNumCount];
	g_pArrayNodePtr = new P_TNode[iNumCount];

	for (int i = 0; i < iNumCount; i++)
	{
		g_pArrayNode[i] = NewNode();
	}
	g_pRootNode = g_pArrayNode[0];
	AddNode(g_pRootNode);

	// 부모노드 인덱스 = (Node->iIndex-1) / 2;
	// 3번 자식 노드의 부모노드를 검색
	TNode* pParent = g_pArrayNode[(3 - 1) / 2];

	PrintAddNodePtr(g_pRootNode);
	// 배열로 트리를 삭제
	DeleteNode(iNumCount);

	// 재귀호출을 통해서 트리를 구축하고 배열에 저장.
	g_iIndex = 0;
	g_pRootNodePtr = NewNode();
	g_pArrayNodePtr[0] = g_pRootNodePtr;
	AddNodePtr(g_pRootNodePtr);
	PrintAddNodePtr(g_pRootNodePtr);
	// 재귀호출로 트리를 삭제
	DeleteNodePtr(g_pRootNodePtr);

	delete[] g_pArrayNode;
	delete[] g_pArrayNodePtr;

	g_pArrayNode = 0;
	g_pArrayNodePtr = 0;
}