#include "KScene_Maptool.h"
#include "KSceneManager.h"
#include "KSkyBox.h"
#include "KState.h"
#include "ImGuiManager.h"
bool KScene_Maptool::Load(std::wstring file)
{
	//todo:파일입출력 맵 구성
	return true;
}

bool KScene_Maptool::Init(ID3D11DeviceContext* context)
{
	KScene::Init(context);
	m_SceneID = S_MapTool;

	//스카이박스-------------------------------------------------------------
	m_SkyBox.Init(context, L"../../data/shader/Skybox.hlsl",L"../../data/texture/Skybox_Miramar.dds");
	//미니맵-------------------------------------------------------------
	m_MiniMap_DebugShadow.Init(-1.0f, -0.4f);
	m_MiniMap_DebugCamera.Init(0.5f, 1.0f);
	//Fbx 파일 로드-------------------------------------------------------------
	m_FbxLoader.Init();
	m_FbxLoader.Load(L"../../data/model/SM_Barrel.FBX");
	for (int iObj = 0; iObj < m_FbxLoader.m_ObjectList.size(); iObj++)
	{
		/*if (m_FbxLoader.m_ObjectList[iObj]->m_tex_name_diffuse.empty())
		{
			m_FbxLoader.m_ObjectList[iObj]->m_pTexture_Diffuse = g_TextureMananger.Load(L"../../data/texture/brick.jpg");
		}
		else
		{
			m_FbxLoader.m_ObjectList[iObj]->m_pTexture_Diffuse = g_TextureMananger.Load(m_FbxLoader.m_ObjectList[iObj]->m_tex_name_diffuse);
		}*/
 		m_FbxLoader.m_ObjectList[iObj]->SetPosition(KVector3(0.0f, 1.0f, 0.0f));
		m_FbxLoader.m_ObjectList[iObj]->m_matWorld._11 = 0.1f;
		m_FbxLoader.m_ObjectList[iObj]->m_matWorld._22 = 0.1f;
		m_FbxLoader.m_ObjectList[iObj]->m_matWorld._33 = 0.1f;
		if (!m_FbxLoader.m_ObjectList[iObj]->CreateObject(L"../../data/shader/VSPS_DepthShadow.hlsl", L"../../data/shader/VSPS_DepthShadow.hlsl", L"../../data/model/T_Pack_01_D.jpg",
			L"../../data/model/T_Pack_01_S.jpg", L""))
		{
			return false;
		}
	}

	//지형-------------------------------------------------------------
	m_Terrian.Init(m_pContext, L"../../data/map/129_heightmap.jpg");
	m_Terrian.CreateObject(L"../../data/shader/VSPS_DepthShadow.hlsl", L"../../data/shader/VSPS_DepthShadow.hlsl", L"../../data/map/baseColor.jpg",
		L"../../data/map/Ground_Grass_001_ROUGH.jpg", L"../../data/map/Ground_Grass_001_NORM.jpg");
	m_Terrian_Space.Build(&m_Terrian, &m_Camera);
	m_Terrian_Space.DrawDebugInit(m_pContext);

	std::shared_ptr<KBoxObj> tempBox = std::make_shared<KBoxObj>();
	tempBox.get()->Init(L"../../data/shader/VSPS_DepthShadow.hlsl", L"../../data/shader/VSPS_DepthShadow.hlsl", 
		L"../../data/texture/brick.jpg", L"../../data/texture/brick.jpg", L"../../data/texture/brick_normal.jpg");
	m_Terrian_Space.RandomSetupObject(tempBox.get(),20);

	m_Scene_ObjList.push_back(tempBox);

	//카메라 초기화-------------------------------------------------------------
	m_Camera.m_pVS = g_ShaderManager.CreateVertexShader(L"../../data/shader/VSPS_Frustum.hlsl", "VS");
	m_Camera.m_pPS = g_ShaderManager.CreatePixelShader(L"../../data/shader/VSPS_Frustum.hlsl", "PS");
	m_Camera.Init(m_pContext);
	m_Camera.CreateViewMatrix(KVector3(100, 100, 0), KVector3(0, 0, 0));
	m_Camera.CreateProjMatrix(1.0f, 10000.0f, XM_PI * 0.45f,
		static_cast<float>(g_rtClient.right)/ static_cast<float>(g_rtClient.bottom));
	
	m_TopView.Init(m_pContext);
	m_TopView.CreateViewMatrix(KVector3(0, 400.0f, -1),KVector3(0, 0, 0));
	m_TopView.CreateProjMatrix(1.0f, 10000.0f, XM_PI * 0.45f,
		static_cast<float>(g_rtClient.right) / static_cast<float>(g_rtClient.bottom));

	//라이트 그림자----------------------------------------------------------------
	m_Light.SetLight(KVector3(100.0f,420.0f,0.0f), KVector3(0.0f, 0.0f, 0.0f));
	m_Shadow.CreateShadow(&m_Light);

	//마우스 피커------------------------------------------------------------
	m_MousePicker.Init(m_pContext, &m_Terrian_Space, &m_Camera);

	return true;
}

bool KScene_Maptool::Frame()
{
	m_Camera.Frame();
	m_MousePicker.Frame();
	m_Light.Frame();
	m_Shadow.Frame(); // 쉐도우 행렬 계산, 프로젝션 행렬 ,텍스쳐 행렬 곱한것

	if (ImGui::Begin(u8"디버깅 인스펙터"))
	{
		float* lightPos[3]	= { &m_Light .m_vPos.x,&m_Light.m_vPos.y,&m_Light.m_vPos.z};
		float* lightColor[3] = { &m_Light.m_vLightColor.x,&m_Light.m_vLightColor.y,&m_Light.m_vLightColor.z };
		float* lightTarget[3] = {&m_Light.m_vTarget.x,&m_Light.m_vTarget.y,&m_Light.m_vTarget.z};
		ImGui::Text(u8"Light Position"); ImGui::SameLine();
		ImGui::InputFloat3("##lightpos", *lightPos, 1, 0);
		ImGui::Text(u8"Light Target"); ImGui::SameLine();
		ImGui::InputFloat3("##lightTarget", *lightTarget, 1, 0);
		ImGui::Text(u8"Light Color"); ImGui::SameLine();
		ImGui::InputFloat3("##lightcolor", *lightColor, 1, 0);
	}
	ImGui::End();

	KScene::Frame();
	return true;
}


bool KScene_Maptool::Render()
{
	//스카이박스------------------------------------------
	m_SkyBox.m_matSkyView = m_Camera.m_matView;
	m_SkyBox.m_matSkyView._41 = 0;
	m_SkyBox.m_matSkyView._42 = 0;
	m_SkyBox.m_matSkyView._43 = 0;

	m_SkyBox.SetMatrix(&m_SkyBox.m_matWorld, &m_SkyBox.m_matSkyView, &m_Camera.m_matProj);
	m_SkyBox.Render(m_pContext);

	float shadow[4] = { 1.0f,1.0f,1.0f,1.0f };
	//그림자 ------------------------------------------
	if (m_Shadow.m_ShadowRT.Begin(m_pContext, shadow))
	{
		//라이트 방향에서 캡쳐
		m_Terrian.SetMatrix(&m_Terrian.m_matWorld,&m_Light.m_matView, &m_Light.m_matProj);
		//쉐이더 셰이더로 교체
		m_Terrian.PreRender(m_pContext);
		m_pContext->PSSetShader(m_Shadow.m_pPSShadow->m_pPixelShader.Get(), NULL, 0);
		m_Terrian.PostRender(m_pContext,
			m_Terrian.m_iNumIndex);//
		//오브젝트
		for (auto obj : m_Terrian_Space.m_ObjectList)
		{
			obj.get()->obj_pObject->SetMatrix(&obj.get()->obj_matWorld, &m_Light.m_matView, &m_Light.m_matProj);
			obj.get()->obj_pObject->PreRender(m_pContext);
			m_pContext->PSSetShader(m_Shadow.m_pPSShadow->m_pPixelShader.Get(), NULL, 0);
			obj.get()->obj_pObject->PostRender(m_pContext,
				obj.get()->obj_pObject->m_iNumIndex);
		}
		for (int iObj = 0; iObj < m_FbxLoader.m_ObjectList.size(); iObj++)
		{
			m_FbxLoader.m_ObjectList[iObj]->SetMatrix(&m_FbxLoader.m_ObjectList[iObj]->m_matWorld, &m_Light.m_matView, &m_Light.m_matProj);
			m_FbxLoader.m_ObjectList[iObj]->PreRender(m_pContext);
			m_pContext->PSSetShader(m_Shadow.m_pPSShadow->m_pPixelShader.Get(), NULL, 0);
			m_FbxLoader.m_ObjectList[iObj]->PostRender(m_pContext,
				m_FbxLoader.m_ObjectList[iObj]->m_iNumIndex);//
			
		}
		//복원 작업
		m_Shadow.m_ShadowRT.End(m_pContext);
	}
	m_Shadow.m_Shadow_cbData.m_matShadow = m_Shadow.m_Shadow_cbData.m_matShadow.Transpose();
	m_pContext->UpdateSubresource(
		m_Shadow.m_pShadowCB.Get(), 0, NULL, &m_Shadow.m_Shadow_cbData, 0, 0);
	//상수버퍼 2번째 슬롯에 넣음
	m_pContext->VSSetConstantBuffers(2, 1, m_Shadow.m_pShadowCB.GetAddressOf());
	//샘플러 상태 : 클램프 -> 그림자용
	ApplySS(m_pContext, KState::g_pClampSS, 1);
	D3DKMatrixInverse(&m_Terrian.m_cbData.matNormal, NULL,
		&m_Terrian.m_matWorld);
	
	//지형 렌더------------------------------------------------------------
	m_Terrian.SetMatrix(nullptr, &m_Camera.m_matView, &m_Camera.m_matProj);
	m_Terrian.m_cbData.vLightColor = { m_Light.m_vLightColor.x,m_Light.m_vLightColor.y,m_Light.m_vLightColor.z,1.0f };
	m_Terrian.m_cbData.vLightPos =   { m_Light.m_vPos.x,m_Light.m_vPos.y,m_Light.m_vPos.z};
	m_Terrian.m_cbData.vCamPos = { m_Camera.GetCameraPos()->x, m_Camera.GetCameraPos()->y, m_Camera.GetCameraPos()->z, 1.0f };
	m_pContext->PSSetShaderResources(3, 1, m_Shadow.m_ShadowRT.m_pTextureSRV.GetAddressOf());
	m_Terrian_Space.Render(m_pContext);

	//지형 오브젝트 렌더-----------------------------------------------------
	for (auto obj : m_Terrian_Space.m_ObjectList)
	{
		D3DKMatrixInverse(&obj->obj_pObject->m_cbData.matNormal, NULL,
			&obj->obj_matWorld);
	}
	m_pContext->PSSetShaderResources(3, 1, m_Shadow.m_ShadowRT.m_pTextureSRV.GetAddressOf());
	m_Terrian_Space.Render_MapObject(m_pContext);

	m_Terrian_Space.ImGuiRender(m_pContext);

	//FBX Render------------------------------------------
	for (int iObj = 0; iObj < m_FbxLoader.m_ObjectList.size(); iObj++)
	{
		D3DKMatrixInverse(&m_FbxLoader.m_ObjectList[iObj]->m_cbData.matNormal, NULL,
			&m_FbxLoader.m_ObjectList[iObj]->m_matWorld);
		m_FbxLoader.m_ObjectList[iObj]->m_cbData.vLightColor = { m_Light.m_vLightColor.x,m_Light.m_vLightColor.y,m_Light.m_vLightColor.z,1.0f };
		m_FbxLoader.m_ObjectList[iObj]->m_cbData.vLightPos = { m_Light.m_vPos.x,m_Light.m_vPos.y,m_Light.m_vPos.z };
		m_FbxLoader.m_ObjectList[iObj]->m_cbData.vCamPos = { m_Camera.GetCameraPos()->x , m_Camera.GetCameraPos()->y, m_Camera.GetCameraPos()->z, 1.0f };

		m_FbxLoader.m_ObjectList[iObj]->SetMatrix(&m_FbxLoader.m_ObjectList[iObj]->m_matWorld, &m_Camera.m_matView, &m_Camera.m_matProj);
		m_pContext->PSSetShaderResources(3, 1, m_Shadow.m_ShadowRT.m_pTextureSRV.GetAddressOf());
		m_FbxLoader.m_ObjectList[iObj]->Render(m_pContext);
	}

	//미니맵------------------------------------------------
	m_MiniMap_DebugShadow.SetMatrix(nullptr, nullptr, nullptr);
	m_MiniMap_DebugShadow.PreRender(m_pContext);
	m_pContext->PSSetShaderResources(0, 1, m_Shadow.m_ShadowRT.m_pTextureSRV.GetAddressOf());
	m_MiniMap_DebugShadow.PostRender(m_pContext, m_MiniMap_DebugShadow.m_iNumIndex);

	float color[4] = { 0.2f,0.2f,0.2f,1.0f };
	if (m_MiniMap_DebugCamera.m_Rt.Begin(m_pContext, color))
	{
		ApplyBS(m_pContext, KState::g_pAlphaBlendState);
		m_Terrian.SetMatrix(nullptr, &m_TopView.m_matView, &m_TopView.m_matProj);
		m_Terrian_Space.Render(m_pContext);
		m_Camera.SetMatrix(nullptr, &m_TopView.m_matView,
			&m_TopView.m_matProj);
		m_Camera.Render(m_pContext);
		m_MiniMap_DebugCamera.m_Rt.End(m_pContext);
		KState::g_pCurrentBS = KState::g_pBlendState;
	}
	m_MiniMap_DebugCamera.Render(m_pContext);
	
	KScene::Render();
	return true;
}

bool KScene_Maptool::Release()
{
	m_FbxLoader.Release();
	m_SkyBox.Release();
	m_Terrian_Space.Release();
	m_Terrian.Release();
	m_Camera.Release();
	m_TopView.Release();
	m_MiniMap_DebugCamera.Release();
	m_MiniMap_DebugShadow.Release();
	m_MousePicker.Release();

	KScene::Release();
	return true;
}
