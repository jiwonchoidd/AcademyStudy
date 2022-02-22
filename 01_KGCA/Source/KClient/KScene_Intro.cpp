#include "KScene_Intro.h"
#include "KSceneManager.h"
bool KScene_Intro::Load(std::wstring file)
{
	m_BGM = g_SoundManager.LoadSound(L"../../data/sound/bgm/Opening Movie.wav");
	m_BGM->SoundPlay(true);
	K2DAsset* obj = new K2DAsset;
	obj->Init();
	obj->SetRectDraw({ 0, 0, g_rtClient.right, g_rtClient.bottom });
	obj->SetPosition(KVector2(g_rtClient.right / 2.0f, g_rtClient.bottom / 2.0f));
	if (!obj->CreateObject_Mask(L"../../data/shader/VS_2D.txt", L"../../data/shader/PS_2D.txt",
		L"../../data/texture/DS DSi - Pokemon Diamond Pearl - Intro Movie.png", L""))
	{
		return false;
	}
	m_UIobj.insert(std::make_pair(0, obj));
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
		return true;
	}
	for (auto obj : m_UIobj)
	{
		KObject* pObj = obj.second;
		if (pObj != nullptr)
		{
			
		}
	}
	return true;
}

bool KScene_Intro::Render()
{
	for (auto obj : m_UIobj)
	{
		KObject* pObj = obj.second;
		if (pObj != nullptr)
		{
			pObj->Render(m_pContext);
		}
	}
	return true;
}

bool KScene_Intro::Release()
{
	m_BGM->SoundStop();
	KScene::Release();
	return true;
}
