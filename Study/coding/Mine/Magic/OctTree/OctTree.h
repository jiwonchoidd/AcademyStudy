#pragma once
#include "TNode.h"
#include <queue>
class OctTree 
{
public:
	TNode* m_pRootNode;
	int		m_iWidth;
	int		m_iHeight;
	std::queue<TNode*> m_Queue;
public:
	bool Init(int iWidth, int iHeight);
	void Buildtree(TNode* t);
	TNode* CreateNode(TNode* p, float x, float y, float w, float h);
	bool    AddObject(TVector pos);
	TNode* FindNode(TNode* pNode, TVector pos);
	void    Release();
public:
	OctTree();
	virtual ~OctTree();
};