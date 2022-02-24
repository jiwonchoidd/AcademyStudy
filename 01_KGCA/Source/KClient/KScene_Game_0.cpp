#include "KScene_Game_0.h"
#include "KSceneManager.h"
bool KScene_Game_0::Load(std::wstring file)
{
	m_BGM = g_SoundManager.LoadSound(L"../../data/sound/bgm/Twinleaf Town (Day).wav");
	m_BGM->SoundPlay(true);

	//
	m_PlayerObj.SetPosition(KVector2(300, 300));
	//맵과 띄워 놓는다.
	D3DKMatrixTranslation(&m_PlayerObj.m_matWorld, 0.0f, 0.0f, -0.1f);
	if (!m_PlayerObj.Init(m_pContext,
		L"../../data/shader/vs_2D.txt",
		L"../../data/shader/ps_2D.txt",
		L"../../data/texture/player_lucas.png",
		L"../../data/texture/player_lucas_mask.png"))
	{
		return false;
	}

	KMap* map = new KMap;
	map->SetRectSource({ 255,2,255,186 });
	map->SetRectDraw({ 0, 0, g_rtClient.right, g_rtClient.bottom });
	map->SetPosition(KVector2(g_rtClient.right / 2.0f, g_rtClient.bottom / 2.0f));
	if (!map->Init(m_pContext,
		L"../../data/shader/VS_2D_Map.txt", L"../../data/shader/PS_2D_Map.txt",
		L"../../data/texture/DS DSi - Pokemon Diamond Pearl - Players House.png", L""))
	{
		return false;
	}
	m_MapObj.insert(std::make_pair(0, map));
	return true;
}

bool KScene_Game_0::Init(ID3D11DeviceContext* context)
{
	KScene::Init(context);

	m_SceneID = S_GAME;
	m_Camera.Init();
	//
	m_Camera.CreateViewMatrix(KVector3(0, 0, -2), KVector3(0, 0, 0));
	m_Camera.CreateProjMatrix(1.0f, 1000.0f, XM_PI * 0.45f, (float)g_rtClient.right / (float)g_rtClient.bottom);

	//플레이어 생성
	//left,top,right,bottom
	
	
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


	return true;
}

bool KScene_Game_0::Frame()
{
	KScene::Frame();
	m_BGM->Frame();
	//플레이어 이동
	m_PlayerObj.SetMatrix(&m_PlayerObj.m_matWorld, &m_Camera.m_matView, &m_Camera.m_matProj);
	m_PlayerObj.Frame();
	m_Camera.Follow2DPos(&m_PlayerObj.m_matWorld);
	m_MapObj.find(0)->second->SetMatrix(nullptr, &m_Camera.m_matView, &m_Camera.m_matProj);
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
	if (g_InputData.bDownKey)
	{
		g_SceneManager.SetScene(1);
		g_InputData.bSpace = false;
		return true;
	}
	return true;
}

bool KScene_Game_0::Render()
{
	KScene::Render();
	//npc 렌더링
	for (int iObj = 0; iObj < m_NpcLlist.size(); iObj++)
	{
		m_NpcLlist[iObj]->SetMatrix(nullptr, &m_Camera.m_matView, &m_Camera.m_matProj);
		m_NpcLlist[iObj]->Render(m_pContext);
	}
	//플레이어 렌더링
	m_PlayerObj.Render(m_pContext);

	return true;
}

bool KScene_Game_0::Release()
{
	m_BGM->SoundStop();
	KScene::Release();
	m_PlayerObj.Release();
	for (int iObj = 0; iObj < m_NpcLlist.size(); iObj++)
	{
		m_NpcLlist[iObj]->Release();
	}
	m_Camera.Release();
	return true;
}
