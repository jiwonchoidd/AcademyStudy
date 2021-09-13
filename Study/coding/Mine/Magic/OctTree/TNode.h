#pragma once
#include "TRect.h"
class TNode
{
public:
	TRect m_TRect;
	std::vector<TVector>  m_ObjectList;
	//¿ÁÆ®¸® 8°³
	TNode* m_pChild[8];
	TNode* m_pParent;
	int m_iDepth = 0;
public:
	bool isRect(TVector pos);
	bool AddObject(TVector pos);
public:
	TNode();
	TNode(float x, float y, float w, float h);
	~TNode();
};