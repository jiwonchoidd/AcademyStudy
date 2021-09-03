#pragma once
#include "TStd.h"
#include "TBitmap.h"
#include "TVector2.h"
class TShape
{
public:
	HDC			m_hScreenDC;
	TBitmap*	m_pBitmap;
	TBitmap*	m_pBitmapMask;
	int			m_iBitmap;
	int			m_iBitmapMask;
	bool		m_bMask;
	HDC			m_hOffScreenDC;
	HBRUSH		m_hBrush;
	HBRUSH		m_hOldBrush;
	tPOINT		m_ObjPos; // 위치(좌,상단이 원점)
	tPOINT		m_ObjBeforePos; // 위치(좌,상단이 원점)
	RECT		m_rtObj;  // 원본이미지에서 드로우할 영역
	RECT		m_rtCollision;  // 충돌영역(클라이언트 좌표계)
	DWORD		m_dwReverse;
	tPOINT		m_rtStart;
	tPOINT		m_tTargetPos;
	XFORM		m_xform;
	int			m_iRadius;
	float		m_fAngle;
	Vector2		m_vLook;
	bool		m_bStart;
	bool		m_bHero;
public:
	int			GetRadius() { return m_iRadius; }
	void		SetRadius(int iRadius) { m_iRadius = iRadius; }
	float		GetAngle() { return m_fAngle; }
	void		SetAngle(float fAngle) { m_fAngle += fAngle; }
	virtual POINT       getPosition();
	virtual void		SetTargetPos(tPOINT pos);
	virtual void		SetPos(tPOINT pos);
	virtual void		SetRect(RECT rt, tPOINT pos);
	virtual bool		Load(	HDC hdc,
								HDC hOffScrenDC,							
								TCHAR* pszLoadFileName=0,
								TCHAR* pszMaskLoadFileName=0 );
	virtual bool		Init();
	virtual bool		Frame();
	virtual bool		Render();
	virtual bool		Release();

	virtual void		RotateBlt();
	virtual HBITMAP		GetRotationBimap(HDC hdc,
										int iWidth,
										int iHeight);
public:
	TShape();
	virtual ~TShape();
};

