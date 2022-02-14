#include "KPlayer2D.h"
bool KPlayer2D::Frame()
{
	if (g_InputData.bWKey)
	{
		KVector2 pos;
		pos.y -= speed * g_fSecPerFrame;
		AddPosition(pos, m_pContext);
	}
	if (g_InputData.bAKey)
	{
		KVector2 pos;
		pos.x -= speed * g_fSecPerFrame;
		AddPosition(pos, m_pContext);
	}
	if (g_InputData.bSKey)
	{
		KVector2 pos;
		pos.y += speed * g_fSecPerFrame;
		AddPosition(pos, m_pContext);
	}
	if (g_InputData.bDKey)
	{
		KVector2 pos;
		pos.x += speed * g_fSecPerFrame;
		AddPosition(pos, m_pContext);
	}
	return true;
}

bool KPlayer2D::Init(ID3D11DeviceContext* context)
{
	speed = 100.0f;
	m_pContext = context;
	return true;
}

KPlayer2D::KPlayer2D()
{
	speed = 100.0f;
}

KPlayer2D::~KPlayer2D()
{
}
