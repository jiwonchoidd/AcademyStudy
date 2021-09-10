#include <stdio.h>
typedef struct Node
{
	int iDepth;
	int iOrder;
	int iData;
	struct Node*  pChildNode[2];
}TNode;

int g_iData[10] = { 5,0,4,7,8,3,9,2,6,1 };
int g_iIndex = 0;

TNode* g_pRoot = 0;
// 2ÁøÆ®¸® ±¸Ãà( ±íÀÌ 3±îÁö¸¸ )
TNode* CreateNode(TNode* pParent)
{
	if (pParent == 0) return nullptr;
	if (pParent->iDepth > 3) return nullptr;

	pParent->iOrder = g_iIndex++;
	pParent->pChildNode[0] = new TNode();
	pParent->pChildNode[1] = new TNode();

	pParent->pChildNode[0]->iDepth = pParent->iDepth + 1;
	pParent->pChildNode[1]->iDepth = pParent->iDepth + 1;

	CreateNode(pParent->pChildNode[0]);
	CreateNode(pParent->pChildNode[1]);
}
// 2Áø Å½»ö¿ë Æ®¸® ±¸Ãà
TNode* BinaryNode(TNode* pParent, int iData)
{
	if (pParent == 0) return nullptr;
	if (pParent->iData > iData)
	{
		if (pParent->pChildNode[0] == 0)
		{
			pParent->pChildNode[0] = new TNode();
			pParent->pChildNode[0]->iData = iData;
			pParent->pChildNode[0]->iDepth =
				pParent->iDepth + 1;
		}
		else
		{
			BinaryNode(pParent->pChildNode[0], iData);
		}
	}
	else
	{
		if (pParent->pChildNode[1] == 0)
		{
			pParent->pChildNode[1] = new TNode();
			pParent->pChildNode[1]->iData = iData;
			pParent->pChildNode[1]->iDepth =
				pParent->iDepth + 1;
		}
		else
		{
			BinaryNode(pParent->pChildNode[1], iData);
		}
	}
}
// 2Áø Å½»ö
TNode* BinarySearch(TNode* pParent, int iData)
{
	if (pParent == 0) return nullptr;
	if (pParent->iData == iData)
	{
		return pParent;
	}
	if (pParent->iData > iData)
	{
		BinarySearch(pParent->pChildNode[0], iData);
	}
	else
	{
		BinarySearch(pParent->pChildNode[1], iData);
	}	
}
void main()
{
	g_pRoot = new TNode(); // ¸â¹öÃÊ±âÈ­µÊ.
	//g_pRoot = new TNode; // ¸â¹öÃÊ±âÈ­ ¾ÈµÊ.
	g_pRoot->iData = g_iData[g_iIndex++];

	for (int i = 1; i < 10; i++)
	{
		BinaryNode(g_pRoot, g_iData[i]);
	}
	for (int i = 0; i < 10; i++)
	{
		TNode* pfind = BinarySearch(g_pRoot, g_iData[i]);
		if(pfind!=nullptr)
			printf(" %d", pfind->iData);
	}
}
