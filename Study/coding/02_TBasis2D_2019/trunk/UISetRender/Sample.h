#pragma once
#include "TCore.h"
#include "TObject.h"

struct TOffset
{
	float fScaleX0;
	float fScaleX1;
	float fScaleY0;
	float fScaleY1;
	float fOffsetU0;
	float fOffsetV0;
	float fOffsetU1;
	float fOffsetV1;
};
struct PT_VERTEX
{
	TPoint p;
	TPoint t;
	bool operator == (const PT_VERTEX & Vertex)
	{
		if (p == Vertex.p  &&  t == Vertex.t)
		{
			return true;
		}
		return  false;
	}
	PT_VERTEX() {}
	PT_VERTEX(TPoint		vp, TPoint     vt)
	{
		p = vp, t = vt;
	}
};
class TActor : public TObject
{
public:
	TRect		m_rtInit;
	TOffset		m_tOffset;
	TPoint		m_vCenter;
	PT_VERTEX	m_VertexList[6];
public:
	virtual TPoint  Gen(float x, float y);
	virtual void    GenCenter();
	virtual void    SetVertexData();
	virtual  bool	SetData(float left, float top, float width, float height);

public:
	TActor() {};
	~TActor() {}
};
class TDialogBox : public TActor
{	
public:
	vector<PT_VERTEX> m_tVertexList;
	TActor             m_Actor[9];
	TPoint			   m_cpPoint[16];
public:
	void   SetVertexData();
	bool   SetData(RECT rt, float fScaleX0, float fScaleX1,
		float fScaleY0, float fScaleY1,
		float fOffsetU0, float fOffsetU1,
		float fOffsetV0, float fOffsetV1);
public:
	TDialogBox() {};
	~TDialogBox() {}
};
class Sample : public TCore
{
public:
	TDialogBox			m_ui;
	BITMAPFILEHEADER	m_bitFileHeader;
	POINT				m_BitmapSize;
	HBITMAP				m_hBitmap;
	BITMAP				m_bmpInfo;
public:
	bool LoadDIB(const TCHAR* pFileName);
	bool RenderDIB(int x, int y);
	bool Init();
	bool Frame();
	bool Render();
	bool Release();
public:
	Sample();
	~Sample();
};

