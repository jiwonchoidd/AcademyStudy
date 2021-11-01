#include "Sample.h"
bool		Sample::Init()
{
	m_MiniMap.CreateModel(L"../../data/shader/PlaneVS.txt",
		L"../../data/shader/PlanePS.txt", L"");
	m_MapObj.CreateModel(L"../../data/shader/PlaneVS.txt",
		L"../../data/shader/PlanePS.txt", L"../../data/charport.bmp");
	KMatrix matWorld, matScale;
	D3DKMatrixRotationX(&matWorld, XM_PI / 2.0f);
	D3DKMatrixScaling(&matScale, 1000.0f, 1000.0f, 1000.0f);
	m_MapObj.m_matWorld = matScale * matWorld;
	m_Rt.Create(4096, 4096);
	KPlane p;
	p.x = 0.0f;
	p.y = 1.0f;
	p.z = 0.0f;
	p.w = -0.1f;
	KVector4 v;
	v.x = 100.0f;
	v.y = 100.0f;
	v.z = 100.0f;
	v.w = 0.0f;
	D3DKMatrixShadow(&m_matShadow, &v, &p);

	ID3DBlob* PSBlob = nullptr;
	PSBlob = KModel::LoadShaderBlob(L"CharacterShader.hlsl",
		"PSShadow", "ps_5_0");

	if (PSBlob != nullptr)
	{
		HRESULT hr = S_OK;
		hr = g_pd3dDevice->CreatePixelShader(
			PSBlob->GetBufferPointer(),
			PSBlob->GetBufferSize(),
			NULL, &m_pPSShadow);
		if (FAILED(hr)) return hr;
		PSBlob->Release();
	}

	m_FbxObjA.LoadObject("../../data/object/Man.fbx", "CharacterShader.hlsl");
	//m_FbxObjB.LoadObject("../../data/object/multiCamera.FBX", "CharacterShader.hlsl");
	////map resize
	//m_FbxObjB.m_matWorld._11 = 6;
	//m_FbxObjB.m_matWorld._22 = 6;
	//m_FbxObjB.m_matWorld._33 = 6;

	m_DebugCamera.CreateViewMatrix(KVector3(0, 50, -100), KVector3(0, 0, 0));
	m_DebugCamera.CreateProjMatrix(1.0f, 1000.0f, XM_PI * 2.5f, (float)g_rtClient.right / (float)g_rtClient.bottom);
	return true;
}
float yRot = 0;
bool		Sample::Frame()
{
	if (g_Input.GetKey(VK_F4) == KEY_PUSH)
	{
		(m_FbxObjA.m_bAnimPlay) ? m_FbxObjA.m_bAnimPlay = false : m_FbxObjA.m_bAnimPlay = true;
	}
	m_FbxObjA.Frame();

#pragma region 캐릭터 이동
	if (g_Input.GetKey(VK_UP) >= KEY_PUSH)
	{
		yRot = XM_PI;
		m_MovePos.z += g_fSecPerFrame * 100.0f;
		m_matShadow._43 += g_fSecPerFrame * 10000.0f;
		m_FbxObjA.m_matWorld._41 = m_MovePos.x;
		m_FbxObjA.m_matWorld._42 = m_MovePos.y;
		m_FbxObjA.m_matWorld._43 = m_MovePos.z;
		m_FbxObjA.SetMatrix(&m_matShadow, &m_DebugCamera.m_matView, &m_DebugCamera.m_matProj);
	}
	if (g_Input.GetKey(VK_DOWN) >= KEY_PUSH)
	{
		yRot = 0.0f;
		m_MovePos.z -= g_fSecPerFrame * 100.0f;
		m_matShadow._43 -= g_fSecPerFrame * 10000.0f;
		m_FbxObjA.m_matWorld._41 = m_MovePos.x;
		m_FbxObjA.m_matWorld._42 = m_MovePos.y;
		m_FbxObjA.m_matWorld._43 = m_MovePos.z;
		m_FbxObjA.SetMatrix(&m_matShadow, &m_DebugCamera.m_matView, &m_DebugCamera.m_matProj);
	}
	if (g_Input.GetKey(VK_RIGHT) >= KEY_PUSH)
	{
		yRot = -(4 / XM_PI);
		m_MovePos.x += g_fSecPerFrame * 100.0f;
		m_matShadow._41 += g_fSecPerFrame * 10000.0f;
		m_FbxObjA.m_matWorld._41 = m_MovePos.x;
		m_FbxObjA.m_matWorld._42 = m_MovePos.y;
		m_FbxObjA.m_matWorld._43 = m_MovePos.z;
		m_FbxObjA.SetMatrix(&m_matShadow, &m_DebugCamera.m_matView, &m_DebugCamera.m_matProj);
	}
	if (g_Input.GetKey(VK_LEFT) >= KEY_PUSH)
	{
		yRot = 4 / XM_PI;
		m_MovePos.x -= g_fSecPerFrame * 100.0f;
		m_matShadow._41 -= g_fSecPerFrame * 10000.0f;
		m_FbxObjA.m_matWorld._41 = m_MovePos.x;
		m_FbxObjA.m_matWorld._42 = m_MovePos.y;
		m_FbxObjA.m_matWorld._43 = m_MovePos.z;
		m_FbxObjA.SetMatrix(&m_matShadow, &m_DebugCamera.m_matView, &m_DebugCamera.m_matProj);
	}
#pragma endregion

#pragma region 캐릭터 회전
	if (g_Input.GetKey(VK_UP) || g_Input.GetKey(VK_DOWN) || g_Input.GetKey(VK_RIGHT) || g_Input.GetKey(VK_LEFT))
	{
		m_FbxObjA.m_matWorld._11 = cos(yRot);
		m_FbxObjA.m_matWorld._13 = -sin(yRot);
		m_FbxObjA.m_matWorld._31 = sin(yRot);
		m_FbxObjA.m_matWorld._33 = cos(yRot);
		m_matShadow._11 = cos(yRot) * 100;
		m_matShadow._13 = -sin(yRot) * 100;
		m_matShadow._31 = sin(yRot) * 100;
		m_matShadow._33 = cos(yRot) * 100;
		m_FbxObjA.SetMatrix(&m_matShadow, &m_DebugCamera.m_matView, &m_DebugCamera.m_matProj);
	}
#pragma endregion
	return true;
}
bool		Sample::Render()
{
	//렌더타겟 객체 저장해놓고 나중에 end로 복원
	if (m_Rt.Begin(m_pImmediateContext))
	{
		m_FbxObjA.SetMatrix(&m_FbxObjA.m_matWorld,
			&m_DebugCamera.m_matView, &m_DebugCamera.m_matProj);
		m_FbxObjA.ChangePixelShader(m_pPSShadow);
		m_FbxObjA.Render(m_pImmediateContext);
		m_Rt.End(m_pImmediateContext);
	}

	m_MapObj.SetMatrix(&m_MapObj.m_matWorld,
		&m_DebugCamera.m_matView, &m_DebugCamera.m_matProj);
	m_MapObj.Render(m_pImmediateContext);

	m_MiniMap.SetMatrix(nullptr, nullptr, nullptr);
	m_MiniMap.PreRender(m_pImmediateContext);
	m_pImmediateContext->PSSetShaderResources(
		0, 1, &m_Rt.m_pTextureSRV);
	m_MiniMap.PostRender(m_pImmediateContext,
		m_MiniMap.m_iNumIndex);

	m_FbxObjA.SetMatrix(&m_FbxObjA.m_matWorld, 
		&m_DebugCamera.m_matView, &m_DebugCamera.m_matProj);
	m_FbxObjA.ChangePixelShader(nullptr);
	m_FbxObjA.Render(m_pImmediateContext);

	if (g_Input.GetKey(VK_F5) == KEY_PUSH)
	{
		m_Rt.Save(m_pImmediateContext, L"frame.jpg");
	}
	return true;
}
bool		Sample::Release()
{
	m_MapObj.Release();
	m_MiniMap.Release();
	m_Rt.Release();
	m_FbxObjA.Release();
	//m_FbxObjB.Release();
	SAFE_RELEASE(m_pPSShadow);
	return true;
}
Sample::Sample()
{
}
Sample::~Sample()
{
}
WinMain_OPT(FBX애니메이션, 1200, 675);