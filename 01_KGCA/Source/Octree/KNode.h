#pragma once
#include "KVector3.h"
#include "KRect.h"
class KNode
{
public:
	static int		     g_iNewCounter;
	KRect				 m_KRect;
	std::vector<KVector3> m_ObjectList;
	//노드 안에 8개의 자식 노드
	KNode*				m_pChild[8];
	KNode*				m_pParent;
	int					m_iDepth;
	int					m_iIndex;
public:
	bool AddObject(float fX, float fY, float fZ);
	bool isRect(KVector3 pos);
	bool AddObject(KVector3 pos);
public:
	KNode();
	KNode(float x, float y, float z, float w, float h);
	~KNode();
};