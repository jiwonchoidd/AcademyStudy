#include "KScene_Intro.h"
#include "KSceneManager.h"
#include "KSkyBox.h"
#include "KMap.h"
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
	
	std::shared_ptr<KMap> map(new KMap);
	map.get()->Init(m_pContext, L"../../data/map/129_heightmap.jpg");
	map->CreateObject(L"../../data/shader/VS_Normalmap.hlsl", L"../../data/shader/PS_Normalmap.hlsl", L"../../data/map/baseColor.jpg",
		L"../../data/map/Ground_Grass_001_ROUGH.jpg", L"../../data/map/Ground_Grass_001_NORM.jpg");
	m_MapObj.push_back(map);
	

	m_Box.Init(L"../../data/shader/VS_Normalmap.hlsl", L"../../data/shader/PS_Normalmap.hlsl", L"../../data/texture/brick.jpg",
		L"../../data/texture/brick.jpg", L"../../data/texture/brick_normal.jpg");
	m_Box.m_matWorld._42 = 1.0f;

	//카메라 초기화
	m_Camera.Init();
	m_Camera.CreateViewMatrix(KVector3(-2, 3, -8), KVector3(0, 0, 0));
	m_Camera.CreateProjMatrix(1.0f, 1000.0f, XM_PI * 0.35f, 
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
	m_MapObj[1].get()->SetMatrix(nullptr, &m_Camera.m_matView, &m_Camera.m_matProj);

	m_MapObj[1]->m_cbData.vLightColor = { 0.9f,0.9f,0.85f,1 };
	m_MapObj[1]->m_cbData.vLightPos = { 300.0f,300.0f,-300.0f,1.0f };
	m_MapObj[1]->m_cbData.vCamPos = { m_Camera.GetCameraPos()->x , m_Camera.GetCameraPos()->y, m_Camera.GetCameraPos()->z, 1.0f };
	KScene::Render();
	m_Box.SetMatrix(&m_Box.m_matWorld, &m_Camera.m_matView, &m_Camera.m_matProj);
	m_Box.m_cbData.vLightColor = { 0.9f,0.9f,0.85f,1 };
	m_Box.m_cbData.vLightPos = { 300.0f,300.0f,-300.0f,1.0f };
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
