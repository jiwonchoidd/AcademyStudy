#pragma once
#include "TBitmapMgr.h"
class xObject
{
public:
	HDC			m_hScreenDC;
	TBitmap*	m_pBitmap;
	TBitmap*	m_pBitmapMask;
	int			m_iBitmap;
	int			m_iBitmapMask;

	bool		m_bMask;
	tPOINT		m_ObjPos; // ��ġ(��,����� ����)
	RECT		m_rtObj;  // �����̹������� ��ο��� ����
	RECT		m_rtCollision;  // �浹����(Ŭ���̾�Ʈ ��ǥ��)
	HDC			m_hOffScreenDC;
	HBRUSH		m_hBrush;
	HBRUSH		m_hOldBrush;
	DWORD		m_dwReverse;
	bool		m_bHero;
	int			m_iDirection[2];
	float		m_fSpeed;
	// ȸ������ �������
public:
	HDC			m_hColorDC;
	HDC			m_hMaskDC;
	HDC			m_hRotationDC;
	HBITMAP		m_hColorRotateBitmap;
	HBITMAP		m_hhMaskRotateBitmap;
public:
	bool		Init();
	bool		Frame();
	bool		Render();
	bool		Release();
public:
	void		SetRect(RECT rt, 
						tPOINT pos, 
						bool bHero=false, 
						float fSpeed= 10.0f);
	bool		Load(	HDC hScreenDC, 
						HDC hOffScrenDC, 
						TCHAR* pszMaskLoadFileName,
						TCHAR* pszLoadFileName);
	void		RotateBlt(float iAngle);
	HBITMAP		GetRotationBimap(HDC hdc, HBITMAP hbmResult,
		float fAngle,
		int iWidth,
		int iHeight);
	void		Left();
	void		Right();
	void		Up();
	void		Down();
public:
	xObject();
	virtual ~xObject();
};

