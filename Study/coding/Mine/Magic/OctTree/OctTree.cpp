#include "OctTree.h"

bool OctTree::Init(int iWidth, int iHeight)
{
	m_iWidth = iWidth;
	m_iHeight = iHeight;
	m_pRootNode = CreateNode(nullptr, 0, 0, iWidth, iHeight);
	Buildtree(m_pRootNode);
	return true;
}

void OctTree::Buildtree(TNode* pNode)
{
	if (pNode->m_TRect.wh.x >= 30.0f &&
		pNode->m_TRect.wh.y >= 30.0f)
	{
		pNode->m_pChild[0] = CreateNode(pNode, pNode->m_TRect.p0.x,
			pNode->m_TRect.p0.y,
			pNode->m_TRect.wh.x / 2.0f,
			pNode->m_TRect.wh.y / 2.0f);
		Buildtree(pNode->m_pChild[0]);
		//child
		for (int iChild = 1; iChild < 8; iChild++)
		{
		pNode->m_pChild[iChild] = CreateNode(pNode, pNode->m_pChild[0]->m_TRect.p1.x,
			pNode->m_pChild[0]->m_TRect.p0.y,
			pNode->m_pChild[0]->m_TRect.wh.x,
			pNode->m_pChild[0]->m_TRect.wh.y);
		Buildtree(pNode->m_pChild[iChild]);
		}
	}
}

TNode* OctTree::CreateNode(TNode* pParent, float x, float y, float w, float h)
{
	TNode* pNode = new TNode(x, y, w, h);
	if (pParent != nullptr)
	{
		pNode->m_iDepth = pParent->m_iDepth + 1;
		pNode->m_pParent = pParent;
	}
	return pNode;
}

bool OctTree::AddObject(TVector pos)
{
	TNode* pFindNode = FindNode(m_pRootNode, pos);
	if (pFindNode != nullptr)
	{
		pFindNode->AddObject(pos);
		return true;
	}
	return false;
}

TNode* OctTree::FindNode(TNode* pNode, TVector pos)
{
	do {
		for (int iNode = 0; iNode < 8; iNode++)
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

void OctTree::Release()
{
	delete m_pRootNode;
	m_pRootNode = nullptr;
}

OctTree::OctTree()
{
	m_pRootNode = nullptr;
}

OctTree::~OctTree()
{
}
