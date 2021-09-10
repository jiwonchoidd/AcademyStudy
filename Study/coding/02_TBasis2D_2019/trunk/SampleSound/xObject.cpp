#include "xObject.h"
void		xObject::RotateBlt(float iAngle)
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

	GetRotationBimap(m_pBitmapMask->m_hMemDC, m_hhMaskRotateBitmap, iAngle, iOffWidth, iOffHeight);
	GetRotationBimap(m_pBitmap->m_hMemDC, m_hColorRotateBitmap, iAngle, iOffWidth, iOffHeight);

	HBITMAP hOldMask = (HBITMAP)SelectObject(m_hMaskDC, m_hhMaskRotateBitmap);
	HBITMAP hOldColor = (HBITMAP)SelectObject(m_hColorDC, m_hColorRotateBitmap);

	BitBlt(m_hOffScreenDC,
		m_ObjPos.x, m_ObjPos.y,
		iOffWidth, iOffHeight,
		m_hMaskDC, 0, 0, SRCAND);
	BitBlt(m_hOffScreenDC, m_ObjPos.x, m_ObjPos.y,
		iOffWidth, iOffHeight,
		m_hColorDC, 0, 0, SRCINVERT);
	BitBlt(m_hOffScreenDC, m_ObjPos.x, m_ObjPos.y,
		iOffWidth, iOffHeight,		// 원본 영역
		m_hMaskDC, 0, 0, SRCINVERT);

	SelectObject(m_hMaskDC, hOldMask);
	SelectObject(m_hColorDC, hOldColor);

};
HBITMAP xObject::GetRotationBimap(HDC hdc, HBITMAP hbmResult,
	float fAngle,
	int iWidth,
	int iHeight)
{
	float fRadian = fAngle * 3.141592f / 180.0f;
	float fCosine = cos(fRadian);// *0.5f + 0.5f;
	float fSine = sin(fRadian);

	// 제 2의 DC 결과물이 hbmResult;	
	//HBITMAP hbmResult = MakeDIBSection(m_hScreenDC, iWidth, iHeight);	
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


	xform.eM11 = 1; xform.eM12 = 0;
	xform.eM21 = 0; xform.eM22 = 1;
	xform.eDx = 0; xform.eDy = 0;
	SetWorldTransform(m_hRotationDC, &xform);
	SetGraphicsMode(m_hRotationDC, iOldGraphic);
	return hbmResult;
}
bool		xObject::Load(HDC hdc,
	HDC hOffScrenDC,
	TCHAR* pszMaskLoadFileName,
	TCHAR* pszLoadFileName)
{
	m_hScreenDC = hdc;
	m_hRotationDC = CreateCompatibleDC(m_hScreenDC);
	if (pszMaskLoadFileName!=NULL)
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
		m_hMaskDC = CreateCompatibleDC(m_hScreenDC);
	}
	
	m_hOffScreenDC = hOffScrenDC;
	m_iBitmap = I_BMgr.Add(pszLoadFileName);
	m_pBitmap = I_BMgr.GetPtr(m_iBitmap);
	m_pBitmap->Set(hdc);
	if (m_iBitmap >= 0)
	{	
		m_hBrush = (HBRUSH)GetStockObject(GRAY_BRUSH);
		m_hOldBrush = (HBRUSH)SelectObject(
							hdc, m_hBrush);
		m_hColorDC = CreateCompatibleDC(m_hScreenDC);
		return true;
	}
	return false;
};

bool		xObject::Init()
{
	return true;
};
void		xObject::SetRect( RECT rt, tPOINT pos, 
	bool bHero,
	float fSpeed )
{
	m_rtObj = rt;// { 90, 1, 40, 60 };
	m_ObjPos = pos;
	m_bHero = bHero;
	m_fSpeed = fSpeed;
	m_rtCollision.left = m_ObjPos.x;
	m_rtCollision.top = m_ObjPos.y;
	m_rtCollision.right = m_rtObj.right+ m_ObjPos.x;
	m_rtCollision.bottom = m_rtObj.bottom+ m_ObjPos.y;

	int iDistance = sqrt(m_rtObj.right*m_rtObj.right + m_rtObj.bottom*m_rtObj.bottom);
	m_hColorRotateBitmap = CreateCompatibleBitmap(m_hScreenDC, iDistance, iDistance);
	m_hhMaskRotateBitmap = CreateCompatibleBitmap(m_hScreenDC, iDistance, iDistance);
}
void		xObject::Left()
{
	m_ObjPos.x -= g_fSecPerFrame * m_fSpeed;
}
void		xObject::Right()
{
	m_ObjPos.x += g_fSecPerFrame * m_fSpeed;
}
void		xObject::Up()
{
	m_ObjPos.y -= g_fSecPerFrame * m_fSpeed;
}
void		xObject::Down()
{
	m_ObjPos.y += g_fSecPerFrame * m_fSpeed;
}
bool		xObject::Render()
{
	if (m_bMask==false)
	{
		m_pBitmap->Draw(m_hOffScreenDC,
			m_ObjPos,
			m_rtObj,
			m_dwReverse,
			SRCCOPY);
		return true;
	}
	if (m_bMask)
	{
		m_pBitmapMask->Draw(m_hOffScreenDC,
			m_ObjPos,
			m_rtObj,
			m_dwReverse,
			SRCAND);
		m_pBitmap->Draw(	m_hOffScreenDC, 
						m_ObjPos,
						m_rtObj, 
						m_dwReverse,
						SRCINVERT);
		m_pBitmapMask->Draw(m_hOffScreenDC,
			m_ObjPos,
			m_rtObj,
			m_dwReverse,
			SRCINVERT);
	}

	
	return true;
};
bool		xObject::Frame()
{
	if (m_bHero == false)
	{
		if (m_ObjPos.x < g_rtClient.left + 10 ||
			m_ObjPos.x > g_rtClient.right - 60)
		{
			m_iDirection[0] *= -1;
		}
		if (m_ObjPos.y < g_rtClient.top + 10 ||
			m_ObjPos.y > g_rtClient.bottom - 80)
		{
			m_iDirection[1] *= -1;
		}
		float fTime = g_fSecPerFrame;
		if (fTime >= 0.01f)
		{
			fTime = 0.001f;
		}
		m_ObjPos.x += m_iDirection[0] * m_fSpeed * fTime;
		m_ObjPos.y += m_iDirection[1] * m_fSpeed * fTime;
	}
	m_rtCollision.left = m_ObjPos.x;
	m_rtCollision.top = m_ObjPos.y;
	m_rtCollision.right = m_rtObj.right + m_ObjPos.x;
	m_rtCollision.bottom = m_rtObj.bottom + m_ObjPos.y;
	return true;
};
bool		xObject::Release()
{
	DeleteDC(m_hMaskDC);
	DeleteDC(m_hColorDC);
	DeleteDC(m_hRotationDC);
	DeleteObject(m_hhMaskRotateBitmap);
	DeleteObject(m_hColorRotateBitmap);

	DeleteObject(m_hBrush);
	SelectObject(m_pBitmap->m_hScreenDC, m_hOldBrush);
	return true;
};

xObject::xObject()
{
	m_ObjPos.x = 100;
	m_ObjPos.y = 100;
	m_dwReverse = 0;
	m_bHero		= true;
	m_fSpeed = 70.0f;
	m_bMask = false;
	m_iDirection[0] = m_iDirection[1] =1;
}


xObject::~xObject()
{
}
