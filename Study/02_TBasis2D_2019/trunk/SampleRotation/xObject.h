#pragma once
#include "TBitmap.h"
class xObject
{
public:
	HDC			m_hScreenDC;
	TBitmap		m_Bitmap;
	TBitmap		m_BitmapMask;
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
public:
	bool		Init();
	bool		Frame();
	bool		Render();
	bool		Release();
public:
	HBITMAP		MakeDIBSection(HDC hdc, int width, int height);
	void		SetRect(RECT rt, 
						tPOINT pos, 
						bool bHero=false, 
						float fSpeed= 10.0f);
	bool		Load(	HDC hScreenDC, 
						HDC hOffScrenDC, 
						TCHAR* pszMaskLoadFileName,
						TCHAR* pszLoadFileName);
	void		RotateBlt(float iAngle, float fAlpha=255);
	void		GetRotationBimap(HDC hdc, HDC srdDC,
								float fAngle,
								int iWidth,
								int iHeight);
	void		Left();
	void		Right();
	void		Up();
	void		Down();

	// clrMask �ȼ��� ����ó���Ѵ�.
	void		TransBitBlt(HDC hdc, int x, int y, int width, int height,
		HDC MemDC, int mx, int my, COLORREF clrMask);
	bool AlphaBlend(HDC dcDest, int x, int y, int cx, int cy,
		HDC dcSrc,
		HDC dcMaskSrc, int sx, int sy, int scx, int scy, int alpha, DWORD opMode, COLORREF rgbMask);
public:
	xObject();
	virtual ~xObject();
};

