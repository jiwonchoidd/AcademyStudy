#pragma once
#include "TNode.h"
#include <queue>

class KQuadtree
{
public:
	TNode*		m_pRootNode;
	int			m_iNumCol;
	int			m_iNumRow;
	std::queue<TNode*>  m_Queue;
	std::vector<TNode*>  m_pLeafList;
	KMap*		m_pMap;
public:
	bool	UpdateIndexList(TNode* pNode);
	HRESULT CreateIndexBuffer(TNode* pNode);
	void    Build(KMap* pMap);
	bool    Init();
	TNode*  CreateNode(TNode* pParent, float x, float y, float w, float h);
	void	Buildtree(TNode*);
	bool    AddObject(KVector2 pos);
	TNode*  FindNode(TNode* pNode, KVector2 pos);
	
	TNode*  FindPlayerNode(KVector2 pos);
	bool	SubDivide(TNode* pNode);
public:
	bool	Frame();
	bool	Render(ID3D11DeviceContext* pContext);
	bool    Release();
public:
	KQuadtree();
	virtual ~KQuadtree();
};

