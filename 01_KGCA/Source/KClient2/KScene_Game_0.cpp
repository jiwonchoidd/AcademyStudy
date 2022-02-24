#include "KScene_Game_0.h"
#include "KSceneManager.h"
bool KScene_Game_0::Load(std::wstring file)
{
	m_BGM = g_SoundManager.LoadSound(L"../../data/sound/bgm/Twinleaf Town (Day).wav");
	m_BGM->SoundPlay(true);

	// ĳ���� �ε�
	m_PlayerObj.SetPosition(KVector2(0, 0));
	//ĳ���Ϳ� �ʰ� ��� ���´�.
	D3DKMatrixTranslation(&m_PlayerObj.m_matWorld, 0.0f, 0.0f, -0.1f);
	if (!m_PlayerObj.Init(m_pContext,
		L"../../data/shader/vs_2D.txt",
		L"../../data/shader/ps_2D.txt",
		L"../../data/texture/player_lucas.png",
		L"../../data/texture/player_lucas_mask.png"))
	{
		return false;
	}

	//NPC �ε�
	for (int inpc = 0; inpc < 2; inpc++)
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

	//�� �ε�
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
	//���� �� ������ Ÿ�� ����
	m_SceneID = S_GAME;
	
	//��ӵ� �� �ʱ�ȭ
	KScene::Init(context);

	m_Camera.Init();
	m_Camera.CreateViewMatrix(KVector3(0, 0, -2), KVector3(0, 0, 0));
	m_Camera.CreateProjMatrix(1.0f, 500.0f, XM_PI * 0.45f, (float)g_rtClient.right / (float)g_rtClient.bottom);
	return true;
}

bool KScene_Game_0::Frame()
{
	KScene::Frame();
	m_BGM->Frame();

	//�÷��̾� �̵�
	m_PlayerObj.Frame();

	//ī�޶� �̵�
	//m_Camera.Follow2DPos(&m_PlayerObj.m_pos);
	
	//npc �̵�
	for (int iObj = 0; iObj < m_NpcLlist.size(); iObj++)
	{
		RECT rt = m_NpcLlist[iObj]->m_rtDraw;
		rt.right = rt.right + (cos(g_fSecTimer) * 0.5f + 0.5f) * 50.0f;
		rt.bottom = rt.bottom + (cos(g_fSecTimer) * 0.5f + 0.5f) * 50.0f;
		m_NpcLlist[iObj]->UpdateRectDraw(rt);
		m_NpcLlist[iObj]->Frame();
	}

	//������ ���̵�
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
	m_MapObj.find(0)->second->SetMatrix(nullptr, &m_Camera.m_matView, &m_Camera.m_matProj);
	KScene::Render();
	//npc ������
	for (int iObj = 0; iObj < m_NpcLlist.size(); iObj++)
	{
		m_NpcLlist[iObj]->SetMatrix(nullptr, &m_Camera.m_matView, &m_Camera.m_matProj);
		m_NpcLlist[iObj]->Render(m_pContext);
	}
	//�÷��̾� ������
	m_PlayerObj.SetMatrix(nullptr, &m_Camera.m_matView, &m_Camera.m_matProj);
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
