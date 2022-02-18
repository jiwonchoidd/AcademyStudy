#include "KScene_Game_0.h"

bool KScene_Game_0::Load(std::wstring file)
{
	m_BGM = g_Sound.LoadSound(L"../../data/sound/bgm/Twinleaf Town (Day).wav");
	//m_BGM->SoundPlay(true);
	return true;
}

bool KScene_Game_0::Init(ID3D11Device* device, ID3D11DeviceContext* context)
{
	m_SceneID = S_GAME;
	m_pd3dDevice = device;
	m_pContext = context;
	m_Camera.Init();
	m_Skybox.Init(L"../../data/shader/Skybox.txt",
		L"../../data/texture/Skybox_dd.dds");
	
	

	m_Camera.CreateViewMatrix(KVector3(0, 0, -3), KVector3(0, 0, 0));
	m_Camera.CreateProjMatrix(1.0f, 1000.0f, XM_PI * 0.45f, (float)g_rtClient.right / (float)g_rtClient.bottom);

	//플레이어 생성
	m_PlayerObj.SetRectSource({ 91,1,42,56 }); //소스 
	m_PlayerObj.SetRectDraw({ 0,0,42 * 2,56 * 2 });
	m_PlayerObj.SetPosition(KVector2(400, 300));

	if (!m_PlayerObj.Init(m_pContext,
		L"../../data/shader/vs_2D.txt",
		L"../../data/shader/ps_2D.txt",
		L"../../data/texture/bitmap1.bmp",
		L"../../data/texture/bitmap2.bmp"))
	{
		return false;
	}
	//NPC 생성
	for (int inpc = 0; inpc < 5; inpc++)
	{
		KNpc2D* npc = new KNpc2D;
		if (inpc % 2 == 0)
		{
			npc->SetRectSource({ 46,63,69,79 });
			npc->SetRectDraw({ 0,0, 69,79 });
		}
		else
		{
			npc->SetRectSource({ 1,63,42,76 });
			npc->SetRectDraw({ 0,0, 42,76 });
		}
		npc->SetPosition(KVector2(50 + inpc * 150, 50));
		if (!npc->Init(m_pContext,
			L"../../data/shader/vs_2D.txt",
			L"../../data/shader/ps_2D.txt",
			L"../../data/texture/bitmap1.bmp",
			L"../../data/texture/bitmap2.bmp"))
		{
			return false;
		}
		m_NpcLlist.push_back(npc);
	}

	m_Box.Init(L"../../data/shader/vs_0.txt", L"../../data/shader/ps_0.txt",
		L"../../data/texture/uv.bmp");
	return true;
}

bool KScene_Game_0::Frame()
{
	//플레이어 이동
	m_PlayerObj.Frame();

	//npc 이동
	for (int iObj = 0; iObj < m_NpcLlist.size(); iObj++)
	{
		RECT rt = m_NpcLlist[iObj]->m_rtDraw;
		rt.right = rt.right + (cos(g_fSecTimer) * 0.5f + 0.5f) * 50.0f;
		rt.bottom = rt.bottom + (cos(g_fSecTimer) * 0.5f + 0.5f) * 50.0f;
		m_NpcLlist[iObj]->UpdateRectDraw(rt);
		m_NpcLlist[iObj]->Frame();
	}
	m_Camera.Frame();
	m_Skybox.Frame();
	m_Box.Frame();
	return true;
}

bool KScene_Game_0::Render()
{
	//환경 텍스쳐
	ApplyRS(m_pContext, KState::g_pRSBackface);
	m_Skybox.SetMatrix(&m_Camera.m_matWorld, &m_Camera.m_matView, &m_Camera.m_matProj);
	m_Skybox.Render(m_pContext);
	ApplyRS(m_pContext, KState::g_pRSSolid);

	m_Box.SetMatrix(nullptr, &m_Camera.m_matView, &m_Camera.m_matProj);
	m_Box.Render(m_pContext);

	//npc 렌더링
	for (int iObj = 0; iObj < m_NpcLlist.size(); iObj++)
	{
		m_NpcLlist[iObj]->Render(m_pContext);
	}
	//플레이어 렌더링
	m_PlayerObj.Render(m_pContext);


	return true;
}

bool KScene_Game_0::Release()
{
	m_PlayerObj.Release();
	for (int iObj = 0; iObj < m_NpcLlist.size(); iObj++)
	{
		m_NpcLlist[iObj]->Release();
	}
	m_Box.Release();
	m_Plane.Release();
	m_Camera.Release();
	m_Skybox.Release();
	return true;
}
