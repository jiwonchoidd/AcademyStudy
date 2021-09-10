// 2�� Ʈ���� �迭�� ���踦 ����ϴ� ����� �����Ѵ�.
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
// PreOrder Ž������ �����Ѵ�.
TNode* AddNode(TNode* pNode)
{
	if (pNode == NULL) return NULL;
	if (pNode->iDepth >= g_iMaxDepth) return NULL;

	// ���� �ڽĳ�� �ε��� = pNode->iIndex * 2 + 1
	// ������ �ڽĳ�� �ε��� = pNode->iIndex * 2 + 2
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

	// ���� �ڽĳ�� �ε��� = pParentNode->iIndex * 2 + 1
	// ������ �ڽĳ�� �ε��� = pParentNode->iIndex * 2 + 2
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
// Post tree Ž������ �����Ѵ�.
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
	// �迭�� �Ҵ��ϰ� 2�� Ʈ���� ����
	int iNumCount = pow(2.0f, g_iMaxDepth + 1) - 1;
	g_pArrayNode = new P_TNode[iNumCount];
	g_pArrayNodePtr = new P_TNode[iNumCount];

	for (int i = 0; i < iNumCount; i++)
	{
		g_pArrayNode[i] = NewNode();
	}
	g_pRootNode = g_pArrayNode[0];
	AddNode(g_pRootNode);

	// �θ��� �ε��� = (Node->iIndex-1) / 2;
	// 3�� �ڽ� ����� �θ��带 �˻�
	TNode* pParent = g_pArrayNode[(3 - 1) / 2];

	PrintAddNodePtr(g_pRootNode);
	// �迭�� Ʈ���� ����
	DeleteNode(iNumCount);

	// ���ȣ���� ���ؼ� Ʈ���� �����ϰ� �迭�� ����.
	g_iIndex = 0;
	g_pRootNodePtr = NewNode();
	g_pArrayNodePtr[0] = g_pRootNodePtr;
	AddNodePtr(g_pRootNodePtr);
	PrintAddNodePtr(g_pRootNodePtr);
	// ���ȣ��� Ʈ���� ����
	DeleteNodePtr(g_pRootNodePtr);

	delete[] g_pArrayNode;
	delete[] g_pArrayNodePtr;

	g_pArrayNode = 0;
	g_pArrayNodePtr = 0;
}