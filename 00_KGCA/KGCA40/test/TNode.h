#pragma once
#include "TStd.h"
#include "KMap.h"
struct Rect
{
	float  x, y, w, h;
};
struct TRect
{
	KVector2  p0;
	KVector2  p1;
	KVector2  wh;
};

class TNode
{
public:
	static      int g_iNewCounter;
	int			m_iIndex;
	TRect		m_tRect;
	std::vector<KVector2>  m_ObjectList;
	std::vector<DWORD>  m_CornerList;
	std::vector<DWORD>  m_IndexList;
	ID3D11Buffer*		m_pIndexBuffer;
	TNode*		m_pChild[4];
	int			m_iDepth;
	bool		m_bLeaf;
	TNode*		m_pParent;
public:	
	bool		AddObject(float fX, float fY);
	bool		AddObject(KVector2 pos);
	bool		IsRect(KVector2 pos);
public:
	TNode() {};
	TNode(UINT x, UINT y, UINT w, UINT h);
	~TNode();
};

