#include "KScene_Intro.h"
#include "KSceneManager.h"
#include "KSkyBox.h"
#include "KState.h"
bool KScene_Intro::Load(std::wstring file)
{
	return true;
}

bool KScene_Intro::Init(ID3D11DeviceContext* context)
{
	KScene::Init(context);
	m_SceneID = S_INTRO;

	std::shared_ptr<KSkyBox> skybox(new KSkyBox);
	skybox.get()->Init(context, L"../../data/shader/Skybox.hlsl",L"../../data/texture/Skybox_dd.dds");
	m_MapObj.push_back(skybox);
	m_Box.Init(L"../../data/shader/VS_0.hlsl", L"../../data/shader/PS_0.hlsl", L"../../data/texture/UV.bmp", L"");
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
	m_Camera.CreateProjMatrix(1.0f, 1000.0f, XM_PI * 0.25f, 
		static_cast<float>(g_rtClient.right)/ static_cast<float>(g_rtClient.bottom));
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
	m_MapObj[0].get()->SetMatrix(&m_Camera.m_matWorld, &m_Camera.m_matView, &m_Camera.m_matProj);
	KScene::Render();
	m_Box.SetMatrix(&m_Box.m_matWorld, &m_Camera.m_matView, &m_Camera.m_matProj);
	m_Box.m_cbData.vLightPos = { -30,80,-40,1 };
	m_Box.m_cbData.vCamPos = { m_Camera.GetCameraPos()->x , m_Camera.GetCameraPos()->y, m_Camera.GetCameraPos()->z, 1.0f};
	m_Box.Render(m_pContext);
	return true;
}

bool KScene_Intro::Release()
{
	m_Camera.Release();
	m_Box.Release();
	KScene::Release();
	return true;
}
