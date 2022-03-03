#include "KSceneManager.h"

bool KSceneManager::Init(ID3D11DeviceContext* context)
{
	m_pContext = context;

	//첫번째 씬은 인트로 설정한다. 
	m_pCurrentScene = new KScene_Intro;
	m_CurrentScene_Index = S_INTRO;
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
			//오브젝트 리스트 삭제
			g_ObjManager.Release();
			m_pCurrentScene->Release();
			delete m_pCurrentScene;
			m_pCurrentScene = nullptr;
		}
		//상태 패턴
		switch (index)
		{
			/*	S_INTRO,
				S_GAME_0,
				S_GAME_1,
				S_COMBAT,*/
		case S_INTRO:
		{
			m_pCurrentScene = new KScene_Intro;
			m_CurrentScene_Index = S_INTRO;
			break;
		}
		case S_GAME_0:
		{
			m_pCurrentScene = new KScene_Game_0;
			m_CurrentScene_Index = S_GAME_0;
			break;
		}
		case S_GAME_1:
		{
			m_pCurrentScene = new KScene_Game_1;
			m_CurrentScene_Index = S_GAME_1;
			break;
		}
		case S_COMBAT:
		{
			m_pCurrentScene = new KScene_Intro;
			m_CurrentScene_Index = S_COMBAT;
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
