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

		D3DKMatrixScaling(&matScale, randstep(1.0f, 4.0f),
			randstep(1.0f, 4.0f),
			randstep(1.5f, 4.0f));
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

	//�̴ϸ�-------------------------------------------------------------
	m_Minimap.Init(L"../../data/shader/VS_Plane.hlsl", L"../../data/shader/PS_Plane.hlsl");
	m_Minimap.m_Rt.Create(512, 512);

	//��ī�̹ڽ�-------------------------------------------------------------
	m_SkyBox.Init(context, L"../../data/shader/Skybox.hlsl",L"../../data/texture/Skybox_Miramar.dds");

	//����-------------------------------------------------------------
	m_Terrian.Init(m_pContext, L"../../data/map/129_heightmap.jpg");
	m_Terrian.CreateObject(L"../../data/shader/VS_Normalmap.hlsl", L"../../data/shader/PS_Normalmap.hlsl", L"../../data/map/baseColor.jpg",
		L"../../data/map/Ground_Grass_001_ROUGH.jpg", L"../../data/map/Ground_Grass_001_NORM.jpg");
	m_Lod.Build(&m_Terrian, &m_Camera);

	m_Lod.DrawDebugInit(m_pContext);
	SetupMapObject();
	for (int iObj = 0; iObj < m_pObjList.size(); iObj++)
	{
		m_Lod.AddObject(m_pObjList[iObj]);
	}

	//ī�޶� �ʱ�ȭ-------------------------------------------------------------
	m_Camera.m_pVS = g_ShaderManager.CreateVertexShader(L"../../data/shader/VSPS_Frustum.hlsl", "VS");
	m_Camera.m_pPS = g_ShaderManager.CreatePixelShader(L"../../data/shader/VSPS_Frustum.hlsl", "PS");
	m_Camera.Init(m_pContext);
	m_Camera.CreateViewMatrix(KVector3(-2, 3, -8), KVector3(0, 0, 0));
	m_Camera.CreateProjMatrix(1.0f, 1000.0f, XM_PI * 0.45f, 
		static_cast<float>(g_rtClient.right)/ static_cast<float>(g_rtClient.bottom));
	
	m_TopView.Init(m_pContext);
	m_TopView.CreateViewMatrix(KVector3(0, 150.0f, -1),KVector3(0, 0, 0));
	m_TopView.CreateProjMatrix(1.0f, 10000.0f, XM_PI * 0.45f,
		static_cast<float>(g_rtClient.right) / static_cast<float>(g_rtClient.bottom));

	//Fbx ���� �ε�-------------------------------------------------------------
	m_FbxLoader.Init();
	m_FbxLoader.Load(L"../../data/model/SM_Barrel.FBX");
	for (int iObj = 0; iObj < m_FbxLoader.m_ObjectList.size(); iObj++)
	{
		if (m_FbxLoader.m_ObjectList[iObj]->m_tex_name_diffuse.empty())
		{
			m_FbxLoader.m_ObjectList[iObj]->m_pTexture_Diffuse = g_TextureMananger.Load(L"../../data/texture/brick.jpg");
		}
		else
		{
			m_FbxLoader.m_ObjectList[iObj]->m_pTexture_Diffuse = g_TextureMananger.Load(m_FbxLoader.m_ObjectList[iObj]->m_tex_name_diffuse);
		}
		m_FbxLoader.m_ObjectList[iObj]->SetPosition(KVector3(0.0f, 1.0f, 0.0f));
		m_FbxLoader.m_ObjectList[iObj]->m_matWorld._11 = 0.2f;
		m_FbxLoader.m_ObjectList[iObj]->m_matWorld._22 = 0.2f;
		m_FbxLoader.m_ObjectList[iObj]->m_matWorld._33 = 0.2f;
		if (!m_FbxLoader.m_ObjectList[iObj]->CreateObject(L"../../data/shader/VS_Normalmap.hlsl", L"../../data/shader/PS_Normalmap.hlsl", L"../../data/model/T_Pack_01_D.jpg",
			L"../../data/model/T_Pack_01_S.jpg", L"../../data//T_Pack_01_N.jpg"))
		{
			return false;
		}
	}

	//���콺 ��Ŀ------------------------------------------------------------
	m_MousePicker.Init(m_pContext, &m_Lod, &m_Camera);

	//����Ʈ ----------------------------------------------------------------
	m_Light.SetLight(KVector3(10.0f,100.0f,0.0f), KVector3(0.0f, 0.0f, 0.0f));
	m_Shadow.CreateShadow(&m_Light);
	return true;
}

bool KScene_Intro::Frame()
{
	m_Camera.Frame();
	m_MousePicker.Frame();
	m_Light.Frame();
	m_Shadow.Frame(); // ������ ��� ���, �������� ��� ,�ؽ��� ��� ���Ѱ�

	if (ImGui::Begin(u8"����� �ν�����"))
	{
		float* lightPos[3]	= { &m_Light .m_vPos.x,&m_Light.m_vPos.y,&m_Light.m_vPos.z};
		float* lightColor[3] = {&m_Light.m_vLightColor.x,&m_Light.m_vLightColor.y,&m_Light.m_vLightColor.z};
		ImGui::Text(u8"Light Position"); ImGui::SameLine();
		ImGui::InputFloat3("##lightpos", *lightPos, 1, 0);
		ImGui::Text(u8"Light Color"); ImGui::SameLine();
		ImGui::InputFloat3("##lightcolor", *lightColor, 1, 1);
	}
	ImGui::End();

	KScene::Frame();
	return true;
}


bool KScene_Intro::Render()
{
	float shadow[4] = { 1.0f,1.0f,1.0f,1.0f };
	//�׸��� ------------------------------------------
	if (m_Shadow.m_ShadowRT.Begin(m_pContext, shadow))
	{
		//����Ʈ ���⿡�� ĸ��
		m_Terrian.SetMatrix(&m_Terrian.m_matWorld,
			&m_Light.m_matView, &m_Light.m_matProj);
		//���̴� ���̴��� ��ü
		m_Terrian.m_pPS = m_Shadow.m_pPSShadow;
		//���� ��� ������ ������
		m_Lod.Render(m_pContext);
		//���� �۾�
		m_Shadow.m_ShadowRT.End(m_pContext);
	}
	m_Shadow.m_Shadow_cbData.m_matShadow = m_Shadow.m_Shadow_cbData.m_matShadow.Transpose();
	m_pContext->UpdateSubresource(
		m_Shadow.m_pShadowCB.Get(), 0, NULL, &m_Shadow.m_Shadow_cbData, 0, 0);
	m_pContext->VSSetConstantBuffers(2, 1, m_Shadow.m_pShadowCB.GetAddressOf());

	//���÷� ���� : Ŭ���� -> �׸��ڿ�
	ApplySS(m_pContext, KState::g_pClampSS, 1);
	D3DKMatrixInverse(&m_Terrian.m_cbData.matNormal, NULL,
		&m_Terrian.m_matWorld);

	//��ī�̹ڽ� ���� ����------------------------------
	m_Terrian.SetMatrix(&m_Terrian.m_matWorld, &m_Camera.m_matView, &m_Camera.m_matProj);
	m_Terrian.m_cbData.vLightColor = { m_Light.m_vLightColor.x,m_Light.m_vLightColor.y,m_Light.m_vLightColor.z,1.0f };
	m_Terrian.m_cbData.vLightPos =   { m_Light.m_vPos.x,m_Light.m_vPos.y,m_Light.m_vPos.z};
	m_Terrian.m_cbData.vCamPos = { m_Camera.GetCameraPos()->x, m_Camera.GetCameraPos()->y, m_Camera.GetCameraPos()->z, 1.0f };

	m_pContext->PSSetShaderResources(1, 1, &m_Shadow.m_ShadowRT.m_pTextureSRV); // �ؽ��� �ѱ�
	m_Lod.Render(m_pContext);
	m_Lod.ImGuiRender(m_pContext);
	m_SkyBox.m_matSkyView = m_Camera.m_matView;
	m_SkyBox.m_matSkyView._41 = 0;
	m_SkyBox.m_matSkyView._42 = 0;
	m_SkyBox.m_matSkyView._43 = 0;

	m_SkyBox.SetMatrix(&m_SkyBox.m_matWorld, &m_SkyBox.m_matSkyView, &m_Camera.m_matProj);
	m_SkyBox.Render(m_pContext);


	//FBX Render------------------------------------------
	for (int iObj = 0; iObj < m_FbxLoader.m_ObjectList.size(); iObj++)
	{
		m_FbxLoader.m_ObjectList[iObj]->m_cbData.vLightColor = { m_Light.m_vLightColor.x,m_Light.m_vLightColor.y,m_Light.m_vLightColor.z,1.0f };
		m_FbxLoader.m_ObjectList[iObj]->m_cbData.vLightPos = { m_Light.m_vPos.x,m_Light.m_vPos.y,m_Light.m_vPos.z };
		m_FbxLoader.m_ObjectList[iObj]->m_cbData.vCamPos = { m_Camera.GetCameraPos()->x , m_Camera.GetCameraPos()->y, m_Camera.GetCameraPos()->z, 1.0f };

		m_FbxLoader.m_ObjectList[iObj]->SetMatrix(&m_FbxLoader.m_ObjectList[iObj]->m_matWorld, &m_Camera.m_matView, &m_Camera.m_matProj);
		m_FbxLoader.m_ObjectList[iObj]->Render(m_pContext);
	}

	float color[4] = { 0.2f,0.2f,0.2f,1.0f };
	//�̴ϸ�
	if (m_Minimap.m_Rt.Begin(m_pContext, color))
	{
		ApplyBS(m_pContext, KState::g_pAlphaBlendState);
		m_Terrian.SetMatrix(nullptr, &m_TopView.m_matView, &m_TopView.m_matProj);
		m_Lod.Render(m_pContext);
		m_Camera.SetMatrix(nullptr, &m_TopView.m_matView,
			&m_TopView.m_matProj);
		m_Camera.Render(m_pContext);
		m_Minimap.m_Rt.End(m_pContext);
		KState::g_pCurrentBS = KState::g_pBlendState;
	}
	m_Minimap.Render(m_pContext);
	
	KScene::Render();
	return true;
}

bool KScene_Intro::Release()
{
	m_FbxLoader.Release();
	m_SkyBox.Release();
	m_Lod.Release();
	m_Terrian.Release();
	m_Camera.Release();
	m_TopView.Release();
	m_Minimap.Release();
	m_MousePicker.Release();
	KScene::Release();
	return true;
}
