#include "xObject.h"
bool		xObject::Load(HDC hdc,
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

bool		xObject::Init()
{
	m_Bitmap.Init();	
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

	// 내부 white, 테두리 red(CreatePen)
	int prevMode = SetROP2(m_hOffScreenDC, R2_NOTXORPEN);
	Rectangle(m_hOffScreenDC, m_ObjPos.x, m_ObjPos.y,
		m_ObjPos.x + m_rtObj.right,
		m_ObjPos.y + m_rtObj.bottom);
	SetROP2(m_hOffScreenDC, prevMode);

	return true;
};
bool		xObject::DrawColorKey()
{
	m_Bitmap.DrawColorKey(m_ObjPos.x, m_ObjPos.y,	m_rtObj, RGB(255,255,255));
	return true;
};
bool		xObject::Frame()
{
	if (m_bDead == false)
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
			if (m_ObjPos.x > g_rtClient.right)
			{
				m_iDirection[0] *= -1;
				m_ObjPos.x = g_rtClient.right;
			}
			if (m_ObjPos.y > g_rtClient.bottom)
			{
				m_iDirection[1] *= -1;
				m_ObjPos.y = g_rtClient.bottom;
			}
			m_ObjPos.x += m_iDirection[0] * m_fSpeed * g_fSecPerFrame;
			m_ObjPos.y += m_iDirection[1] * m_fSpeed * g_fSecPerFrame;
		}
	}
	else
	{
		m_ObjPos.x = 0;
		m_ObjPos.y = 0;
		m_bDead = false;
	}
	m_rtCollision.left = m_ObjPos.x;
	m_rtCollision.top = m_ObjPos.y;
	m_rtCollision.right = m_rtObj.right + m_ObjPos.x;
	m_rtCollision.bottom = m_rtObj.bottom + m_ObjPos.y;	
	return true;
};
bool		xObject::Release()
{
	DeleteObject(m_hBrush);
	SelectObject(m_Bitmap.m_hScreenDC, m_hOldBrush);
	m_Bitmap.Release();
	return true;
};

xObject::xObject()
{
	m_ObjPos.x = 100;
	m_ObjPos.y = 100;
	m_dwReverse = 0;
	m_bHero		= true;
	m_bDead = false;
	m_fSpeed = 70.0f;
	m_bMask = false;
	m_iDirection[0] = m_iDirection[1] =1;
}


xObject::~xObject()
{
}
