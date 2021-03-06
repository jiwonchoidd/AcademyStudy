#include "KNpc2D.h"
void KNpc2D::HitOverlap(KCollider* pObj, DWORD dwState)
{
	if (dwState == KCollisionType::Overlap)
	{
		int kkk = 0;
	}
}
void	KNpc2D::SelectOverlap(KCollider* pObj, DWORD dwState)
{
	if (m_SelectState & S_HOVER)
	{
		INT K = 0;
	}
	if (m_SelectState & S_FOCUS)
	{
		INT K = 0;
	}
	if (m_SelectState & S_ACTIVE)
	{
		INT K = 0;
	}
	if (m_SelectState & S_SELECTED)
	{
		m_bSelect = true;
	}
}
bool KNpc2D::Init(ID3D11DeviceContext* context, std::wstring vs, std::wstring ps, std::wstring tex, std::wstring mask)
{
	m_pContext = context;
	K2DAsset::CreateObject_Mask(vs, ps, tex, mask);
	return true;
}

bool KNpc2D::Frame()
{
	AddPosition(KVector2(0, 0), m_pContext);
	return true;
}

KNpc2D::KNpc2D()
{
	m_pContext = nullptr;
}

KNpc2D::~KNpc2D()
{
}
