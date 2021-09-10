#pragma once
#include "TStd.h"
#include "TBitmap.h"

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
	RECT		m_rtObj;  // �����̹������� ��ο��� ����
	RECT		m_rtCollision;  // �浹����(Ŭ���̾�Ʈ ��ǥ��)
	DWORD		m_dwReverse;
	tPOINT		m_rtStart;
public:
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

	virtual void		RotateBlt(float iAngle);
	virtual HBITMAP		GetRotationBimap(HDC hdc,
		float fAngle,
		int iWidth,
		int iHeight);
public:
	TShape();
	virtual ~TShape();
};

