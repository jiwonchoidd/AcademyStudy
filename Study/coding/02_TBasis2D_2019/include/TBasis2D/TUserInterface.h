#pragma once
#include "TObject.h"
#include "TInput.h"
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

struct TRect
{
	float    left;
	float    top;
	float    right;
	float    bottom;
	void operator = (const RECT rt)
	{
		left = (float)rt.left;
		top = (float)rt.top;
		right = (float)rt.right;
		bottom = (float)rt.bottom;
	}
};

struct TIndex
{
	int x, y;
	bool operator == (const TIndex& pos)
	{
		return (x == pos.x && y == pos.y);
	}
	TIndex operator + (const TIndex& right_)
	{
		return{ x + right_.x, y + right_.y };
	}
	TIndex(int fX, int fY)
	{
		x = fX;
		y = fY;
	}
	TIndex()
	{
		x = y = 0;
	}
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

class TUserInterface : public TObject
{
public:

	std::vector<TUserInterface>  m_list;
public:
	virtual LRESULT	 MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	virtual TObject* Clone() override;
	virtual bool	PostFrame() override;
	TUserInterface() 
	{
		m_iObjectType = 300;
	}
};
class TActor : public TUserInterface
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
	virtual bool	Init() override;
	virtual bool	Render() override;
	virtual void  Set(RECT rtSrc, RECT rtDesk) override;
	virtual void  Set(TPoint p, RECT rtDesk) override;
public:
	TDialogBox() {};
	~TDialogBox() {}
};
class TButton : public TUserInterface
{
public:

public:
	bool   Frame();
	bool   Render();
	virtual TObject* Clone() override;
public:
	TButton()
	{
		m_iObjectType = 301;
	}
};
class TEdit : public TUserInterface
{
public:
	T_STR	m_szEdit;
	int		m_iWindowCtrl;
	static int g_iControlID;
	HWND	m_hWndEdit;
	RECT	m_rtWndCtrl;
	bool	m_bEditFocus = false;
public:
	void   CreateEdit();
public:
	virtual TObject* Clone() override;
	bool	Init() override;
	bool	Frame()override;
	bool	Render()override;
	LRESULT	 MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
public:
	TEdit()
	{
		m_iObjectType = 302;
	}
};