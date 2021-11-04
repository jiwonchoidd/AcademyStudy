#include "Sample.h"
bool		Sample::Init()
{
	//평면투영쉐도우용 플레인을 만든다.
	m_Plane.x = 0.0f;
	m_Plane.y = 1.0f;
	m_Plane.z = 0.0f;
	m_Plane.w = 0.0f;
	m_lightPos.x = 100.0f;
	m_lightPos.y = 100.0f;
	m_lightPos.z = 100.0f;
	m_lightPos.w = 0.0f;
	//그림자 행렬 만들어주는 함수
	D3DKMatrixShadow(&m_matShadow, &m_lightPos, &m_Plane);
	//맵하고 조금 띄어놓기
	m_matShadow._42 = 1.0f;

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
	m_FbxObjB.LoadObject("../../data/object/multiCamera.FBX", "CharacterShader.hlsl");
	
	//map resize
	m_FbxObjB.m_matWorld._11 = 6;
	m_FbxObjB.m_matWorld._22 = 6;
	m_FbxObjB.m_matWorld._33 = 6;

	m_DebugCamera.CreateViewMatrix(KVector3(0, 50, -100), KVector3(0, 0, 0));
	m_DebugCamera.CreateProjMatrix(1.0f, 1000.0f, XM_PI * 2.5f, (float)g_rtClient.right / (float)g_rtClient.bottom);
	return true;
}
float yRot=0;
bool		Sample::Frame()
{
	if (g_Input.GetKey(VK_F4) == KEY_PUSH)
	{
		(m_FbxObjA.m_bAnimPlay) ? m_FbxObjA.m_bAnimPlay = false : m_FbxObjA.m_bAnimPlay = true;
	}
	if (g_Input.GetKey(VK_F4) == KEY_PUSH)
	{
		(m_FbxObjB.m_bAnimPlay) ? m_FbxObjB.m_bAnimPlay = false : m_FbxObjB.m_bAnimPlay = true;
	}
	m_FbxObjA.Frame();
	m_FbxObjB.Frame();

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
		yRot = -(4/ XM_PI);
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
	//일반 오브젝트
	m_FbxObjA.SetMatrix(&m_FbxObjA.m_matWorld, &m_DebugCamera.m_matView, &m_DebugCamera.m_matProj);
	m_FbxObjA.ChangePixelShader(nullptr);
	m_FbxObjA.Render(m_pImmediateContext);
	//쉐도우
	m_FbxObjA.SetMatrix(&m_matShadow, &m_DebugCamera.m_matView, &m_DebugCamera.m_matProj);
	m_FbxObjA.ChangePixelShader(m_pPSShadow);
	m_FbxObjA.Render(m_pImmediateContext);
	//맵
	m_FbxObjB.SetMatrix(&m_FbxObjB.m_matWorld, &m_DebugCamera.m_matView, &m_DebugCamera.m_matProj);
	m_FbxObjB.ChangePixelShader(nullptr);
	m_FbxObjB.Render(m_pImmediateContext);
	return true;
}
bool		Sample::Release()
{
	m_FbxObjA.Release();
	m_FbxObjB.Release();

	SAFE_RELEASE(m_pPSShadow);
	return true;
}
Sample::Sample()
{
}
Sample::~Sample()
{
}
WinMain_OPT(평면 쉐도우, 1200, 675);