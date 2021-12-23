#pragma once
#include "KNode.h"
#include "KObject.h"
#include <queue>
class KOctree 
{
public:
	KNode* m_pRootNode;
	int		m_iWidth;
	int		m_iHeight;
	std::queue<KNode*> m_Queue;
	KObject Tobj;
public:
	bool Init(int iWidth, int iHeight);
	void Buildtree(KNode* t);
	void Frame(float time);
	void Move(KNode* pNode, KVector pos);
	KNode* CreateNode(KNode* p, float x, float y, float z, float w, float h);
	KNode* FindNode(KNode* pNode, KVector pos);
	KNode* FindPlayerNode(KVector pos);
	bool    AddObject(KVector pos);
	void    Release();
public:
	KOctree();
	virtual ~KOctree();
};