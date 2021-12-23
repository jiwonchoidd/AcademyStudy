#pragma once
#include "KVector.h"
#include "KRect.h"
class KNode
{
public:
	static int  g_iNewCounter;
	KRect m_KRect;
	std::vector<KVector>  m_ObjectList;
	//¿ÁÆ®¸® 8°³
	KNode* m_pChild[8];
	KNode* m_pParent;
	int m_iDepth;
	int	m_iIndex;
public:
	bool AddObject(float fX, float fY, float fZ);
	bool isRect(KVector pos);
	bool AddObject(KVector pos);
public:
	KNode();
	KNode(float x, float y, float z, float w, float h);
	~KNode();
};