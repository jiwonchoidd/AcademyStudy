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
	m_CollisonType = Overlap;
	m_SelectType = Select_Ignore;
	m_Name = L"Map";
	K2DAsset::CreateObject_Mask(vs, ps, tex, mask);

	//����Ʈ�� ��������, 2��° ���ڴ� ���� ��ǥ, 10, 10 �̸� -5, -5�� ���� ��ǥ
	m_Space.Init(m_pContext, {0, 0},m_rtSize.width, m_rtSize.height);

	return true;
}

bool KMap::Frame()
{
	if (g_InputData.bDebugRender)
	{
		m_bDebugRender = !m_bDebugRender;
	}

	//�÷��̾��� ��ġ�� ã�� �ش��ϴ� ��带 ��ȯ�Ѵ�,
	if (m_Player != nullptr)
	{
		KNode* pNode = m_Space.FindLeafNode(m_Player->m_pos);
		if (pNode != nullptr)
		{
			if (pNode->m_data != NULL)
			{
				// ����, ������, �Ʒ���, ����
				m_Player->m_blockstate = pNode->m_data;
			}
			else
			{
				m_Player->m_blockstate = 0;
			}
		}
		if(ImGui::Begin("map node"))
		{
			ImGui::Text(u8"�ش� ��� ���� %d %d", pNode->m_index, pNode->m_data);
		}
		ImGui::End();
	}
	KObject::Frame();
	return true;
}
bool KMap::Render(ID3D11DeviceContext* context)
{
	if(m_bDebugRender)
	m_Space.Render(context);
	KObject::Render(context);
	return true;
}
void KMap::HitOverlap(KCollider* pObj, DWORD dwState)
{
	int k = 0;
}

//�÷��̾� ��ġ
void KMap::ObjectOverlap(KCollider* pObj, DWORD dwState)
{
	if (m_Player == nullptr)
	{
		if (pObj->m_Name == L"Player0")
		{
			m_Player = pObj;
		}
	}
}

bool KMap::Release()
{
	m_Space.Release();
	return true;
}

KMap::KMap()
{
	m_Player = nullptr;
	m_pContext = nullptr;
}

KMap::~KMap()
{
}
