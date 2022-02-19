#include "KSceneManager.h"


bool KSceneManager::Init(ID3D11DeviceContext* context)
{
	m_pContext = context;

	//ù��° ���� ��Ʈ�� �����Ѵ�. 
	m_pCurrentScene = new KScene_Intro;
	m_CurrentScene_Index = S_LOAD;
	m_pCurrentScene->Init(m_pContext);
	m_pCurrentScene->Load(L"test");
	return true;
}

KScene* KSceneManager::GetScene()
{
	return m_pCurrentScene;
}

bool KSceneManager::SetScene(BYTE index)
{
	//����� ���ٸ� �۾� ����
	if (m_CurrentScene_Index == index)return false;
	//�� ���� �ʱ�ȭ
	if (m_pCurrentScene != nullptr)
	{
		m_pCurrentScene->Release();
		delete m_pCurrentScene;
		m_pCurrentScene = nullptr;
	}
	//���� ����
	switch (index)
	{
	case S_LOAD:
	{
		m_pCurrentScene = new KScene_Intro;
		m_CurrentScene_Index = S_LOAD;

		break;
	}
	case S_MENU:
	{
		m_pCurrentScene = new KScene_Intro;
		m_CurrentScene_Index = S_LOAD;
		break;
	}
	case S_GAME:
	{
		m_pCurrentScene = new KScene_Game_0;
		m_CurrentScene_Index = S_GAME;
		break;
	}
	case S_RESULT:
	{
		m_pCurrentScene = new KScene_Intro;
		m_CurrentScene_Index = S_LOAD;
		break;
	}
	default:
		break;
	}

	m_bChangeScene = true;
	m_pCurrentScene->Init(m_pContext);
	m_pCurrentScene->Load(L"test");
	m_bChangeScene = false;
	return true;
}

bool KSceneManager::Frame()
{
	if(m_bChangeScene==false)
	m_pCurrentScene->Frame();
	return true;
}

bool KSceneManager::Render()
{
	if (m_bChangeScene == false)
	m_pCurrentScene->Render();
	return true;
}

bool KSceneManager::Release()
{
	m_pCurrentScene->Release();
	return true;
}

KSceneManager::KSceneManager()
{
	m_pCurrentScene = nullptr;
	m_bChangeScene = false;
	m_CurrentScene_Index = -1;
	m_pContext = nullptr;
}
