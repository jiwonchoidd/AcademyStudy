#include "KPlayer2D.h"
#include "ImGuiManager.h"
bool KPlayer2D::Frame()
{
	if (g_InputData.bWKey)
	{
		//SetRectSource(m_rtWalking[6]);
		KVector2 pos;
		m_dir.y = 1;
		pos.y -= m_speed * g_fSecPerFrame;
		AddPosition(pos, m_pContext);
		
		RectSequence(3.0f, 8, 11);
	}
	else if (g_InputData.bSKey)
	{
		//SetRectSource(m_rtWalking[0]); //소스 
		KVector2 pos;
		pos.y += m_speed * g_fSecPerFrame;
		m_dir.y = -1;
		AddPosition(pos, m_pContext);
		RectSequence(3.0f, 0, 3);
	}
	else if (g_InputData.bAKey)
	{
		//etRectSource(m_rtWalking[9]); //소스 
		KVector2 pos;
		pos.x -= m_speed * g_fSecPerFrame;
		m_dir.x = -1;
		AddPosition(pos, m_pContext);
		RectSequence(3.0f, 12, 15);
	}
	else if (g_InputData.bDKey)
	{
		//SetRectSource(m_rtWalking[3]); //소스 
		KVector2 pos;
		pos.x += m_speed * g_fSecPerFrame;
		m_dir.x = 1;
		AddPosition(pos, m_pContext);
		RectSequence(3.0f, 4, 7);
	}
	return true;
}

bool KPlayer2D::Render(ID3D11DeviceContext* pContext)
{
	//m_AnimIndex 배열로 저장된 좌표로 출력
	if(m_AnimIndex<m_rtWalking.size())
	SetRectSource(m_rtWalking[m_AnimIndex]);
	
	if (ImGui::Begin(u8"애니메이션 디버거"))
	{
		ImGui::Text("m_AnimIndex %d", m_AnimIndex);
		ImGui::Text("m_SequenceTimer %d", m_SequenceTimer);
		ImGui::Text("g_persec %d", g_fSecPerFrame);
	}
	ImGui::End();
	
	KObject::Render(pContext);
	return true;
}

void KPlayer2D::HitOverlap(KCollider* pObj, DWORD dwState)
{
	if (dwState == KCollisionType::Overlap)
	{
		int kkk = 0;
	}
}

void KPlayer2D::SelectOverlap(KCollider* pObj, DWORD dwState)
{
}

bool KPlayer2D::Init(ID3D11DeviceContext* context, std::wstring vs, std::wstring ps, std::wstring tex, std::wstring mask)
{
	m_speed = 2.0f;
	m_pContext = context;
	//걷는 애니메이션 한줄 잘라내기
	for (int resource = 0; resource < 12; resource++)
	{
		RECT rc = { 15 + (resource * 35),30,35,35 };
		m_rtWalking.push_back(rc);
		//홀수면
		if (resource == 1 || resource == 4 || resource == 7 || resource == 10 || resource == 13)
		{
			RECT rc = { 15 + ((resource-1) * 35),30,35,35 };
			m_rtWalking.push_back(rc);
		}
	}
	//
	SetRectSource(m_rtWalking[8]); //소스 
	SetRectDraw({ 0, 0, 182, 98});
	
	K2DAsset::CreateObject_Mask(vs, ps, tex, mask);
	return true;
}

KPlayer2D::KPlayer2D()
{
	m_pContext = nullptr;
	m_speed = 2.0f;
	m_rtWalking.clear();
}

KPlayer2D::~KPlayer2D()
{
	m_rtWalking.clear();
}
