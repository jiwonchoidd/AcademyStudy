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

	//����Ǵ� ������ �ش� ������ �ƴ϶�� �ٲ��ش�.
	if (g_SceneManager.m_BGM->m_name != L"Twinleaf Town (Day)")
	{
		g_SceneManager.m_BGM->SoundStop();
		g_SceneManager.m_BGM = g_SoundManager.LoadSound(L"../../data/sound/bgm/Twinleaf Town (Day).wav");
		g_SceneManager.m_BGM->SoundPlay(true);
	}
	
	D3DKMatrixTranslation(&g_SceneManager.m_Player->m_matWorld, 10.0f, -12.0f, -1.0f);

	//�� �ε�---------------------------
	
	std::shared_ptr<KMapSpace> map_space = std::make_shared<KMapSpace>();

	
	for (int i = 0; i < 3; i++)
	{
		KMatrix s, r, t;
		std::shared_ptr<KObjObject> map3D_1 = std::make_shared<KObjObject>();
		D3DKMatrixIdentity(&s);
		D3DKMatrixIdentity(&r);
		D3DKMatrixIdentity(&t);
		D3DKMatrixScaling(&s, 2.0f, 2.0f, 1.5f);
		if (!map3D_1.get()->Init(m_pContext,
			L"../../data/shader/VS_0.txt",
			L"../../data/shader/PS_0.txt",
			L"../../data/model/map_01tex.png",
			L"../../data/model/map1.obj"))
		{
			return false;
		}
		if (i == 0)
		{
			D3DKMatrixTranslation(&t, 0.0f, 0.0f, 0.0f);
			D3DKMatrixRotationZ(&r, 0.0f);
		}
		if (i == 1)
		{
			D3DKMatrixTranslation(&t, 64.0f, 0.0f, 0.0f);
		}
		if (i == 2)
		{
			D3DKMatrixTranslation(&t, -64.0f, 0.0f, 0.0f);
		}
		map3D_1.get()->m_matWorld = map3D_1.get()->m_matWorld * s * t * r;
		m_MapObj.push_back(std::shared_ptr<KObject>(map3D_1));
	}

	map_space.get()->Init(m_pContext, KVector2(0, 0),
		32*2,32*2);

	map_space.get()->LoadLeafData(L"../../data/map/map_1.txt");

	
	for (int build = 0; build < 4; build++)
	{
		KMatrix s, r, t;
		std::shared_ptr<KObjObject> building = std::make_shared<KObjObject>();;
		if (!building.get()->Init(m_pContext,
			L"../../data/shader/VS_0.txt",
			L"../../data/shader/PS_0.txt",
			L"../../data/model/house1tex.jpg",
			L"../../data/model/house_01.obj"))
		{
			return false;
		}
		D3DKMatrixIdentity(&s);
		D3DKMatrixIdentity(&r);
		D3DKMatrixIdentity(&t);
		D3DKMatrixScaling(&s, 3.5f, 3.5f, 3.5f);
		D3DKMatrixRotationX(&r, -1 * (3.14 / 2));

		if (build == 0)
		{
			D3DKMatrixTranslation(&t, -11.0f, 17.0f, 0.0f);
		}
		if (build == 1)
		{
			D3DKMatrixTranslation(&t, 11.0f, 17.0f, 0.0f);
		}
		if (build == 2)
		{
			D3DKMatrixTranslation(&t, -11.0f, -7.0f, 0.0f);
		}
		if (build == 3)
		{
			D3DKMatrixTranslation(&t, 11.0f, -7.0f, 0.0f);
		}
		building.get()->m_matWorld = building.get()->m_matWorld * s * r *t;
		m_MapObj.push_back(std::shared_ptr<KObject>(building));
	}

	m_MapObj.push_back(std::shared_ptr<KObject>(map_space));
	
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
	m_Camera.CreateViewMatrix(KVector3(0, 0, -28), KVector3(0, 0, 0));
	m_Camera.CreateProjMatrix(1.0f, 1000.0f, XM_PI * 0.18f, (float)g_rtClient.right / (float)g_rtClient.bottom);
	return true;
}

bool KScene_Game_1::Frame()
{
	g_SceneManager.m_BGM->Frame();
	//�÷��̾� �̵�
	g_SceneManager.m_Player->Frame();
	//ī�޶� �̵�
	m_Camera.Follow2DPos(&g_SceneManager.m_Player->m_pos, {0, 20});
	//������ ���̵�
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
	//��
	for (int map = 0; map < m_MapObj.size(); map++)
	{
		m_MapObj[map]->SetMatrix(&m_MapObj[map]->m_matWorld,
			&m_Camera.m_matView, &m_Camera.m_matProj);
	}
	//�÷��̾� ������
		// Y�� ȸ������� _11, _13, _31, _33�� ��Ŀ� ȸ������ ����
		// ī�޶��� Y�� ȸ����İ��� �о ������� ����� X,Z���� ������
		// Y�� ȸ�� ������ ����� ����� �ִ�

	KScene::Render();
	g_SceneManager.m_Player->SetMatrix_Billboard(&g_SceneManager.m_Player->m_matWorld, 
		&m_Camera.m_matView, &m_Camera.m_matProj);
	g_SceneManager.m_Player->Render(m_pContext);

	return true;
}

bool KScene_Game_1::Release()
{
	m_Camera.Release();
	KScene::Release();
	return true;
}
