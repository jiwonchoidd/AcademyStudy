#include "KNpc2D.h"

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
