#include "TQuadtree.h"
bool TQuadtree::Init(int iWidth, int iHeight)
{
	m_iWidth = iWidth;
	m_iHeigth = iHeight;
	m_pRootNode = CreateNode(nullptr, 0, 0, iWidth, iHeight);
	Buildtree(m_pRootNode);
	return true;
}

void TQuadtree::Buildtree(TNode* pNode)
{
	if (pNode->m_tRect.wh.x >= 30.0f &&
		pNode->m_tRect.wh.y >= 30.0f)
	{
		pNode->m_pChild[0] = CreateNode(pNode, pNode->m_tRect.p0.x,
			pNode->m_tRect.p0.y,
			pNode->m_tRect.wh.x / 2.0f,
			pNode->m_tRect.wh.y / 2.0f);
		Buildtree(pNode->m_pChild[0]);

		pNode->m_pChild[1]=CreateNode(pNode,pNode->m_pChild[0]->m_tRect.p1.x,
			pNode->m_pChild[0]->m_tRect.p0.y,
			pNode->m_pChild[0]->m_tRect.wh.x,
			pNode->m_pChild[0]->m_tRect.wh.y);
		Buildtree(pNode->m_pChild[1]);
		pNode->m_pChild[2] = CreateNode(pNode, pNode->m_pChild[0]->m_tRect.p1.x,
			pNode->m_pChild[0]->m_tRect.p1.y,
			pNode->m_pChild[0]->m_tRect.wh.x,
			pNode->m_pChild[0]->m_tRect.wh.y);
		Buildtree(pNode->m_pChild[2]);

		pNode->m_pChild[3] = CreateNode(pNode, pNode->m_pChild[0]->m_tRect.p0.x,
			pNode->m_pChild[0]->m_tRect.p1.y,
			pNode->m_pChild[0]->m_tRect.wh.x,
			pNode->m_pChild[0]->m_tRect.wh.y);
		Buildtree(pNode->m_pChild[3]);
	}
}
TNode* TQuadtree::CreateNode(TNode* pParent, float x, float y, float w, float h)
{
	TNode* pNode = new TNode(x, y, w, h);
	if (pParent != nullptr)
	{
		pNode->m_iDepth = pParent->m_iDepth + 1;
		pNode->m_pParent = pParent;
	}
	return pNode;
}

bool    TQuadtree::AddObject(TVector pos)
{
	TNode* pFindNode = FindNode(m_pRootNode, pos);
	if (pFindNode != nullptr)
	{
		pFindNode->AddObject(pos);
		return true;
	}
	return false;
}
TNode* TQuadtree::FindNode(TNode* pNode, TVector pos)
{
	do {
		for (int iNode = 0; iNode < 4; iNode++)
		{
			if (pNode->m_pChild[iNode] != nullptr &&
				pNode->m_pChild[iNode]->isRect(pos))
			{
				m_Queue.push(pNode->m_pChild[iNode]);
				break;
			}
		}
		if (m_Queue.empty())break;
		pNode = m_Queue.front();
		m_Queue.pop();
	} while (pNode);
	return pNode;
}
void TQuadtree::Release()
{
	delete m_pRootNode;
	m_pRootNode = nullptr;
}
TQuadtree::TQuadtree()
{
	m_pRootNode = nullptr;
}

TQuadtree::~TQuadtree()
{
}