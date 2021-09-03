#include "TEnemy.h"

bool TEnemy::Frame()
{
	if (m_ObjPos.x < g_rtClient.left + m_rtObj.right/2)
	{
		m_iDirection[0] *= -1;
		m_ObjPos.x = g_rtClient.left + m_rtObj.right / 2;
	}
	if (m_ObjPos.x > g_rtClient.right - m_rtObj.right / 2)
	{
		m_iDirection[0] *= -1;
		m_ObjPos.x = g_rtClient.right - m_rtObj.right / 2;
	}
	if (m_ObjPos.y < g_rtClient.top + m_rtObj.bottom / 2)
	{
		m_iDirection[1] *= -1;
		m_ObjPos.y = g_rtClient.top + m_rtObj.bottom / 2;
	}
	if (m_ObjPos.y > g_rtClient.bottom - m_rtObj.bottom / 2)
	{
		m_iDirection[1] *= -1;
		m_ObjPos.y = g_rtClient.bottom - m_rtObj.bottom / 2;
	}
	float fTime = g_fSecPerFrame;
	m_ObjPos.x += m_iDirection[0] * m_fSpeed * fTime;
	m_ObjPos.y += m_iDirection[1] * m_fSpeed * fTime;
	SetPos(m_ObjPos);
	return true;
}

TEnemy::TEnemy()
{
	m_iDirection[0] = m_iDirection[1] = 1;
}


TEnemy::~TEnemy()
{
}
