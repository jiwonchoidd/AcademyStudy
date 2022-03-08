#include "KScene_Game_0.h"
#include "KSceneManager.h"
#include "KUI.h"
#include "ImGuiManager.h"
bool KScene_Game_0::Load(std::wstring file)
{
	#pragma region 사용할 모델 생성

		//메뉴 배경화면-------------------
		std::shared_ptr<KImage> menu_background(new KImage);
		menu_background->m_Name = L"menu_background";
		menu_background->SetRectDraw({ 0, 0, g_rtClient.right / 3, g_rtClient.bottom / 2 });
		menu_background->SetPosition(KVector2(g_rtClient.right / 1.2f, g_rtClient.bottom / 4));
		menu_background->SetCollisionType(KCollisionType::Ignore, KSelectType::Select_Ignore);
		menu_background->m_rtOffset = { 50, 50, 50, 50 };
		if (!menu_background->Init(m_pContext,
			L"../../data/shader/VS_UI_0.txt",
			L"../../data/shader/PS_UI_0.txt",
			L"../../data/texture/menu_background.png",
			L""))
		{
			return false;
		}
		g_UIModelManager.m_list.insert(std::make_pair(L"menu_background", menu_background));

		//전체 페이드 아웃 이미지-------------------
		std::shared_ptr<KImage> fade_background(new KImage);
		fade_background->m_Name = L"fade_background";
		fade_background->SetRectDraw({ 0, 0, g_rtClient.right, g_rtClient.bottom});
		fade_background->SetPosition(KVector2(g_rtClient.right / 2.0f, g_rtClient.bottom / 2.0f));
		fade_background->SetCollisionType(KCollisionType::Ignore, KSelectType::Select_Ignore);
		fade_background->m_rtOffset = { 0, 0, 0, 0 };
		if (!fade_background->Init(m_pContext,
			L"../../data/shader/VSPS_UI_Alpha.txt",
			L"../../data/shader/VSPS_UI_Alpha.txt",
			L"",
			L""))
		{
			return false;
		}
		fade_background->m_fAlpha = 1.0f;
		fade_background->m_bFadeOut = true;

		g_UIModelManager.m_list.insert(std::make_pair(L"fade_background", fade_background));

		//메뉴 버튼-------------------
		std::shared_ptr<KButton> btn(new KButton);
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

		if (!btn->Init(m_pContext, L"../../data/shader/VS_UI_0.txt",
			L"../../data/shader/PS_UI_0.txt",
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

		KUIModel* fadeimg = g_UIModelManager.GetPtr(L"fade_background")->Clone();
		m_UIObj.push_back(std::shared_ptr<KObject>(fadeimg));
		m_UIObj.push_back(std::shared_ptr<KObject>(background));
		m_UIObj.push_back(std::shared_ptr<KObject>(button));

	#pragma endregion

	//사운드 로드
		if (g_SceneManager.m_BGM->m_name != L"Twinleaf Town (Day)")
		{
			g_SceneManager.m_BGM->SoundStop();
			g_SceneManager.m_BGM = g_SoundManager.LoadSound(L"../../data/sound/bgm/Twinleaf Town (Day).wav");
			g_SceneManager.m_BGM->SoundPlay(true);
		}
	// 캐릭터 로드

	D3DKMatrixTranslation(&g_SceneManager.m_Player->m_matWorld, -4, 2, -0.1f);
		

	//맵 로드---------------------------
	std::shared_ptr<KImage> map_img = std::make_shared<KImage>();
	std::shared_ptr<KMapSpace> map_space = std::make_shared<KMapSpace>();
	map_img->SetRectSource({ 255,2,255,186 });
	map_img->SetRectDraw({ 0, 0, 32, 28 });
	map_img->SetPosition(KVector2(0, 0));
	if (!map_img->Init(
		L"../../data/shader/VS_2D_Map.txt", L"../../data/shader/PS_2D_Map.txt",
		L"../../data/texture/DS DSi - Pokemon Diamond Pearl - Players House.png", L""))
	{
		return false;
	}
	map->m_Space.LoadLeafData(L"../../data/map/map_0.txt");
	m_MapObj.push_back(std::shared_ptr<KObject>(map));

	return true;
}

bool KScene_Game_0::Init(ID3D11DeviceContext* context)
{
	//상속된 씬 초기화
	KScene::Init(context);
	//현재 씬 열거형 타입 지정
	m_SceneID = S_GAME_0;
	
	//카메라 초기화
	m_Camera.Init();
	m_Camera.CreateViewMatrix(KVector3(0, 0, -40), KVector3(0, 0, 0));
	m_Camera.CreateProjMatrix(1.0f, 1000.0f, XM_PI * 0.18f, (float)g_rtClient.right / (float)g_rtClient.bottom);
	//m_Camera.CreateProjMatrix(1.0f, 1000.0f, XM_PI * 0.3f, (float)g_rtClient.right / (float)g_rtClient.bottom);
	return true;
}

bool KScene_Game_0::Frame()
{
	g_SceneManager.m_BGM->Frame();
	//플레이어 이동
	g_SceneManager.m_Player->Frame();
	//카메라 이동
	m_Camera.Follow2DPos(&g_SceneManager.m_Player->m_pos);

	//디버깅용 씬이동
	if (g_SceneManager.m_Player->m_blockstate==-1)
	{
		g_SceneManager.m_Player->m_bMove = false;
		g_SceneManager.m_Timer += g_fSecPerFrame;
		if (g_SceneManager.m_Timer > 1.0f)
		{
			g_SceneManager.m_Timer = 0.0f;
			g_SceneManager.m_Player->m_bMove = true;
			g_SceneManager.SetScene(2);
			return true;
		}
	}
	KScene::Frame();
	return true;
}

bool KScene_Game_0::Render()
{
	//맵
	m_MapObj[0]->SetMatrix(&m_MapObj[0]->m_matWorld, &m_Camera.m_matView, &m_Camera.m_matProj);

	//플레이어 렌더링
	g_SceneManager.m_Player->SetMatrix(&g_SceneManager.m_Player->m_matWorld, &m_Camera.m_matView, &m_Camera.m_matProj);
	g_SceneManager.m_Player->Render(m_pContext);

	KScene::Render();
	return true;
}

bool KScene_Game_0::Release()
{
	//m_BGM->SoundStop();
	m_Camera.Release();
	KScene::Release();
	return true;
}
