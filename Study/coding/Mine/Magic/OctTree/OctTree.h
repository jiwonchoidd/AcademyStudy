#pragma once
#include "TNode.h"
#include "TObject.h"
#include <queue>
class OctTree 
{
public:
	TNode* m_pRootNode;
	int		m_iWidth;
	int		m_iHeight;
	std::queue<TNode*> m_Queue;
	TObject Tobj;
public:
	bool Init(int iWidth, int iHeight);
	void Buildtree(TNode* t);
	void Frame(float time);
	void Move(TNode* pNode, TVector pos);
	TNode* CreateNode(TNode* p, float x, float y, float z, float w, float h);
	TNode* FindNode(TNode* pNode, TVector pos);
	TNode* FindPlayerNode(TVector pos);
	bool    AddObject(TVector pos);
	void    Release();
public:
	OctTree();
	virtual ~OctTree();
};