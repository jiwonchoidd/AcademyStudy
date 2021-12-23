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
	KObject	 m_Movableobj;
	KVector3 m_Mp;
public:
	bool Init(int iWidth, int iHeight);
	void Buildtree(KNode* t);
	void Frame(float time);
	void Move(KNode* pNode, KVector3 pos);
	KNode* CreateNode(KNode* p, float x, float y, float z, float w, float h);
	KNode* FindNode(KNode* pNode, KVector3 pos);
	KNode* FindPlayerNode(KVector3 pos);
	bool    AddObject(KVector3 pos);
	void    Release();
public:
	KOctree();
	virtual ~KOctree();
};