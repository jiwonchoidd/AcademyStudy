#include "KScene_Game_0.h"
#include "KSceneManager.h"
#include "KUI.h"
#include "ImGuiManager.h"
bool KScene_Game_0::Load(std::wstring file)
{
	#pragma region ����� �� ����

		//�޴� ���ȭ��-------------------
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

		//��ü ���̵� �ƿ� �̹���-------------------
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

		//�޴� ��ư-------------------
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

	m_BGM = g_SoundManager.LoadSound(L"../../data/sound/bgm/Twinleaf Town (Day).wav");
	m_BGM->SoundPlay(true);

	// ĳ���� �ε�
	m_PlayerObj.SetRectDraw({ 0, 0, 3, 4});
	//ĳ���Ϳ� �ʰ� ��� ���´�.
	if (!m_PlayerObj.Init(m_pContext,
		L"../../data/shader/VS_2D.txt",
		L"../../data/shader/PS_2D.txt",
		L"../../data/texture/player_lucas.png",
		L"../../data/texture/player_lucas_mask.png"))
	{
		return false;
	}
	D3DKMatrixTranslation(&m_PlayerObj.m_matWorld,-4, 2, -0.1f);
	m_PlayerObj.m_CollisonType = KCollisionType::Overlap;

	//�� �ε�---------------------------
	KMap* map = new KMap;
	map->SetRectSource({ 255,2,255,186 });
	map->SetRectDraw({ 0, 0, 32, 28});
	map->SetPosition(KVector2(0, 0));
	if (!map->Init(m_pContext,
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
	//��ӵ� �� �ʱ�ȭ
	KScene::Init(context);
	//���� �� ������ Ÿ�� ����
	m_SceneID = S_GAME_0;
	
	//ī�޶� �ʱ�ȭ
	m_Camera.Init();
	m_Camera.CreateViewMatrix(KVector3(0, 0, -40), KVector3(0, 0, 0));
	m_Camera.CreateProjMatrix(1.0f, 1000.0f, XM_PI * 0.18f, (float)g_rtClient.right / (float)g_rtClient.bottom);
	//m_Camera.CreateProjMatrix(1.0f, 1000.0f, XM_PI * 0.3f, (float)g_rtClient.right / (float)g_rtClient.bottom);
	return true;
}

bool KScene_Game_0::Frame()
{
	m_BGM->Frame();
	//�÷��̾� �̵�
	m_PlayerObj.Frame();
	//ī�޶� �̵�
	m_Camera.Follow2DPos(&m_PlayerObj.m_pos);

	//������ ���̵�
	if (m_PlayerObj.m_blockstate==-1)
	{
		m_PlayerObj.m_bMove = false;
		m_Timer += g_fSecPerFrame;
		if (m_Timer > 1.0f)
		{
			g_SceneManager.SetScene(2);
			m_Timer = 0.0f;
			return true;
		}
	}
	KScene::Frame();
	return true;
}

bool KScene_Game_0::Render()
{
	//��
	m_MapObj[0]->SetMatrix(&m_MapObj[0]->m_matWorld, &m_Camera.m_matView, &m_Camera.m_matProj);

	//�÷��̾� ������
	m_PlayerObj.SetMatrix(&m_PlayerObj.m_matWorld, &m_Camera.m_matView, &m_Camera.m_matProj);
	m_PlayerObj.Render(m_pContext);

	KScene::Render();
	return true;
}

bool KScene_Game_0::Release()
{
	m_BGM->SoundStop();
	m_PlayerObj.Release();
	m_Camera.Release();
	KScene::Release();
	return true;
}
