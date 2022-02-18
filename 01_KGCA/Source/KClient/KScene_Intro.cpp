#include "KScene_Intro.h"

bool KScene_Intro::Load(std::wstring file)
{
	m_BGM = g_Sound.LoadSound(L"../../data/sound/bgm/Opening Movie.wav");
	m_BGM->SoundPlay(true);
	return true;
}

bool KScene_Intro::Init(ID3D11Device* device, ID3D11DeviceContext* context)
{
	m_SceneID = S_GAME;
	m_pd3dDevice = device;
	m_pContext = context;
	return true;
}

bool KScene_Intro::Frame()
{
	m_BGM->Frame();
	
	return true;
}

bool KScene_Intro::Render()
{
	return true;
}

bool KScene_Intro::Release()
{
	m_BGM->SoundStop();
	m_BGM->Release();
	return true;
}
