#include "KPlayer2D.h"
bool KPlayer2D::Frame()
{
	if (g_InputData.bWKey)
	{
		KVector2 pos;
		pos.y -= m_speed * g_fSecPerFrame;
		AddPosition(pos, m_pContext);
	}
	if (g_InputData.bAKey)
	{
		KVector2 pos;
		pos.x -= m_speed * g_fSecPerFrame;
		AddPosition(pos, m_pContext);
	}
	if (g_InputData.bSKey)
	{
		KVector2 pos;
		pos.y += m_speed * g_fSecPerFrame;
		AddPosition(pos, m_pContext);
	}
	if (g_InputData.bDKey)
	{
		KVector2 pos;
		pos.x += m_speed * g_fSecPerFrame;
		AddPosition(pos, m_pContext);
	}
	return true;
}

void KPlayer2D::HitOverlap(KCollider* pObj, DWORD dwState)
{
	if (dwState == KCollisionType::Overlap)
	{
		int kkk = 0;
	}
}

bool KPlayer2D::Init(ID3D11DeviceContext* context, std::wstring vs, std::wstring ps, std::wstring tex, std::wstring mask)
{
	m_speed = 100.0f;
	m_pContext = context;
	K2DAsset::CreateObject_Mask(vs, ps, tex, mask);
	return true;
}

KPlayer2D::KPlayer2D()
{
	m_pContext = nullptr;
	m_speed = 100.0f;
}

KPlayer2D::~KPlayer2D()
{
}
