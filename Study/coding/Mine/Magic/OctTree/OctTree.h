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
	bool Init();
	void Buildtree(TNode* t);
	TNode* CreateNode(TNode* p);
};