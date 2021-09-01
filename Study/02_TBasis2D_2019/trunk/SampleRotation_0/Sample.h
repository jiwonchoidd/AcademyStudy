#pragma once
#include "TCore.h"
#include "xBitmap.h"
class Sample : public TCore
{
public:
	xBitmap    m_BackGround;
	xBitmap    m_ColorBitmap;
	xBitmap    m_MaskBitmap;
	float      m_fAngle;
	HBRUSH	   m_back;
	RECT	   m_rt;
	HDC		   m_RotationBitmapDC;
	HDC        m_hMaskMemDC;
	HDC        m_hColorMemDC;
public:
	bool		Init();
	bool		Frame();
	bool		Render();
	bool		Release();
	void		DrawObject();
	void		DrawBackGround();
	void		DrawRotate(float fAngle);
	HBITMAP		GetRotationBimap(HDC hdc,
				float fAngle, 
				int iWidth, 
				int iHeight);
public:
	Sample();
	virtual ~Sample();
};

