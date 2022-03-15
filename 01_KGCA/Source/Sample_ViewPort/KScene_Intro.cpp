#include "KScene_Intro.h"
#include "KSceneManager.h"
bool KScene_Intro::Load(std::wstring file)
{
	return true;
}

bool KScene_Intro::Init(ID3D11DeviceContext* context)
{
	KScene::Init(context);
	m_SceneID = S_INTRO;


	m_Box.Init(L"../../data/shader/VS_0.txt", L"../../data/shader/PS_0.txt", L"../../data/texture/UV.bmp");
	KMatrix R;
	D3DKMatrixIdentity(&R);
	R._11 = cos(3.14 / 4.0f); //0
	R._12 = sin(3.14 / 4.0f);
	R._21 = -sin(3.14 / 4.0f);
	R._22 = cos(3.14 / 4.0f);

	m_Box.m_matWorld = m_Box.m_matWorld;

	////카메라 초기화
	m_Camera.Init();
	m_Camera.CreateViewMatrix(KVector3(0, 0, -5), KVector3(0, 0, 0));
	m_Camera.CreateProjMatrix(1.0f, 1000.0f, XM_PI * 0.25f, (float)g_rtClient.right / (float)g_rtClient.bottom);
	return true;
}

bool KScene_Intro::Frame()
{
	m_Camera.Frame();
	m_Box.Frame();
	KScene::Frame();
	return true;
}

bool KScene_Intro::Render()
{
	m_Box.SetMatrix(&m_Box.m_matWorld, &m_Camera.m_matView, &m_Camera.m_matProj);
	m_Box.Render(m_pContext);
	KScene::Render();
	return true;
}

bool KScene_Intro::Release()
{
	m_Camera.Release();
	m_Box.Release();
	KScene::Release();
	return true;
}
