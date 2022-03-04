#include "KQuadTree.h"

KNode* KQuadTree::CreateNode(KNode* p, float x, float y, float w, float h)
{
	KNode* pNode = new KNode(x, y, w, h);

	if (p != nullptr)
	{
		pNode->m_depth = p->m_depth + 1;
		pNode->m_pParent = p;
	}

	return pNode;
}

void KQuadTree::Buildtree(float width, float height)
{

}

KQuadTree::KQuadTree()
{
}

bool KQuadTree::Subdivide(KNode* pNode)
{
	//잘라질수 있는지?
	if ((pNode->m_CornerList[1] - pNode->m_CornerList[0]) > 4)
	{
		return true;
	}
	return false;
}

KQuadTree::~KQuadTree()
{
}
