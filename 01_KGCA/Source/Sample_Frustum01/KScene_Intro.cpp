#include "KScene_Intro.h"
#include "KSceneManager.h"
#include "KSkyBox.h"
#include "KState.h"
#include "ImGuiManager.h"
void KScene_Intro::SetupMapObject()
{
	srand(time(NULL));

	m_pBoxObj = std::shared_ptr<KBoxObj>(new KBoxObj);
	m_pBoxObj->Init(L"../../data/shader/VS_Normalmap.hlsl", L"../../data/shader/PS_Normalmap.hlsl", L"../../data/texture/brick.jpg",
		L"../../data/texture/brick.jpg", L"../../data/texture/brick_normal.jpg");

	KMatrix matScale;
	KMatrix matRotateObj;
	for (int iObj = 0; iObj < 30; iObj++)
	{
		KMapObject* pObj= new KMapObject;
		for (int iv = 0; iv < 8; iv++)
		{
			pObj->obj_box.List[iv] = m_pBoxObj.get()->m_BoxCollision.List[iv];
		}
		pObj->obj_pos = KVector3(
			randstep(m_Terrian.m_BoxCollision.min.x, m_Terrian.m_BoxCollision.max.x),
			0.0f,
			randstep(m_Terrian.m_BoxCollision.min.z, m_Terrian.m_BoxCollision.max.z));

		D3DKMatrixScaling(&matScale, randstep(10.0f, 100.0f),
			randstep(10.0f, 100.0f),
			randstep(10.0f, 100.0f));
		D3DKMatrixRotationYawPitchRoll(&matRotateObj,
			cosf(randstep(0.0f, 360.0f)) * XM_PI,
			sinf(randstep(0.0f, 360.0f)) * XM_PI,
			1.0f);
		pObj->obj_matWorld = matScale * matRotateObj;
		pObj->obj_pos.y = m_Terrian.GetHeight(pObj->obj_pos.x, pObj->obj_pos.z);
		pObj->obj_matWorld._41 = pObj->obj_pos.x;
		pObj->obj_matWorld._42 = pObj->obj_pos.y;
		pObj->obj_matWorld._43 = pObj->obj_pos.z;
		pObj->UpdateData();
		pObj->UpdateCollision();
		pObj->obj_pObject = static_cast<K3DAsset*>(m_pBoxObj.get());
		m_pObjList.push_back(pObj);
	}
}

bool KScene_Intro::Load(std::wstring file)
{
	return true;
}

bool KScene_Intro::Init(ID3D11DeviceContext* context)
{
	KScene::Init(context);
	m_SceneID = S_INTRO;

	m_SkyBox.Init(context, L"../../data/shader/Skybox.hlsl",L"../../data/texture/Skybox_Miramar.dds");

	m_Terrian.Init(m_pContext, L"../../data/map/129_heightmap.jpg");
	m_Terrian.CreateObject(L"../../data/shader/VS_Normalmap.hlsl", L"../../data/shader/PS_Normalmap.hlsl", L"../../data/map/baseColor.jpg",
		L"../../data/map/Ground_Grass_001_ROUGH.jpg", L"../../data/map/Ground_Grass_001_NORM.jpg");
	m_Lod.Build(&m_Terrian, &m_Camera);

	m_Box.Init(L"../../data/shader/VS_Normalmap.hlsl", L"../../data/shader/PS_Normalmap.hlsl", L"../../data/texture/brick.jpg",
		L"../../data/texture/brick.jpg", L"../../data/texture/brick_normal.jpg");
	m_Box.m_matWorld._42 = 1.0f;

	//카메라 초기화
	m_Camera.m_pVS = g_ShaderManager.CreateVertexShader(L"../../data/shader/VSPS_Frustum.hlsl", "VS");
	m_Camera.m_pPS = g_ShaderManager.CreatePixelShader(L"../../data/shader/VSPS_Frustum.hlsl", "PS");
	m_Camera.Init(m_pContext);
	m_Camera.CreateViewMatrix(KVector3(-2, 3, -8), KVector3(0, 0, 0));
	m_Camera.CreateProjMatrix(1.0f, 1000.0f, XM_PI * 0.45f, 
		static_cast<float>(g_rtClient.right)/ static_cast<float>(g_rtClient.bottom));
	
	m_TopView.Init(m_pContext);
	m_TopView.CreateViewMatrix(KVector3(0, 3000.0f, -1),KVector3(0, 0, 0));
	m_TopView.CreateProjMatrix(1.0f, 10000.0f, XM_PI * 0.45f,
		static_cast<float>(g_rtClient.right) / static_cast<float>(g_rtClient.bottom));

	SetupMapObject();
	for (int iObj = 0; iObj < m_pObjList.size(); iObj++)
	{
		m_Lod.AddObject(m_pObjList[iObj]);
	}
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

	if (ImGui::Begin(u8"디버깅 인스펙터"))
	{
		ImGui::Text(u8"Light Position"); ImGui::SameLine();
		ImGui::InputFloat3("##lightpos", m_LightPos, 1,0); 
		ImGui::Text(u8"Light Color"); ImGui::SameLine();
		ImGui::InputFloat3("##lightcolor", m_LightColor, 1, 1);
	}
	ImGui::End();

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
	m_Terrian.m_cbData.vLightColor = { m_LightColor[0],m_LightColor[1],m_LightColor[2],1 };
	m_Terrian.m_cbData.vLightPos = { m_LightPos[0],m_LightPos[1],m_LightPos[2]};
	m_Terrian.m_cbData.vCamPos = { m_Camera.GetCameraPos()->x, m_Camera.GetCameraPos()->y, m_Camera.GetCameraPos()->z, 1.0f };
	//m_Terrian.Render(m_pContext);
	m_Lod.Render(m_pContext);

	m_Box.SetMatrix(&m_Box.m_matWorld, &m_Camera.m_matView, &m_Camera.m_matProj);
	m_Box.m_cbData.vLightColor = { m_LightColor[0],m_LightColor[1],m_LightColor[2],1 };
	m_Box.m_cbData.vLightPos = { m_LightPos[0],m_LightPos[1],m_LightPos[2] };
	m_Box.m_cbData.vCamPos = { m_Camera.GetCameraPos()->x , m_Camera.GetCameraPos()->y, m_Camera.GetCameraPos()->z, 1.0f};
	m_Box.Render(m_pContext);
	//미니맵
	m_Camera.SetMatrix(nullptr, &m_TopView.m_matView,
		&m_TopView.m_matProj);
	m_Camera.Render(m_pContext);
	

	KScene::Render();
	return true;
}

bool KScene_Intro::Release()
{
	m_SkyBox.Release();
	m_Lod.Release();
	m_Terrian.Release();
	m_Camera.Release();
	m_TopView.Release();
	m_Box.Release();
	KScene::Release();
	return true;
}
