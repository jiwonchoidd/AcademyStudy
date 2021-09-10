#include "THero.h"
//#include "TSys.h"
void  THero::Left()
{
	m_fPosX -= g_fSecPerFrame * m_fSpeed;
}
void  THero::Right()
{
	m_fPosX += g_fSecPerFrame * m_fSpeed;
}
void  THero::Up()
{
	m_fPosY -= g_fSecPerFrame * m_fSpeed;
}
void  THero::Down()
{
	m_fPosY += g_fSecPerFrame * m_fSpeed;
}
bool THero::Init()
{
	return true;
}

bool THero::Frame()
{
	m_rtCollide.x = m_fPosX - (m_rt.w / 2);
	m_rtCollide.y = m_fPosY - (m_rt.h / 2);
	m_rtCollide.w = m_rtCollide.x + m_rt.w;
	m_rtCollide.h = m_rtCollide.y + m_rt.h;
	return true;
}
// 대상 포지션 X,Y
// 대상 크기 W,H
// 원본 시작위치  X,Y
// 원본 크기 W,H

bool THero::Render()
{
	BitBlt(m_pBitmap->m_hOffScreenDC,
		m_fPosX, m_fPosY,
		m_rt.w,
		m_rt.h,
		m_pBitmap->m_hMemDC,
		m_rt.x, m_rt.y, SRCCOPY);///  원본
								 /*StretchBlt(m_pBitmap->m_hOffScreenDC, m_fPosX, m_fPosY,
								 800,
								 600,
								 m_pBitmap->m_hMemDC,
								 m_pBitmap->m_hBitInfo.bmWidth/2,
								 m_pBitmap->m_hBitInfo.bmHeight/2,
								 m_pBitmap->m_hBitInfo.bmWidth- m_pBitmap->m_hBitInfo.bmWidth / 2,
								 m_pBitmap->m_hBitInfo.bmHeight- m_pBitmap->m_hBitInfo.bmHeight / 2,
								 SRCCOPY); ///  원본*/
	return true;
}

bool THero::Release()
{
	m_pBitmap->Release();
	return true;
}

THero::THero()
{
	m_fDirection[0] = 1.0f;
	m_fDirection[1] = 1.0f;
	m_fSpeed = 0.0f;
	m_fPosX = 0.0f;
	m_fPosY = 0.0f;
}


THero::~THero()
{
}
