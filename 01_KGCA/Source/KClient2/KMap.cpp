#include "KMap.h"

bool KMap::Init(ID3D11DeviceContext* context, std::wstring vs, std::wstring ps, std::wstring tex, std::wstring mask)
{
	m_pContext = context;

	m_CollisonType = Ignore;
	m_SelectType = Select_Ignore;
	m_Name = L"Map0";
	K2DAsset::CreateObject_Mask(vs, ps, tex, mask);
	return true;
}

bool KMap::Frame()
{
	KObject::Frame();
	return true;
}
bool KMap::Render(ID3D11DeviceContext* pContext)
{
	KObject::Render(pContext);
	return true;
}
void KMap::HitOverlap(KCollider* pObj, DWORD dwState)
{

}

KMap::KMap()
{
	m_pContext = nullptr;
}

KMap::~KMap()
{
}
