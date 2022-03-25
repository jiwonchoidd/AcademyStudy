#pragma once
#include "K3DAsset.h"
#include <queue>
class KNode
{
public:
	KNode*  m_pParent;
	KRect	m_rect;
	int		m_index;
	int		m_data;
	int		m_depth;
public:
	KNode*		m_pChildlist[4]; //자식 노드
	KNode*		m_pNeighborlist[4];// 이웃 노드
	std::vector<DWORD>		m_CornerList;// 노드의 코너
	KVector3				m_Center;
	UINT    m_LodLevel;
	POINT	m_Element;				// 위치 포인트
	bool	m_bLeaf;
public:
	std::vector <DWORD>		    m_IndexList;
	std::vector <PNCT_VERTEX>	m_VertexList;
	wrl::ComPtr<ID3D11Buffer>	m_pVertexBuffer;
	//wrl::ComPtr<ID3D11Buffer>	m_pIndexBuffer;
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
	void SetRect(float x, float y, float z, float w, float h)
	{
		this->m_rect.size = KVector2(w, h);
		this->m_rect.min = KVector2(x, z);
		this->m_rect.max.x = this->m_rect.min.x + w;
		this->m_rect.max.y = this->m_rect.min.y - h;
		m_Center.x = x + (w / 2.0f);
		m_Center.y = y;
		m_Center.z = z - (h / 2.0f);
	}
public:
	KNode()
	{
		m_pParent = nullptr;
		m_index =0;
		m_data = 0;
		m_depth = 0;
		m_Element = {0,0};				// 위치 포인트
		m_bLeaf = false;
	}
	KNode(float x, float y, float w, float h)
	{
		m_pParent = nullptr; m_Element = { 0,0 };
		m_bLeaf = false; m_data = 0;
		m_index = 0; m_depth = 0;
		m_CornerList.push_back(x);
		m_CornerList.push_back(y);
		m_CornerList.push_back(w);
		m_CornerList.push_back(h);
	}
	~KNode()
	{
		for (int iChild = 0; iChild < 4; iChild++)
		{
			if (m_pChildlist[iChild] != nullptr)
			{
				m_pChildlist[iChild]->m_pVertexBuffer.Reset();
				//m_pChildlist[iChild]->m_pIndexBuffer.Reset();
				delete m_pChildlist[iChild];
				m_pChildlist[iChild] = nullptr;
			}
		}
		m_pVertexBuffer.Reset();
	}
};

class KQuadTree
{
public:
	int					m_width;
	int					m_height;
	int					m_maxDepth;
public:
	KNode*				m_pRootNode;
	std::map<int, KNode*> m_pLeafList;
	std::queue<KNode*>	m_queue;
public:
	bool	LoadLeafData(std::wstring data);
public:
	void    Buildtree(KNode* pNode);
	void	SetNeighborNode();
	KNode*	FindLeafNode(KVector2 pos);
	KNode*  FindNode(KNode* pNode, KVector2 pos);
	bool	SubDivide(KNode* pNode);
public:
	virtual bool	UpdateVertexList(KNode* pNode); // 가상 함수
	virtual HRESULT	CreateVertexBuffer(KNode* pNode); // 가상 함수
	virtual KNode*  CreateNode(KNode* pParent, float x, float y, float w, float h); // 가상 함수
public:
	virtual bool	Init(float width, float height);
	virtual bool	Frame();
	virtual bool	Render(ID3D11DeviceContext* pContext);
	virtual bool	Release();
public:
	KQuadTree();
	virtual ~KQuadTree();
};

