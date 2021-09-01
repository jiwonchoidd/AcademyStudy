#include "TShape.h"
#include "TBitmapMgr.h"
POINT        TShape::getPosition()
{
	POINT pos;
	pos.x = m_ObjPos.x;
	pos.y = m_ObjPos.y;
	return pos;
}
void		TShape::SetTargetPos(tPOINT pos)
{
	m_tTargetPos = pos;
}
void	TShape::SetPos(tPOINT pos)
{
	m_ObjPos = pos;	
	if (m_bStart == false && m_bHero == false)
	{
		tPOINT pTarget = m_tTargetPos; // hero
		tPOINT pPos = m_ObjPos; // anemy
		pTarget.y *= -1.0f;
		pPos.y *= -1.0f;

		Vector2 vD;
		vD.x = 0.0f; vD.y = -1.0f;

		m_vLook.x = pTarget.x - pPos.x;
		m_vLook.y = pTarget.y - pPos.y;
		m_vLook = m_vLook.Normal();
		m_fAngle = m_vLook.Angle(vD);
		m_bStart = true;
	}
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
	m_ObjBeforePos = pos;
	SetPos(pos);
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
void		TShape::RotateBlt()
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

	HBITMAP hMaskBitmap = GetRotationBimap(m_pBitmapMask->m_hMemDC, 
		iOffWidth, iOffHeight);
	HBITMAP hColorBitmap = GetRotationBimap(m_pBitmap->m_hMemDC, 
		iOffWidth, iOffHeight);

	HDC m_hMaskDC = CreateCompatibleDC(m_hScreenDC);
	HDC m_hColorDC = CreateCompatibleDC(m_hScreenDC);
	HBITMAP hOldMask = (HBITMAP)SelectObject(m_hMaskDC, hMaskBitmap);
	HBITMAP hOldColor = (HBITMAP)SelectObject(m_hColorDC, hColorBitmap);

	BitBlt(m_hOffScreenDC,
		m_rtStart.x, m_rtStart.y,
		iOffWidth, iOffHeight,
		m_hMaskDC, 0, 0, SRCAND);
	BitBlt(m_hOffScreenDC, m_rtStart.x, m_rtStart.y,
		iOffWidth, iOffHeight,
		m_hColorDC, 0, 0, SRCINVERT);
	BitBlt(m_hOffScreenDC, m_rtStart.x, m_rtStart.y,
		iOffWidth, iOffHeight,		// 원본 영역
		m_hMaskDC, 0, 0, SRCINVERT);

	SelectObject(m_hMaskDC, hOldMask);
	SelectObject(m_hColorDC, hOldColor);
	DeleteDC(m_hMaskDC);
	DeleteDC(m_hColorDC);
	DeleteObject(hMaskBitmap);
	DeleteObject(hColorBitmap);
};
HBITMAP TShape::GetRotationBimap(HDC hdc,
	int iWidth,
	int iHeight)
{
	float fRadian = m_fAngle * 3.141592f / 180.0f;
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
		
	m_xform.eM11 = fCosine; m_xform.eM12 = -fSine;
	m_xform.eM21 = fSine; m_xform.eM22 = fCosine;
	m_xform.eDx = iWidth / 2.0f;
	m_xform.eDy = iHeight / 2.0f;
	SetWorldTransform(m_hRotationDC, &m_xform);

	//m_vLook.x = m_xform.eM12;
	//m_vLook.y = -m_xform.eM22;

	BitBlt(m_hRotationDC,
		-(m_rtObj.right / 2.0f),
		-(m_rtObj.bottom / 2.0f),
		m_rtObj.right,
		m_rtObj.bottom,
		hdc, m_rtObj.left, m_rtObj.top, SRCCOPY);

	SelectObject(m_hRotationDC, hbmOldDest);
	DeleteObject(m_hRotationDC);

	XFORM		xform;
	xform.eM11 = 1.0f; xform.eM12 = 0.0f;
	xform.eM21 = 0.0f; xform.eM22 = 1.0f;
	xform.eDx = 0.0f; xform.eDy = 0.0f;
	SetWorldTransform(m_hRotationDC, &xform);
	SetGraphicsMode(m_hRotationDC, iOldGraphic);
	return hbmResult;
}
bool		TShape::Render()
{
	if ( fabs(m_fAngle-0.0f) >= 0.001f)
	{
		RotateBlt();
		return true;
	}
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
	m_ObjBeforePos = m_ObjPos;
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
	m_fAngle = 0.0f;
	m_iRadius = 0.0f;
	m_ObjPos.x = 0;
	m_ObjPos.y = 0;
	m_dwReverse = 0;
	m_bMask = false;
	m_bStart = false;
}


TShape::~TShape()
{
}
