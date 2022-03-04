#include "Quadtree.h"
int main()
{
	Quadtree qt;
	qt.Init(30, 30);

	return 0;
}

void Quadtree::Init(float width, float height)
{
	m_width = width;
	m_height = height;
	m_pRootNode = CreateNode(nullptr, 0, 0, m_width, m_height);
	Buildtree(m_pRootNode);
}

void Quadtree::Buildtree(KNode* pNode)
{
	if (pNode->m_rect.size.x >= 30.0f &&
		pNode->m_rect.size.y >= 30.0f)
	{
		pNode->m_pChild[0] = CreateNode(pNode, pNode->m_rect.min.x,
			pNode->m_rect.min.y,
			pNode->m_rect.size.x / 2.0f,
			pNode->m_rect.size.y / 2.0f);
		Buildtree(pNode->m_pChild[0]);

		pNode->m_pChild[1] = CreateNode(pNode, pNode->m_pChild[0]->m_rect.max.x,
			pNode->m_pChild[0]->m_rect.min.y,
			pNode->m_pChild[0]->m_rect.size.x,
			pNode->m_pChild[0]->m_rect.size.y);
		Buildtree(pNode->m_pChild[1]);
		pNode->m_pChild[2] = CreateNode(pNode, pNode->m_pChild[0]->m_rect.max.x,
			pNode->m_pChild[0]->m_rect.max.y,
			pNode->m_pChild[0]->m_rect.size.x,
			pNode->m_pChild[0]->m_rect.size.y);
		Buildtree(pNode->m_pChild[2]);

		pNode->m_pChild[3] = CreateNode(pNode, pNode->m_pChild[0]->m_rect.min.x,
			pNode->m_pChild[0]->m_rect.max.y,
			pNode->m_pChild[0]->m_rect.size.x,
			pNode->m_pChild[0]->m_rect.size.y);
		Buildtree(pNode->m_pChild[3]);
	}
}

void Quadtree::Release()
{
	if (m_pRootNode!=nullptr)
	{
		delete m_pRootNode;
		m_pRootNode = nullptr;
	}
	m_pRootNode;
	m_width;
	m_height;
	m_queue;
}

KNode* Quadtree::CreateNode(KNode* pParent, float x, float y, float w, float h)
{
	KNode* pNode = new KNode(x, y, w, h);

	//초기에 부모가 없으므로 예외처리
	if (pParent != nullptr)
	{
		pNode->m_depth = pParent->m_depth + 1;
		pNode->m_pParent = pParent;
	}

	return pNode;
}

KNode* Quadtree::FindNode(KNode* pNode, KVector2 pos)
{
	do {
		for (int iNode = 0; iNode < 4; iNode++)
		{
			if (pNode->m_pChild[iNode] != nullptr &&
				pNode->m_pChild[iNode]->isRect(pos))
			{
				m_queue.push(pNode->m_pChild[iNode]);
				break;
			}
		}
		if (m_queue.empty())break;
		pNode = m_queue.front();
		m_queue.pop();
	} while (pNode);
	return pNode;
}

Quadtree::Quadtree()
{
}

Quadtree::~Quadtree()
{
}
