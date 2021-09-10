#include "TShape.h"
#include "TBitmapMgr.h"
void	TShape::SetPos(tPOINT pos)
{
	m_ObjPos = pos;
	m_rtStart.x = m_ObjPos.x - m_rtObj.right / 2.0f;
	m_rtStart.y = m_ObjPos.y - m_rtObj.bottom / 2.0f;
	m_rtCollision.left = m_rtStart.x;
	m_rtCollision.top = m_rtStart.y;
	m_rtCollision.right = m_rtObj.right + m_rtCollision.left;
	m_rtCollision.bottom = m_rtObj.bottom + m_rtCollision.top;
}
void		TShape::SetRect(RECT rt, tPOINT pos)
{
	m_rtObj = rt;// { 90, 1, 40, 60 };
	m_ObjPos = pos;
	m_rtStart.x = m_ObjPos.x - rt.right / 2.0f;
	m_rtStart.x = m_ObjPos.y - rt.bottom / 2.0f;
	m_rtCollision.left = m_rtStart.x;
	m_rtCollision.top = m_rtStart.y;
	m_rtCollision.right = m_rtObj.right + m_rtCollision.left;
	m_rtCollision.bottom = m_rtObj.bottom + m_rtCollision.top;
};
bool		TShape::Load(HDC hdc,
	HDC hOffScrenDC,
	TCHAR* pszLoadFileName,
	TCHAR* pszMaskLoadFileName )
{
	m_hScreenDC = hdc;
	m_hOffScreenDC = hOffScrenDC;

	if (pszMaskLoadFileName != NULL)
	{
		m_bMask = true;
		m_hOffScreenDC = hOffScrenDC;
		m_iBitmapMask = I_BMgr.Add(pszMaskLoadFileName);
		m_pBitmapMask = I_BMgr.GetPtr(m_iBitmapMask);
		m_pBitmapMask->Set(hdc);
		if (m_iBitmapMask < 0)
		{
			return false;
		}
	}
	
	if (pszLoadFileName != NULL)
	{
		m_iBitmap = I_BMgr.Add(pszLoadFileName);
		m_pBitmap = I_BMgr.GetPtr(m_iBitmap);
		m_pBitmap->Set(hdc);
		if (m_iBitmap >= 0)
		{
			m_hBrush = (HBRUSH)GetStockObject(GRAY_BRUSH);
			m_hOldBrush = (HBRUSH)SelectObject(hdc, m_hBrush);
			return true;
		}
	}
	return false;
};
void		TShape::RotateBlt(float iAngle)
{
	int iOffWidth = m_rtObj.right;
	int iOffHeight = m_rtObj.bottom;
	if (iOffWidth > iOffHeight)
	{
		iOffHeight = iOffWidth;
	}
	else
	{
		iOffWidth = iOffHeight;
	}

	HBITMAP hMaskBitmap = GetRotationBimap(m_pBitmapMask->m_hMemDC, iAngle,
		iOffWidth, iOffHeight);
	HBITMAP hColorBitmap = GetRotationBimap(m_pBitmap->m_hMemDC, iAngle,
		iOffWidth, iOffHeight);

	HDC hMaskMemDC = CreateCompatibleDC(m_hScreenDC);
	HDC hColorMemDC = CreateCompatibleDC(m_hScreenDC);
	HBITMAP hOldMask = (HBITMAP)SelectObject(hMaskMemDC, hMaskBitmap);
	HBITMAP hOldColor = (HBITMAP)SelectObject(hColorMemDC, hColorBitmap);

	BitBlt(m_hOffScreenDC,
		m_rtStart.x, m_rtStart.y,
		iOffWidth, iOffHeight,
		hMaskMemDC, 0, 0, SRCAND);
	BitBlt(m_hOffScreenDC, m_rtStart.x, m_rtStart.y,
		iOffWidth, iOffHeight,
		hColorMemDC, 0, 0, SRCINVERT);
	BitBlt(m_hOffScreenDC, m_rtStart.x, m_rtStart.y,
		iOffWidth, iOffHeight,		// 원본 영역
		hMaskMemDC, 0, 0, SRCINVERT);

	SelectObject(hMaskMemDC, hOldMask);
	SelectObject(hColorMemDC, hOldColor);
	DeleteDC(hMaskMemDC);
	DeleteDC(hColorMemDC);
	DeleteObject(hMaskBitmap);
	DeleteObject(hColorBitmap);
};
HBITMAP TShape::GetRotationBimap(HDC hdc,
	float fAngle,
	int iWidth,
	int iHeight)
{
	float fRadian = fAngle * 3.141592f / 180.0f;
	float fCosine = cos(fRadian);// *0.5f + 0.5f;
	float fSine = sin(fRadian);

	// 제 2의 DC 결과물이 hbmResult;
	HDC m_hRotationDC = CreateCompatibleDC(m_hScreenDC);
	HBITMAP hbmResult = CreateCompatibleBitmap(m_hScreenDC, iWidth, iHeight);
	HBITMAP hbmOldDest = (HBITMAP)SelectObject(m_hRotationDC, hbmResult);
	// m_hRotationDC 클리어
	COLORREF bkColor = RGB(255, 255, 255);
	HBRUSH hbrBack = CreateSolidBrush(bkColor);
	HBRUSH hbrOld = (HBRUSH)SelectObject(m_hRotationDC, hbrBack);
	PatBlt(m_hRotationDC, 0, 0, iWidth, iHeight, PATCOPY);
	DeleteObject(SelectObject(m_hRotationDC, hbrOld));

	int iOldGraphic = SetGraphicsMode(m_hRotationDC, GM_ADVANCED);

	XFORM xform;
	xform.eM11 = fCosine; xform.eM12 = -fSine;
	xform.eM21 = fSine; xform.eM22 = fCosine;
	xform.eDx = iWidth / 2;
	xform.eDy = iHeight / 2;
	SetWorldTransform(m_hRotationDC, &xform);

	BitBlt(m_hRotationDC,
		-(m_rtObj.right / 2.0f),
		-(m_rtObj.bottom / 2.0f),
		m_rtObj.right,
		m_rtObj.bottom,
		hdc, m_rtObj.left, m_rtObj.top, SRCCOPY);

	SelectObject(m_hRotationDC, hbmOldDest);
	DeleteObject(m_hRotationDC);

	xform.eM11 = 1; xform.eM12 = 0;
	xform.eM21 = 0; xform.eM22 = 1;
	xform.eDx = 0; xform.eDy = 0;
	SetWorldTransform(m_hRotationDC, &xform);
	SetGraphicsMode(m_hRotationDC, iOldGraphic);
	return hbmResult;
}
bool		TShape::Render()
{
	if (m_bMask==false)
	{
		m_pBitmap->Draw(m_hOffScreenDC,
			m_rtStart,
			m_rtObj,
			m_dwReverse,
			SRCCOPY);
		return true;
	}
	if (m_bMask)
	{
		m_pBitmapMask->Draw(m_hOffScreenDC,
			m_rtStart,
			m_rtObj,
			m_dwReverse,
			SRCAND);
		m_pBitmap->Draw(	m_hOffScreenDC, 
			m_rtStart,
						m_rtObj, 
						m_dwReverse,
						SRCINVERT);
		m_pBitmapMask->Draw(m_hOffScreenDC,
			m_rtStart,
			m_rtObj,
			m_dwReverse,
			SRCINVERT);
	}	
	return true;
};
bool		TShape::Init()
{
	return true;
};
bool		TShape::Frame()
{
	return true;
};
bool		TShape::Release()
{
	return true;
};
TShape::TShape()
{
	m_ObjPos.x = 0;
	m_ObjPos.y = 0;
	m_dwReverse = 0;
	m_bMask = false;
}


TShape::~TShape()
{
}
