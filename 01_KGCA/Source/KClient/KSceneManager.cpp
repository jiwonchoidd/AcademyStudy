#include "KSceneManager.h"

bool KSceneManager::Init(ID3D11DeviceContext* context)
{
	m_pContext = context;

	//첫번째 씬은 인트로 설정한다. 
	m_pCurrentScene = new KScene_Intro;
	m_CurrentScene_Index = S_MENU;
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
	if (m_Timer > 3.0f && m_bChangeScene)
	{
		//씬 해제 초기화
		if (m_pCurrentScene != nullptr)
		{
			m_pCurrentScene->Release();
			delete m_pCurrentScene;
			m_pCurrentScene = nullptr;
		}
		//상태 패턴
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
			m_CurrentScene_Index = S_MENU;
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

		m_pCurrentScene->Init(m_pContext);
		m_pCurrentScene->Load(L"test");
		m_bChangeScene = false;
		m_Timer = 0.0f;
	}
	
	return true;
}

bool KSceneManager::Frame()
{
	m_pCurrentScene->Frame();

	//계속해서 시간을 추가함.
	//배경을 바꾸면
	if (m_Timer < 5.0f)
	{
		m_Timer += g_fSecTimer;
		m_bChangeScene = false;
	}
	else
	{
		m_bChangeScene = true;
	}

	return true;
}

bool KSceneManager::Render()
{
	m_pCurrentScene->Render();
	return true;
}

bool KSceneManager::Release()
{
	m_pCurrentScene->Release();
	delete m_pCurrentScene;
	return true;
}

KSceneManager::KSceneManager()
{
	m_pCurrentScene = nullptr;
	m_bChangeScene = false;
	m_CurrentScene_Index = -1;
	m_Timer = 0.0f;
	m_pContext = nullptr;
}
