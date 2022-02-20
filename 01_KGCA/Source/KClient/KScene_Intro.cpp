#include "KScene_Intro.h"
#include "KSceneManager.h"
bool KScene_Intro::Load(std::wstring file)
{
	m_BGM = g_SoundManager.LoadSound(L"../../data/sound/bgm/Opening Movie.wav");
	m_BGM->SoundPlay(true);
	return true;
}

bool KScene_Intro::Init(ID3D11DeviceContext* context)
{
	m_SceneID = S_GAME;
	m_pContext = context;
	return true;
}

bool KScene_Intro::Frame()
{
	m_BGM->Frame();
	if (g_InputData.bUpKey)
	{
		g_SceneManager.SetScene(2);
	}
	return true;
}

bool KScene_Intro::Render()
{
	return true;
}

bool KScene_Intro::Release()
{
	m_BGM->SoundStop();
	return true;
}
