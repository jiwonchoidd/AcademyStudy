#include "KScene_Game_1.h"
#include "KSceneManager.h"
#include "KUI.h"
#include "ImGuiManager.h"
bool KScene_Game_1::Load(std::wstring file)
{
#pragma region 사용할 모델 생성

	//메뉴 배경화면-------------------

	//메뉴 버튼-------------------

#pragma endregion
	//진행되는 음원이 해당 음원이 아니라면 바꿔준다.
	if (g_SceneManager.m_BGM->m_name != L"Twinleaf Town (Day)")
	{
		g_SceneManager.m_BGM->SoundStop();
		g_SceneManager.m_BGM = g_SoundManager.LoadSound(L"../../data/sound/bgm/Twinleaf Town (Day).wav");
		g_SceneManager.m_BGM->SoundPlay(true);
	}
	

	D3DKMatrixTranslation(&g_SceneManager.m_Player->m_matWorld, 10.0f, -12.0f, -1.0f);

	//맵 로드---------------------------
	KMapSpace* map = new KMapSpace;
	map->SetRectSource({ 255,2,255,186 });
	map->SetRectDraw({ 0, 0, 32, 28 });
	map->SetPosition(KVector2(0, 0));
	if (!map->Init(m_pContext,
		L"../../data/shader/VS_2D_Map.txt", L"../../data/shader/PS_2D_Map.txt",
		L"../../data/texture/DS DSi - Pokemon Diamond Pearl - Players House.png", L""))
	{
		return false;
	}
	map->m_Space.LoadLeafData(L"../../data/map/map_0.txt");
	m_MapObj.push_back(std::shared_ptr<KObject>(map));
	
	
	std::shared_ptr<KObjObject> map3D = std::make_shared<KObjObject>();
	if (!map3D->Init(m_pContext,
		L"../../data/shader/VS_0.txt",
		L"../../data/shader/PS_0.txt",
		L"../../data/model/map_01tex.png",
		L"../../data/model/map1.obj"))
	{
		return false;
	}
	D3DKMatrixScaling(&map3D->m_matWorld, 2.0f, 2.0f, 1.5f);

	std::shared_ptr<KObjObject> building = std::make_shared<KObjObject>();;
	if (!building->Init(m_pContext,
		L"../../data/shader/VS_0.txt",
		L"../../data/shader/PS_0.txt",
		L"../../data/model/house1tex.jpg",
		L"../../data/model/house_01.obj"))
	{
		return false;
	}

	D3DKMatrixTranslation(&building->m_matWorld, 10.0f, -10.0f, 1.0f);
	D3DKMatrixRotationX(&building->m_matWorld, -1 * (3.14 / 2));
	m_MapObj.push_back(std::shared_ptr<KObject>(map3D));
	m_MapObj.push_back(std::shared_ptr<KObject>(building));

	return true;
}

bool KScene_Game_1::Init(ID3D11DeviceContext* context)
{
	//상속된 씬 초기화
	KScene::Init(context);
	//현재 씬 열거형 타입 지정
	m_SceneID = S_GAME_1;

	//카메라 초기화
	m_Camera.Init();
	m_Camera.CreateViewMatrix(KVector3(0, 0, -35), KVector3(0, 0, 0));
	m_Camera.CreateProjMatrix(1.0f, 1000.0f, XM_PI * 0.18f, (float)g_rtClient.right / (float)g_rtClient.bottom);
	return true;
}

bool KScene_Game_1::Frame()
{
	g_SceneManager.m_BGM->Frame();
	//플레이어 이동
	g_SceneManager.m_Player->Frame();
	//카메라 이동
	m_Camera.Follow2DPos(&g_SceneManager.m_Player->m_pos, {0, 20});
	//디버깅용 씬이동
	if (g_InputData.bAKey)
	{
		g_SceneManager.m_Timer += g_fSecPerFrame;
		if (g_SceneManager.m_Timer > 1.0f)
		{
			g_SceneManager.m_BGM->SoundStop();
			g_SceneManager.m_BGM = g_SoundManager.LoadSound(L"../../data/sound/bgm/Battle! (Wild Pokemon).wav");
			g_SceneManager.m_BGM->SoundPlay(true);
			g_SceneManager.m_Timer = 0.0f;
			g_SceneManager.SetScene(3);
			return true;
		}
	}
	KScene::Frame();
	return true;
}

bool KScene_Game_1::Render()
{
	//맵
	m_MapObj[0]->SetMatrix(&m_MapObj[0]->m_matWorld, &m_Camera.m_matView, &m_Camera.m_matProj);
	m_MapObj[1]->SetMatrix(&m_MapObj[1]->m_matWorld, &m_Camera.m_matView, &m_Camera.m_matProj);

	//플레이어 렌더링
		// Y축 회전행렬은 _11, _13, _31, _33번 행렬에 회전값이 들어간다
		// 카메라의 Y축 회전행렬값을 읽어서 역행렬을 만들면 X,Z축이 고정된
		// Y축 회전 빌보드 행렬을 만들수 있다
	

	g_SceneManager.m_Player->SetMatrix_Billboard(&g_SceneManager.m_Player->m_matWorld, 
		&m_Camera.m_matView, &m_Camera.m_matProj);
	g_SceneManager.m_Player->Render(m_pContext);

	KScene::Render();
	return true;
}

bool KScene_Game_1::Release()
{
	m_Camera.Release();
	KScene::Release();
	return true;
}
