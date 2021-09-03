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
	tPOINT		m_ObjPos; // ��ġ(��,����� ����)
	tPOINT		m_ObjBeforePos; // ��ġ(��,����� ����)
	RECT		m_rtObj;  // �����̹������� ��ο��� ����
	RECT		m_rtCollision;  // �浹����(Ŭ���̾�Ʈ ��ǥ��)
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

