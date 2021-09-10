#include <iostream>

#define SAFE_NEW(p,N) {if(!p) p = new N; }
#define SAFE_DEL(p) {if(p) delete p; (p) = NULL; }
#define SAFE_NEW_VALUE(p,N,v) {if(!p) p = new N(v);}

#define SAFE_NEW_ARRAY(p,N,C) {if(!p) p = new N[C]; if(p) memset(p, 0, sizeof(N)*C);}
#define SAFE_DEL_ARRAY(p) {if(p) delete [] p;(p)=NULL;}

#define SAFE_ZERO(p) {p =0;};
struct TNode
{
	int m_iValue;
	int m_iDepth;
	TNode* m_pLeft;
	TNode* m_pRight;
	TNode(int idata) : m_iValue(idata),
		m_pLeft(0), m_pRight(0), m_iDepth(0)
	{}
};
TNode* g_pRoot = NULL;
int  g_iIndex = 0;
bool Add(TNode* pParentNode)
{
	if (pParentNode->m_iDepth > 2)
	{
		return false;
	}
	TNode* pLeft = 0;
	SAFE_NEW_VALUE(pLeft, TNode, ++g_iIndex);
	pLeft->m_iDepth = pParentNode->m_iDepth + 1;
	pParentNode->m_pLeft = pLeft;

	TNode* pRight = 0;
	SAFE_NEW_VALUE(pRight, TNode, ++g_iIndex);
	pRight->m_iDepth = pParentNode->m_iDepth + 1;
	pParentNode->m_pRight = pRight;
	return true;
}
void Build(TNode* pParentNode)
{
	if (Add(pParentNode))
	{
		Build(pParentNode->m_pLeft);
		Build(pParentNode->m_pRight);
	}
}
void PRINT(TNode* pParentNode)
{
	if (pParentNode != NULL)
	{
		std::cout << pParentNode->m_iValue << std::endl;
		PRINT(pParentNode->m_pLeft);
		PRINT(pParentNode->m_pRight);
	}
}
void DELALL(TNode* pParentNode)
{
	if (pParentNode == NULL) return;
	DELALL(pParentNode->m_pLeft);
	DELALL(pParentNode->m_pRight);
	SAFE_DEL(pParentNode);
}
void main()
{
	SAFE_NEW_VALUE(g_pRoot, TNode, 0);
	Build(g_pRoot);
	PRINT(g_pRoot);
	DELALL(g_pRoot);
	SAFE_ZERO(g_pRoot);
	getchar();
}