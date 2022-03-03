#include "KScene_Game_1.h"
#include "KSceneManager.h"
#include "KUI.h"
#include "ImGuiManager.h"
bool KScene_Game_1::Load(std::wstring file)
{
#pragma region ����� �� ����

	//�޴� ���ȭ��-------------------

	//�޴� ��ư-------------------

#pragma endregion
	//ĳ����

	m_BGM = g_SoundManager.LoadSound(L"../../data/sound/bgm/Twinleaf Town (Day).wav");
	m_BGM->SoundPlay(true);

	// ĳ���� �ε�
	m_PlayerObj.SetPosition(KVector2(0, 0));
	m_PlayerObj.SetRectDraw({ 0, 0, 4, 4 });
	//ĳ���Ϳ� �ʰ� ��� ���´�.
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

	//�� �ε�---------------------------
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
	//��ӵ� �� �ʱ�ȭ
	KScene::Init(context);
	//���� �� ������ Ÿ�� ����
	m_SceneID = S_GAME_1;

	//ī�޶� �ʱ�ȭ
	m_Camera.Init();
	m_Camera.CreateViewMatrix(KVector3(0, 0, -15), KVector3(0, 0, 0));
	m_Camera.CreateProjMatrix(1.0f, 1000.0f, XM_PI * 0.45f, (float)g_rtClient.right / (float)g_rtClient.bottom);
	return true;
}

bool KScene_Game_1::Frame()
{
	m_BGM->Frame();
	//�÷��̾� �̵�
	m_PlayerObj.Frame();
	//ī�޶� �̵�
	//m_Camera.Follow2DPos(&m_PlayerObj.m_pos);
	m_Camera.Frame();
	//������ ���̵�
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

	//��
	m_MapObj[0]->SetMatrix(&m_MapObj[0]->m_matWorld, &m_Camera.m_matView, &m_Camera.m_matProj);

	//�÷��̾� ������
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
