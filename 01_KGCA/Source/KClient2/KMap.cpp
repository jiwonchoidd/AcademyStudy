#include "KMap.h"
#include "ImGuiManager.h"
void KMap::SetMatrix(KMatrix* pMatWorld, KMatrix* pMatView, KMatrix* pMatProj)
{
	if (pMatWorld != nullptr)
	{
		m_cbData.matWorld = pMatWorld->Transpose();
		m_Space.m_cbData.matWorld = pMatWorld->Transpose();
	}
	if (pMatView != nullptr)
	{
		m_cbData.matView = pMatView->Transpose();
		m_Space.m_cbData.matView = pMatView->Transpose();
	}
	if (pMatProj != nullptr)
	{
		m_cbData.matProj = pMatProj->Transpose();
		m_Space.m_cbData.matProj = pMatProj->Transpose();
	}
	m_cbData.matNormal = m_cbData.matNormal.Transpose();
	m_Space.m_cbData.matNormal = m_Space.m_cbData.matNormal.Transpose();
}
bool KMap::Init(ID3D11DeviceContext* context,std::wstring vs, std::wstring ps,
	std::wstring tex, std::wstring mask)
{
	m_pContext = context;
	m_CollisonType = Ignore;
	m_SelectType = Select_Ignore;
	m_Name = L"Map";
	K2DAsset::CreateObject_Mask(vs, ps, tex, mask);
	m_Space.Init(m_pContext, m_rtSize.width, m_rtSize.height);
	return true;
}

bool KMap::Frame()
{
	KObject::Frame();
	return true;
}
bool KMap::Render(ID3D11DeviceContext* context)
{
	//KObject::Render(context);
	m_Space.Render(context);
	return true;
}
void KMap::HitOverlap(KCollider* pObj, DWORD dwState)
{

}

bool KMap::Release()
{
	m_Space.Release();
	return true;
}

KMap::KMap()
{
	m_pContext = nullptr;
}

KMap::~KMap()
{
}
