#pragma once
#include "TVector.h"
#include "TRect.h"
class TNode
{
public:
	TRect m_TRect;
	std::vector<TVector>  m_ObjectList;
	//¿ÁÆ®¸® 8°³
	TNode* m_pChild[8];
	TNode* m_pParent;
	int m_iDepth;
	int	m_iIndex;
public:
	bool AddObject(float fX, float fY, float fZ);
	bool isRect(TVector pos);
	bool AddObject(TVector pos);
public:
	TNode();
	TNode(float x, float y, float z, float w, float h);
	~TNode();
};