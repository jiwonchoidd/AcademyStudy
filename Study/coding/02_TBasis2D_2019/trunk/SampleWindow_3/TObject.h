#pragma once
#include "TBitmap.h"
struct TRect
{
	int   x, y;
	int   w, h;
};
class TObject
{
public :
	TBitmap*	m_pBitmap;
	int			m_iBimapID;

	float		m_fSpeed;
	float		m_fPosX;
	float		m_fPosY;
	float		m_fDirection[2];
	TRect		m_rt;
	TRect		m_rtCollide;
public:
	bool     Load(	HDC hScreenDC,
					HDC hOffScreenDC,
					TCHAR* pFileName);
	virtual bool     Init();
	virtual bool     Frame();
	virtual bool     Render();
	virtual bool     Release();
public:
	bool     Draw();
	void	 SetPos(int iX, int iY, float fSpeed);
	void	 SetSpeed(float fSpeed=1.0f);
	void	 SetRect(int x, int y, int w, int h);
public:
	TObject();
	virtual ~TObject();
};

