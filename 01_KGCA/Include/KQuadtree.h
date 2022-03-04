#pragma once
#include "KCollider.h"
#include <queue>
class KNode
{
public:
	KNode* m_pParent;
	KRect	m_rect;
	int		m_data;
	int		m_depth;
	KNode* m_pChild[4];
	std::vector<DWORD>  m_CornerList;
public:
	bool isRect(KVector2 pos)
	{
		if (this->m_rect.min.x <= pos.x &&
			this->m_rect.max.x >= pos.x &&
			this->m_rect.min.y <= pos.y &&
			this->m_rect.max.y >= pos.y)
		{
			return true;
		}
		return false;
	}
public:
	KNode()
	{
		m_pParent = nullptr;
	}
	KNode(float x, float y, float w, float h)
	{
		m_pParent = nullptr;
		m_depth = 0;
		m_rect = KRect({ x,y }, w, h);
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

class KQuadTree : public KObject
{

public:
	KNode*				m_pRootNode;
	std::vector<KNode*> m_pReafNode;
	int					m_width;
	int					m_height;
	std::queue<KNode*>	m_queue;
	bool	Init(ID3D11DeviceContext* contex, float width, float height);
	void    Buildtree(KNode* pNode);
	bool	Release();
	KNode* CreateNode(KNode* pParent, float x, float y, float w, float h);
	KNode* FindNode(KNode* pNode, KVector2 pos);
	bool	CheckVertexData() override;
	bool	Render(ID3D11DeviceContext* pContext)override;
public:
	KQuadTree();
	virtual ~KQuadTree();
};

