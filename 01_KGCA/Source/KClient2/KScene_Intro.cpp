#include "KScene_Intro.h"
#include "KSceneManager.h"
#include "KUI.h"
bool KScene_Intro::Load(std::wstring file)
{
	//사운드 바로 실행
	m_BGM = g_SoundManager.LoadSound(L"../../data/sound/bgm/Opening Movie.wav");
	m_BGM->SoundPlay(true);

	//UI 생성
	KImage* obj = new KImage;
	obj->SetRectDraw({ 0, 0, g_rtClient.right, g_rtClient.bottom });
	obj->SetPosition(KVector2(g_rtClient.right / 2.0f, g_rtClient.bottom / 2.0f));
	if (!obj->Init(m_pContext,L"../../data/shader/VSPS_UI_0.txt", L"../../data/shader/VSPS_UI_0.txt",
		L"../../data/texture/DS DSi - Pokemon Diamond Pearl - Intro Movie.png", L""))
	{
		return false;
	}
	obj->m_fAlpha = 0.0f;
	obj->m_bFadeIn = true;
	m_UIObj.push_back(obj);
	return true;
}

bool KScene_Intro::Init(ID3D11DeviceContext* context)
{
	KScene::Init(context);
	m_SceneID = S_MENU;
	return true;
}

bool KScene_Intro::Frame()
{
	m_BGM->Frame();
	if (g_InputData.bUpKey)
	{
		g_SceneManager.SetScene(2);
		return true;
	}
	KScene::Frame();
	return true;
}

bool KScene_Intro::Render()
{
	KScene::Render();
	return true;
}

bool KScene_Intro::Release()
{
	m_BGM->SoundStop();
	KScene::Release();
	return true;
}
