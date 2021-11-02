#include "Sample.h"
bool		Sample::Init()
{
	//�ؽ��� ��ȯ��� �ؽ���
	m_matTex._11 = 0.5f; m_matTex._22 = -0.5f;
	m_matTex._41 = 0.5f; m_matTex._42 = 0.5f;
	//����Ʈ ����
	m_Light1.SetLight(KVector3(100, 200, 80), KVector3(0, 0, 0));

	m_MiniMap.CreateModel(L"../../data/shader/PlaneVS.txt",
		L"../../data/shader/PlanePS.txt", L"");
	m_MapObj.CreateModel(L"ProjectionShadow.hlsl",
		L"ProjectionShadow.hlsl", L"../../data/object/texture_grass.jpg");
	KMatrix matWorld, matScale;

	//��ũ�⸦ Ű��
	D3DKMatrixRotationX(&matWorld, XM_PI / 2.0f);
	D3DKMatrixScaling(&matScale, 100.0f, 100.0f, 100.0f);
	m_MapObj.m_matWorld = matScale * matWorld;
	//�׸��� ����Ÿ�� �̹��� 
	m_Rt.Create(4096, 4096);

	//��� ����
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

	m_DebugCamera.CreateViewMatrix(KVector3(0, 50, -100), KVector3(0, 0, 0));
	m_DebugCamera.CreateProjMatrix(1.0f, 1000.0f, XM_PI * 2.5f, (float)g_rtClient.right / (float)g_rtClient.bottom);
	return true;
}

bool		Sample::Frame()
{
	if (g_Input.GetKey(VK_F4) == KEY_PUSH)
	{
		(m_FbxObjA.m_bAnimPlay) ? m_FbxObjA.m_bAnimPlay = false : m_FbxObjA.m_bAnimPlay = true;
	}
	m_FbxObjA.Frame();
	m_Light1.Frame();
	//������ �����, �������� ���, �ؽ������ ���Ѱ�
	m_ShadowCB.g_matShadow1 =m_Light1.m_matView * m_Light1.m_matProj * m_matTex;

	#pragma region ĳ���� �̵� & ȸ�� & �ִϸ��̼�

	if (g_Input.GetKey(VK_RIGHT))
	{
		m_fYRot += 3 * g_fSecPerFrame;
	}
	if (g_Input.GetKey(VK_LEFT))
	{
		m_fYRot -= 3 * g_fSecPerFrame;
	}
	m_FbxObjA.m_matWorld._11 = cos(m_fYRot);
	m_FbxObjA.m_matWorld._13 = -sin(m_fYRot);
	m_FbxObjA.m_matWorld._31 = sin(m_fYRot);
	m_FbxObjA.m_matWorld._33 = cos(m_fYRot);
	//look vector
	//_31,_32,_33 ������� �ϰ� ����ȭ
	m_FbxObjA.m_vLook.x = m_FbxObjA.m_matWorld._31;
	m_FbxObjA.m_vLook.y = m_FbxObjA.m_matWorld._32;
	m_FbxObjA.m_vLook.z = m_FbxObjA.m_matWorld._33;

	if (g_Input.GetKey(VK_UP) >= KEY_PUSH)
	{
		m_MovePos -= m_FbxObjA.m_vLook *g_fSecPerFrame * 120.0f;
		//�ִϸ��̼�
		m_FbxObjA.m_fElpaseTime += 1.0f * g_fSecPerFrame;
		m_FbxObjA.m_iAnimIndex = (m_FbxObjA.m_fElpaseTime+3.1f) * 30.0f;
		if (m_FbxObjA.m_fElpaseTime > 0.8f)
		{
			m_FbxObjA.m_fElpaseTime = 0.0f;
		}
	}
	else if (g_Input.GetKey(VK_DOWN) >= KEY_PUSH)
	{
		m_MovePos += m_FbxObjA.m_vLook * g_fSecPerFrame * 120.0f;
		//�ִϸ��̼�
		m_FbxObjA.m_fElpaseTime += 1.0f * g_fSecPerFrame;
		m_FbxObjA.m_iAnimIndex = (m_FbxObjA.m_fElpaseTime + 3.1f) * 30.0f;
		if (m_FbxObjA.m_fElpaseTime > 0.8f)
		{
			m_FbxObjA.m_fElpaseTime = 0.0f;
		}
	}
	else
	{
		//Default �ִϸ��̼�
		m_FbxObjA.m_fElpaseTime += 1.0f * g_fSecPerFrame;
		m_FbxObjA.m_iAnimIndex = (m_FbxObjA.m_fElpaseTime) * 30.0f;
		if (m_FbxObjA.m_fElpaseTime > 2.0f)
		{
			m_FbxObjA.m_fElpaseTime = 0.0f;
		}
	}
	m_FbxObjA.m_matWorld._41 = m_MovePos.x;
	m_FbxObjA.m_matWorld._42 = m_MovePos.y;
	m_FbxObjA.m_matWorld._43 = m_MovePos.z;
	#pragma endregion

	return true;
}
bool		Sample::Render()
{
	//�����Ͷ����� �ָ���
	//ApplyRS(m_pImmediateContext, KDXState::g_pRSSolid);

	//����Ÿ�� ��ü �����س��� ���߿� end�� ����
	// �׸���
	if (m_Rt.Begin(m_pImmediateContext))
	{
		m_FbxObjA.SetMatrix(&m_FbxObjA.m_matWorld,
			&m_Light1.m_matView, &m_Light1.m_matProj);
		m_FbxObjA.ChangePixelShader(m_pPSShadow);
		m_FbxObjA.Render(m_pImmediateContext);
		//���� �۾�
		m_Rt.End(m_pImmediateContext);
	}
	//���÷� ���� : Ŭ���� -> �׸��ڿ�
	ApplySS(m_pImmediateContext, KDXState::g_pClampSS, 1);
	m_MapObj.m_cbData.matNormal = m_ShadowCB.g_matShadow1;
	m_MapObj.SetMatrix(&m_MapObj.m_matWorld, &m_DebugCamera.m_matView, &m_DebugCamera.m_matProj);
	//1������ : �׸��ڿ� �ؽ��� ���ҽ� 
	m_pImmediateContext->PSSetShaderResources(1, 1, &m_Rt.m_pTextureSRV);
	m_MapObj.Render(m_pImmediateContext);

	//����׿� �̴ϸ� ����ȴ�.
	m_MiniMap.SetMatrix(nullptr, nullptr, nullptr);
	m_MiniMap.PreRender(m_pImmediateContext);
	//0������ : ����Ʈ �ؽ��� ���ҽ� 
	m_pImmediateContext->PSSetShaderResources(
		0, 1, &m_Rt.m_pTextureSRV);
	m_MiniMap.PostRender(m_pImmediateContext,
		m_MiniMap.m_iNumIndex);
	//

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
WinMain_OPT(FBX�ִϸ��̼�, 1200, 675);