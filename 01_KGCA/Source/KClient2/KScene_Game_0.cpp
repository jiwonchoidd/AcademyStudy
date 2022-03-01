#include "KScene_Game_0.h"
#include "KSceneManager.h"
#include "KUI.h"
#include "ImGuiManager.h"
bool KScene_Game_0::Load(std::wstring file)
{
	#pragma region 사용할 모델 생성

		//메뉴 배경화면-------------------
		KImage* menu_background = new KImage;
		menu_background->m_Name = L"menu_background";
		menu_background->SetRectDraw({ 0, 0, g_rtClient.right / 3, g_rtClient.bottom / 2 });
		menu_background->SetPosition(KVector2(g_rtClient.right / 1.2f, g_rtClient.bottom / 4));
		menu_background->SetCollisionType(KCollisionType::Ignore, KSelectType::Select_Ignore);
		menu_background->m_rtOffset = { 50, 50, 50, 50 };
		if (!menu_background->Init(m_pContext,
			L"../../data/shader/VSPS_UI_0.txt",
			L"../../data/shader/VSPS_UI_0.txt",
			L"../../data/texture/menu_background.png",
			L""))
		{
			return false;
		}
		g_UIModelManager.m_list.insert(std::make_pair(L"menu_background", menu_background));

		//메뉴 버튼-------------------
		KButton* btn = new KButton;
		btn->m_Name = L"menu_button";
		btn->m_rtOffset = { 50, 50, 50, 50 };
		btn->SetRectDraw({ 0, 0, g_rtClient.right / 3, (g_rtClient.bottom / 2) / 5 });
		btn->SetPosition(KVector2(g_rtClient.right / 1.2f, (g_rtClient.bottom / 20)));

		KTexture* pTex = g_TextureMananger.Load(L"../../data/texture/blank.png");
		KSound* pSound = g_SoundManager.LoadSound(L"../../data/sound/menu_open.mp3");
		btn->m_datalist.emplace_back(pTex, pSound);

		pTex = g_TextureMananger.Load(L"../../data/texture/menu_hover.png");
		pSound = g_SoundManager.LoadSound(L"../../data/sound/menu_hover.mp3");
		btn->m_datalist.emplace_back(pTex, pSound);

		pTex = g_TextureMananger.Load(L"../../data/texture/menu_hover.png");
		pSound = g_SoundManager.LoadSound(L"../../data/sound/menu_select.mp3");
		btn->m_datalist.emplace_back(pTex, pSound);

		if (!btn->Init(m_pContext, L"../../data/shader/VSPS_UI_0.txt",
			L"../../data/shader/VSPS_UI_0.txt",
			L"../../data/texture/blank.png", L""))
		{
			return false;
		}
		g_UIModelManager.m_list.insert(std::make_pair(L"menu_button", btn));

		//-----------------------
		KUIModel* background = g_UIModelManager.GetPtr(L"menu_background")->Clone();
		background->m_Name = L"menu_background_1";
		background->UpdateData();

		KUIModel* button = g_UIModelManager.GetPtr(L"menu_button")->Clone();
		button->m_Name = L"menu_button_1";
		button->UpdateData();

		m_UIObj.push_back(background);
		m_UIObj.push_back(button);

	#pragma endregion

	m_BGM = g_SoundManager.LoadSound(L"../../data/sound/bgm/Twinleaf Town (Day).wav");
	m_BGM->SoundPlay(true);

	// 캐릭터 로드
	m_PlayerObj.SetPosition(KVector2(0, 0));
	m_PlayerObj.SetRectDraw({ 0, 0, 900, 500});
	//캐릭터와 맵과 띄워 놓는다.
	D3DKMatrixTranslation(&m_PlayerObj.m_matWorld, 0.0f, 0.0f, -0.1f);
	if (!m_PlayerObj.Init(m_pContext,
		L"../../data/shader/vs_2D.txt",
		L"../../data/shader/ps_2D.txt",
		L"../../data/texture/player_lucas.png",
		L"../../data/texture/player_lucas_mask.png"))
	{
		return false;
	}

	//NPC 로드-------------------------
	/*for (int inpc = 0; inpc < 2; inpc++)
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
	}*/

	//맵 로드---------------------------
	KMap* map = new KMap;
	map->SetRectSource({ 255,2,255,186 });
	map->SetRectDraw({ 0, 0, g_rtClient.right, g_rtClient.bottom });
	map->SetPosition(KVector2(0, 0));
	if (!map->Init(m_pContext,
		L"../../data/shader/VS_2D_Map.txt", L"../../data/shader/PS_2D_Map.txt",
		L"../../data/texture/DS DSi - Pokemon Diamond Pearl - Players House.png", L""))
	{
		return false;
	}
	m_MapObj.push_back(map);

	test = new KObjObject;
	test->SetPosition(KVector2(0, 0));
	test->SetRectDraw({ 0,0, 42,76 });
	test->Init(m_pContext, L"../../data/shader/VS_0.txt", L"../../data/shader/PS_0.txt",
		L"../../data/texture/UV.bmp", L"../../data/model/test.obj");

	return true;
}

bool KScene_Game_0::Init(ID3D11DeviceContext* context)
{
	//상속된 씬 초기화
	KScene::Init(context);
	//현재 씬 열거형 타입 지정
	m_SceneID = S_GAME;

	//카메라 초기화
	m_Camera.Init();
	m_Camera.CreateViewMatrix(KVector3(0, 0, -10), KVector3(0, 0, 0));
	m_Camera.CreateProjMatrix(1.0f, 1000.0f, XM_PI * 0.45f, (float)g_rtClient.right / (float)g_rtClient.bottom);
	return true;
}

bool KScene_Game_0::Frame()
{
	m_BGM->Frame();
	test->Frame();
	//플레이어 이동
	m_PlayerObj.Frame();
	//카메라 이동
	m_Camera.Follow2DPos(&m_PlayerObj.m_pos);
	//npc 이동
	/*for (int iObj = 0; iObj < m_NpcLlist.size(); iObj++)
	{
		RECT rt = m_NpcLlist[iObj]->m_rtDraw;
		rt.right = rt.right + (cos(g_fSecTimer) * 0.5f + 0.5f) * 50.0f;
		rt.bottom = rt.bottom + (cos(g_fSecTimer) * 0.5f + 0.5f) * 50.0f;
		m_NpcLlist[iObj]->UpdateRectDraw(rt);
		m_NpcLlist[iObj]->Frame();
	}*/

	//디버깅용 씬이동
	if (g_InputData.bDownKey)
	{
		g_SceneManager.SetScene(1);
		g_InputData.bSpace = false;
		return true;
	}
	KScene::Frame();
	return true;
}

bool KScene_Game_0::Render()
{
#ifdef DEBUG
	if (ImGui::Begin("map debugger"))
	{
		ImGui::Text("map %d %d", m_MapObj[0]->m_matWorld._41, m_MapObj[0]->m_matWorld._42);
	}
	ImGui::End();
#endif // DEBUG

	//맵
	m_MapObj[0]->SetMatrix(&m_MapObj[0]->m_matWorld, &m_Camera.m_matView, &m_Camera.m_matProj);

	//npc 렌더링
	/*for (int iObj = 0; iObj < m_NpcLlist.size(); iObj++)
	{
		m_NpcLlist[iObj]->SetMatrix(nullptr, &m_Camera.m_matView, &m_Camera.m_matProj);
		m_NpcLlist[iObj]->Render(m_pContext);
	}*/
	//플레이어 렌더링
	m_PlayerObj.SetMatrix(&m_PlayerObj.m_matWorld, &m_Camera.m_matView, &m_Camera.m_matProj);
	m_PlayerObj.Render(m_pContext);

	test->SetMatrix(&test->m_matWorld, &m_Camera.m_matView, &m_Camera.m_matProj);
	test->Render(m_pContext);

	KScene::Render();
	return true;
}

bool KScene_Game_0::Release()
{
	m_BGM->SoundStop();
	m_PlayerObj.Release();
	/*for (int iObj = 0; iObj < m_NpcLlist.size(); iObj++)
	{
		m_NpcLlist[iObj]->Release();
	}*/
	m_Camera.Release();
	KScene::Release();
	return true;
}
