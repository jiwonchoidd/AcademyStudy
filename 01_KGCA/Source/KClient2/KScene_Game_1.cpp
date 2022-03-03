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
	//캐릭터

	m_BGM = g_SoundManager.LoadSound(L"../../data/sound/bgm/Twinleaf Town (Day).wav");
	m_BGM->SoundPlay(true);

	// 캐릭터 로드
	m_PlayerObj.SetPosition(KVector2(0, 0));
	m_PlayerObj.SetRectDraw({ 0, 0, 4, 4 });
	//캐릭터와 맵과 띄워 놓는다.
	if (!m_PlayerObj.Init(m_pContext,
		L"../../data/shader/vs_2D.txt",
		L"../../data/shader/ps_2D.txt",
		L"../../data/texture/player_lucas.png",
		L"../../data/texture/player_lucas_mask.png"))
	{
		return false;
	}
	D3DKMatrixTranslation(&m_PlayerObj.m_matWorld, 0.0f, 0.0f, -0.1f);
	m_PlayerObj.m_CollisonType = KCollisionType::Overlap;

	//맵 로드---------------------------
	KObjObject* m_Map = new KObjObject;
	if (!m_Map->Init(m_pContext,
		L"../../data/shader/VS_0.txt",
		L"../../data/shader/PS_0.txt",
		L"../../data/model/map_01tex.png",
		L"../../data/model/map1.obj"))
	{
		return false;
	}
	D3DKMatrixScaling(&m_Map->m_matWorld, 2.0f, 2.0f, 1.0f);
	
	m_MapObj.push_back(m_Map);

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
	m_Camera.CreateViewMatrix(KVector3(0, 0, -15), KVector3(0, 0, 0));
	m_Camera.CreateProjMatrix(1.0f, 1000.0f, XM_PI * 0.45f, (float)g_rtClient.right / (float)g_rtClient.bottom);
	return true;
}

bool KScene_Game_1::Frame()
{
	m_BGM->Frame();
	//플레이어 이동
	m_PlayerObj.Frame();
	//카메라 이동
	//m_Camera.Follow2DPos(&m_PlayerObj.m_pos);
	m_Camera.Frame();
	//디버깅용 씬이동
	if (g_InputData.bDownKey)
	{
		//g_SceneManager.SetScene(1);
		g_InputData.bSpace = false;
		return true;
	}
	KScene::Frame();
	return true;
}

bool KScene_Game_1::Render()
{

	//맵
	m_MapObj[0]->SetMatrix(&m_MapObj[0]->m_matWorld, &m_Camera.m_matView, &m_Camera.m_matProj);

	//플레이어 렌더링
	m_PlayerObj.SetMatrix(&m_PlayerObj.m_matWorld, &m_Camera.m_matView, &m_Camera.m_matProj);
	m_PlayerObj.Render(m_pContext);

	KScene::Render();
	return true;
}

bool KScene_Game_1::Release()
{
	m_BGM->SoundStop();
	m_PlayerObj.Release();
	m_Camera.Release();
	KScene::Release();
	return true;
}
