#include "KPlayer2D.h"
#include "ImGuiManager.h"
#include "KSpriteManager.h"
bool KPlayer2D::Init(ID3D11DeviceContext* context, std::wstring vs, std::wstring ps, std::wstring tex, std::wstring mask)
{
	m_pContext = context;
	m_Speed = 10.0f;
	m_Name = L"Player0";

	//걷는 애니메이션 한줄 잘라내기
	KSprite* walk = new KSprite;
	walk->m_Name = L"Player_Walk";
	walk->m_anim_time = 0.0f;
	for (int resource = 0; resource < 12; resource++)
	{
		RECT rc = { 15 + (resource * 35),30,35,35 };
		walk->SetAnimArray(rc);
		//홀수면
		if (resource == 1 || resource == 4 || resource == 7 || resource == 10 || resource == 13)
		{
			RECT rc = { 15 + ((resource-1) * 35),30,35,35 };
			walk->SetAnimArray(rc);
		}
	}
	//
	g_SpriteManager.Load(walk);
	
	SetRectSource(walk->m_anim_array[8]);
	
	K2DAsset::CreateObject_Mask(vs, ps, tex, mask);
	return true;
}

bool KPlayer2D::Frame()
{
	KSprite* walk= g_SpriteManager.GetPtr(L"Player_Walk");
	
	KVector2 pos;
	if (g_InputData.bWKey)
	{
		pos.y += m_Speed * g_fSecPerFrame;
		m_dir = {0,1};
		walk->RunAnim(3.0f, 8, 11);
	}
	else if (g_InputData.bSKey)
	{
		pos.y -= m_Speed * g_fSecPerFrame;
		m_dir = { 0,-1 };
		walk->RunAnim(3.0f, 0, 3);
	}
	else if (g_InputData.bAKey)
	{
		pos.x -= m_Speed * g_fSecPerFrame;
		m_dir = { -1,0 };
		walk->RunAnim(3.0f, 12, 15);
	}
	else if (g_InputData.bDKey)
	{
		pos.x += m_Speed * g_fSecPerFrame;
		m_dir = { 1,0 };
		walk->RunAnim(3.0f, 4, 7);
	}
	else
	{
		if (m_dir == KVector2{ 0,1 })
		{
			walk->m_AnimIndex = 8;
		}
		if (m_dir == KVector2{ 0,-1 })
		{
			walk->m_AnimIndex = 0;
		}
		if (m_dir == KVector2{ -1,0 })
		{
			walk->m_AnimIndex = 12;
		}
		if (m_dir == KVector2{ 1,0 })
		{
			walk->m_AnimIndex = 4;
		}
	}
	AddPosition(pos, m_pContext);
	
	return true;
}
bool KPlayer2D::Render(ID3D11DeviceContext* pContext)
{
	KSprite* walk = g_SpriteManager.GetPtr(L"Player_Walk");
	//m_AnimIndex 배열로 저장된 좌표로 출력
	//if(m_AnimIndex<m_rtWalking.size())
	SetRectSource(walk->m_anim_array[walk->m_AnimIndex]);
	//anim 디버거
	if (ImGui::Begin(u8"player anim"))
	{
		ImGui::Text("animation %d, ", walk->m_AnimIndex);
	}
	ImGui::End();

	//플레이어 디버거
	if (ImGui::Begin(u8"플레이어 디버거"))
	{
		ImGui::Text("m_pos %d, %d", (int)m_pos.x, (int)m_pos.y);
		ImGui::Text("rt_coll %d, %d", (int)m_rtColl.max.x, (int)m_rtColl.max.y);
	}
	ImGui::End();
	KObject::Render(pContext);
	return true;
}

void KPlayer2D::ObjectOverlap(KCollider* pObj, DWORD dwState)
{
	//현재 오브젝트와 상대 오브젝트와 타입이 같을때
	if (dwState == pObj->m_CollisonType)
	{
		int kkk=0;
	}
}

void KPlayer2D::SelectOverlap(KCollider* pObj, DWORD dwState)
{
}


KPlayer2D::KPlayer2D()
{
	m_pContext = nullptr;
	m_Speed = 10.0f;
}

KPlayer2D::~KPlayer2D()
{
}
