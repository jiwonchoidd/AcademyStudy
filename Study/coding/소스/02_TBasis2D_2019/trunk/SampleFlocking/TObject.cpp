#include "TObject.h"
bool		TObject::Load(HDC hdc,
	HDC hOffScrenDC,
	TCHAR* pszMaskLoadFileName,
	TCHAR* pszLoadFileName)
{
	if (pszMaskLoadFileName!=NULL)
	{	
		m_bMask = true;
		m_BitmapMask.Set(hdc);
		m_hOffScreenDC = hOffScrenDC;
		if (m_BitmapMask.Load(pszMaskLoadFileName) == false)
		{
			return false;
		}
	}

	m_Bitmap.Set(hdc);
	m_hOffScreenDC = hOffScrenDC;
	if (m_Bitmap.Load(pszLoadFileName)==true)
	{
		m_hBrush = (HBRUSH)GetStockObject(GRAY_BRUSH);
		m_hOldBrush = (HBRUSH)SelectObject(
							hdc, m_hBrush);
		return true;
	}
	return false;
};

bool		TObject::Init()
{
	m_Bitmap.Init();	
	return true;
};
void		TObject::SetRect( RECT rt, tPOINT pos, 
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
}
void		TObject::Left()
{
	m_ObjPos.x -= g_fSecPerFrame * m_fSpeed;
}
void		TObject::Right()
{
	m_ObjPos.x += g_fSecPerFrame * m_fSpeed;
}
void		TObject::Up()
{
	m_ObjPos.y -= g_fSecPerFrame * m_fSpeed;
}
void		TObject::Down()
{
	m_ObjPos.y += g_fSecPerFrame * m_fSpeed;
}
bool		TObject::Render()
{
	if (m_bMask==false)
	{
		m_Bitmap.Draw(m_hOffScreenDC,
			m_ObjPos,
			m_rtObj,
			m_dwReverse,
			SRCCOPY);
		return true;
	}
	if (m_bMask)
	{
		m_BitmapMask.Draw(m_hOffScreenDC,
			m_ObjPos,
			m_rtObj,
			m_dwReverse,
			SRCAND);
		m_Bitmap.Draw(	m_hOffScreenDC, 
						m_ObjPos,
						m_rtObj, 
						m_dwReverse,
						SRCINVERT);
		m_BitmapMask.Draw(m_hOffScreenDC,
			m_ObjPos,
			m_rtObj,
			m_dwReverse,
			SRCINVERT);
	}

	
	return true;
};
bool		TObject::Frame()
{
	if (m_bHero == false)
	{
		if (m_ObjPos.x < g_rtClient.left)
		{
			m_iDirection[0] *= -1;
			m_ObjPos.x = g_rtClient.left;
		}
		if (m_ObjPos.y < g_rtClient.top)
		{
			m_iDirection[1] *= -1;
			m_ObjPos.y = g_rtClient.top;
		}
		if (	m_ObjPos.x > g_rtClient.right)
		{
			m_iDirection[0] *= -1;
			m_ObjPos.x = g_rtClient.right;
		}
		if (	m_ObjPos.y > g_rtClient.bottom)
		{
			m_iDirection[1] *= -1;
			m_ObjPos.y = g_rtClient.bottom;
		}
		m_ObjPos.x += m_iDirection[0] * m_fSpeed * g_fSecPerFrame;
		m_ObjPos.y += m_iDirection[1] * m_fSpeed * g_fSecPerFrame;
	}
	m_rtCollision.left = m_ObjPos.x;
	m_rtCollision.top = m_ObjPos.y;
	m_rtCollision.right = m_rtObj.right + m_ObjPos.x;
	m_rtCollision.bottom = m_rtObj.bottom + m_ObjPos.y;
	return true;
};
bool		TObject::Release()
{
	DeleteObject(m_hBrush);
	SelectObject(m_Bitmap.m_hScreenDC, m_hOldBrush);
	m_Bitmap.Release();
	return true;
};

TObject::TObject()
{
	m_ObjPos.x = 100;
	m_ObjPos.y = 100;
	m_dwReverse = 0;
	m_bHero		= true;
	m_fSpeed = 70.0f;
	m_bMask = false;
	m_iDirection[0] = m_iDirection[1] =1;
}


TObject::~TObject()
{
}
