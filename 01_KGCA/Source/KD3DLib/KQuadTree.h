#pragma once
#include "KCollider.h"
#include <queue>
class KNode
{
public:
	KNode*	m_pParent;
	KRect	m_rect;
	POINT	m_Element;
	int		m_index;
	int		m_depth;
	KNode*	m_pChild[4];
	std::vector<DWORD>  m_CornerList;
	KNode*	m_NeighborList[4];
public:
	KNode()
	{

	}
	KNode(UINT x, UINT y, UINT w, UINT h)
	{
		m_pParent = nullptr;
		this->m_rect.middle = {x,y};
		this->m_rect.min = m_rect.middle - KVector2(w / 2.0f, h / 2.0f);
		this->m_rect.max = m_rect.middle + KVector2(w / 2.0f, h / 2.0f);
		this->m_rect.size.x = w;
		this->m_rect.size.y = h;
	}
	~KNode()
	{
		for (int iChild = 0; iChild < 4; iChild++)
		{
			if (m_pChild[iChild] != nullptr)
			{
				delete m_pChild[iChild];
				m_pChild[iChild] = nullptr;
			}
		}
	}
};

class KQuadTree
{
public:
	KNode*				m_pRootNode;
	int					m_width;
	int					m_height;
	int					m_numcol;
	std::queue<KNode*>	m_queue;
	void    Buildtree(float width, float height);
	KNode*  CreateNode(KNode* pParent, float x, float y, float w, float h);
	bool	Subdivide(KNode* pNode);
public:
	KQuadTree();
	virtual ~KQuadTree();
};

