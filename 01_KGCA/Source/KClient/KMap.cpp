#include "KMap2D.h"

bool KMap2D::Init(ID3D11DeviceContext* context, std::wstring vs, std::wstring ps, std::wstring tex, std::wstring mask)
{
	m_pContext = context;
	K2DAsset::CreateObject_Mask(vs, ps, tex, mask);
	return true;
}

bool KMap2D::Frame()
{
	KObject::Frame();
	return true;
}
bool KMap2D::Render(ID3D11DeviceContext* pContext)
{
	KObject::Render(pContext);
	return true;
}
void KMap2D::HitOverlap(KCollider* pObj, DWORD dwState)
{

}


KMap2D::KMap2D()
{
	m_pContext = nullptr;
}

KMap2D::~KMap2D()
{
}
