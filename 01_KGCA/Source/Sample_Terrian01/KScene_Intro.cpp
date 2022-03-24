#include "KScene_Intro.h"
#include "KSceneManager.h"
#include "KSkyBox.h"
#include "KState.h"
#include "ImGuiManager.h"
bool KScene_Intro::Load(std::wstring file)
{
	return true;
}

bool KScene_Intro::Init(ID3D11DeviceContext* context)
{
	KScene::Init(context);
	m_SceneID = S_INTRO;

	m_SkyBox.Init(context, L"../../data/shader/Skybox.hlsl",L"../../data/texture/Skybox_dd.dds");

	m_Terrian.Init(m_pContext, L"../../data/map/129_heightmap.jpg");
	m_Terrian.CreateObject(L"../../data/shader/VS_Normalmap.hlsl", L"../../data/shader/PS_Normalmap.hlsl", L"../../data/map/baseColor.jpg",
		L"../../data/map/Ground_Grass_001_ROUGH.jpg", L"../../data/map/Ground_Grass_001_NORM.jpg");
	m_Lod.Build(&m_Terrian);

	m_Box.Init(L"../../data/shader/VS_Normalmap.hlsl", L"../../data/shader/PS_Normalmap.hlsl", L"../../data/texture/brick.jpg",
		L"../../data/texture/brick.jpg", L"../../data/texture/brick_normal.jpg");
	m_Box.m_matWorld._42 = 1.0f;

	//카메라 초기화
	m_Camera.Init();
	m_Camera.CreateViewMatrix(KVector3(-2, 3, -8), KVector3(0, 0, 0));
	m_Camera.CreateProjMatrix(1.0f, 300.0f, XM_PI * 0.45f, 
		static_cast<float>(g_rtClient.right)/ static_cast<float>(g_rtClient.bottom));
	return true;
}

bool KScene_Intro::Frame()
{
	m_Camera.Frame();
	m_Box.Frame();

#pragma region  오브젝트 이동
	if (g_InputData.bUpKey)
	{
		m_Box.m_pos += m_Box.m_vLook * g_fSecPerFrame * 10.0f;
	}
	if (g_InputData.bDownKey)
	{
		m_Box.m_pos -= m_Box.m_vLook * g_fSecPerFrame * 10.0f;
	}
	if (g_InputData.bRightKey)
	{
		m_Box.m_rot.y += g_fSecPerFrame * 4.0f;
	}
	if (g_InputData.bLeftKey)
	{
		m_Box.m_rot.y -= g_fSecPerFrame * 4.0f;
	}
	m_Box.m_pos.y = m_Terrian.GetHeight(m_Box.m_pos.x, m_Box.m_pos.z)+1.0f;
	m_Box.SetRotation(m_Box.m_rot);
	m_Box.SetPosition(m_Box.m_pos);
#pragma endregion
	KScene::Frame();
	return true;
}

bool KScene_Intro::Render()
{
	m_SkyBox.m_matSkyView = m_Camera.m_matView;
	m_SkyBox.m_matSkyView._41 = 0;
	m_SkyBox.m_matSkyView._42 = 0;
	m_SkyBox.m_matSkyView._43 = 0;

	m_SkyBox.SetMatrix(&m_SkyBox.m_matWorld, &m_SkyBox.m_matSkyView, &m_Camera.m_matProj);
	m_SkyBox.Render(m_pContext);

	m_Terrian.SetMatrix(nullptr, &m_Camera.m_matView, &m_Camera.m_matProj);
	m_Terrian.m_cbData.vLightColor = { 0.9f,0.9f,0.85f,1 };
	m_Terrian.m_cbData.vLightPos = { 100.0f,100.0f,1.0f };
	m_Terrian.m_cbData.vCamPos = { m_Camera.GetCameraPos()->x, m_Camera.GetCameraPos()->y, m_Camera.GetCameraPos()->z, 1.0f };
	//m_Terrian.Render(m_pContext);
	m_Lod.Render(m_pContext,m_Camera.GetCameraPos());

	m_Box.SetMatrix(&m_Box.m_matWorld, &m_Camera.m_matView, &m_Camera.m_matProj);
	m_Box.m_cbData.vLightColor = { 0.9f,0.9f,0.85f,1 };
	m_Box.m_cbData.vLightPos = { 100.0f,100.0f,1.0f };
	m_Box.m_cbData.vCamPos = { m_Camera.GetCameraPos()->x , m_Camera.GetCameraPos()->y, m_Camera.GetCameraPos()->z, 1.0f};
	m_Box.Render(m_pContext);
	
	KScene::Render();
	return true;
}

bool KScene_Intro::Release()
{
	m_Lod.Release();
	m_Camera.Release();
	m_Box.Release();
	KScene::Release();
	return true;
}
