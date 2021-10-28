#pragma once
#include "KNode.h"
#include <queue>
class KQuadtree
{
public:
	KNode* m_pRootNode;
	int			m_iNumCol;
	int			m_iNumRow;
	std::queue<KNode*>  m_Queue;
	std::vector<KNode*>  m_pLeafList;
	MapObj* m_pMap;
public:
	bool	UpdateIndexList(KNode* pNode);
	HRESULT CreateIndexBuffer(KNode* pNode);
	void    Build(MapObj* pMap);
	bool    Init();
	KNode* CreateNode(KNode* pParent, float x, float y, float w, float h);
	void	Buildtree(KNode*);
	bool    AddObject(KVector2 pos);
	KNode* FindNode(KNode* pNode, KVector2 pos);

	KNode* FindPlayerNode(KVector2 pos);
	bool	SubDivide(KNode* pNode);
public:
	bool	Frame();
	bool	Render(ID3D11DeviceContext* pContext);
	bool    Release();
public:
	KQuadtree();
	virtual ~KQuadtree();
};

