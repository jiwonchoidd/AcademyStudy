#include "Sample.h"
#include "TSys.h"

string data = "VS_OUTPUT VS(VS_INPUT input){VS_OUTPUT output = (VS_OUTPUT)0;	float4 pos = float4(input.p, 1.0f);	float4 world = mul(pos, g_matWorld);	float4 view = mul(world, g_matView);	float4 proj = mul(view, g_matProj);	output.p = proj;	output.t = input.t;	output.c = input.c;	return output;}";
bool	Sample::Init()
{
	HRESULT hr = S_OK;
	SAFE_NEW(m_pLineObj, TLineShape);
	if (m_pLineObj)
	{
		m_pLineObj->Create(GetDevice(),
			GetContext(),
			L"../../../data/lobby.bmp", L"Shape.vsh", L"Shape.psh");
		D3DXMatrixIdentity(&m_matWorldLine);
	}

	for (int iObj = 0; iObj < 3; iObj++)
	{
		SAFE_NEW(m_pBoxObj[iObj], TBoxShape);
		if (m_pBoxObj[iObj])
		{
			m_pBoxObj[iObj]->Create(GetDevice(),
				GetContext(),
				L"../../../data/particle.dds", L"Shape.vsh", L"Shape.psh");
			D3DXMatrixIdentity(&m_matWorldBox[iObj]);
		}
	}

	
	// 월드 단위행렬
	D3DXMatrixIdentity(&m_cbVsPerObject.m_matWorld);
	// 뷰 행렬
	m_Camera.Init();
	m_Camera.SetViewMatrix( D3DXVECTOR3(0, 0, -50.0f), 
							D3DXVECTOR3(0, 0, 1.0f));
	m_Camera.SetProjMatrix((float)D3DX_PI * 0.25f, 
		g_rtClient.right / (FLOAT)g_rtClient.bottom, 0.1f, 1000.0f);

	D3D11_BUFFER_DESC bd;
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	bd.MiscFlags = 0;
	bd.ByteWidth = sizeof(CB_VS_PER_OBJECT);
	if (FAILED(hr = GetDevice()->CreateBuffer(&bd, NULL, 
								&m_pcbVSPerObject)))
	{
		return false;
	}

	RECT rt[3];
	rt[0].left = 0; rt[0].top = 0;	rt[0].right = 800;	rt[0].bottom = 600;
	rt[1].left = 0; rt[1].top = 0;	rt[1].right = 800;	rt[1].bottom = 600;
	//rt[1].left = 100; rt[1].top = 100;	rt[1].right = 500;	rt[1].bottom = 300;
	rt[2].left = 300; rt[2].top = 400;	rt[2].right = 500;	rt[2].bottom = 500;

	TCHAR* filename[] =
	{
		L"../../../data/lobby.bmp",
		L"../../../data/hud.dds",
		L"../../../data/be_a2.dds",
		L"../../../data/sidealpha.dds",
	};
	
	T_STR_ARRAY    filelist;
	for (int itex = 0; itex < 10; itex++)
	{
		TCHAR loadfile[50] = { 0, };
		_stprintf_s(loadfile, L"../../../data/SHINE%d.BMP", itex);
		filelist.push_back(loadfile);
	}	
	m_Object[0].Create(GetDevice(),
		GetContext(),
		rt[0], filename[0], L"Sprite.vsh", L"Sprite.psh");

	// 텍스쳐 교체 에니메이션
	m_Object[1].Create(GetDevice(),
		GetContext(),
		rt[1], 0, L"Sprite.vsh", L"Sprite.psh");
	m_Object[1].SetTexArray(filelist);
	m_Object[1].SetLifeTime(1.0f, true);

	
	m_Object[2].Create(GetDevice(),
		GetContext(),
		rt[2], filename[1], L"Sprite.vsh", L"Sprite.psh");
	
	GetContext()->IASetPrimitiveTopology(
		D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	return true;
}
bool	Sample::Render()
{
	HRESULT hr;
	//GetContext()->RSSetState(TDxState::g_pRSNoneCullSolid);
	GetContext()->IASetPrimitiveTopology(
		D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	GetContext()->PSSetSamplers(0, 1, &TDxState::g_pTexSS);
	GetContext()->OMSetBlendState(TDxState::g_pAlphaBlend, 0, -1);
	GetContext()->VSSetConstantBuffers(1, 0, NULL);

	for (int iObj = 0; iObj < 3; iObj++)
	{
		m_pBoxObj[iObj]->SetMatrix(m_matWorldBox[iObj],
			m_Camera.m_matView,
			m_Camera.m_matProj);

		GetContext()->RSSetState(TDxState::g_pRSFrontCullSolid);
		m_pBoxObj[iObj]->Render();
		GetContext()->RSSetState(TDxState::g_pRSSolid);
		m_pBoxObj[iObj]->Render();
	}
	//
	GetContext()->OMSetBlendState(TDxState::g_pAlphaBlend, 0, -1);
	m_pLineObj->SetMatrix(m_matWorldLine,
		m_Camera.m_matView,
		m_Camera.m_matProj);
	m_pLineObj->Render();

	/*GetContext()->IASetPrimitiveTopology(
		D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	GetContext()->OMSetBlendState(TDxState::g_pAlphaBlend, 0, -1);
	GetContext()->VSSetConstantBuffers(0, 0, NULL);
	GetContext()->VSSetConstantBuffers(1, 1, &m_pcbVSPerObject);
	m_Object[0].Render();

	GetContext()->OMSetBlendState(TDxState::g_pColorBlend, 0, -1);
	m_Object[1].Render();

	GetContext()->OMSetBlendState(TDxState::g_pAlphaBlend, 0, -1);
	m_Object[2].Render();
*/

	/*for (int iObj = 0; iObj < MAX_OBJECT; iObj++)
	{		
		m_Object[iObj].Render();
	}*/
	return true;
}
bool    Sample::Frame()
{
	HRESULT hr;
	// 월드 행렬 업데이트
	float fTime = m_Timer.m_fAccumulation;	
	D3DXMatrixIdentity(&m_cbVsPerObject.m_matWorld);
	
	D3DXMATRIX matTrans[3];
	D3DXMATRIX matScale[3];
	D3DXMATRIX matRotY,matRotX, matRotZ;
	D3DXMatrixTranslation(&matTrans[0], 0.0f, 0.0f, 0.0f);
	D3DXMatrixTranslation(&matTrans[1], 10.0f, 0.0f, 0.0f);
	D3DXMatrixTranslation(&matTrans[2], 5.0f, 0.0f, 0.0f);
	D3DXMatrixScaling(&matScale[0],5,5,5);
	D3DXMatrixScaling(&matScale[1],2, 2, 2);
	D3DXMatrixScaling(&matScale[2],1, 1, 1);

	D3DXQUATERNION qX, qY, qZ;
	D3DXQuaternionRotationAxis(&qX, &D3DXVECTOR3(1,0,0), fTime);
	D3DXQuaternionRotationAxis(&qY, &D3DXVECTOR3(0,1, 0), fTime*3.0f);
	D3DXQuaternionRotationAxis(&qZ, &D3DXVECTOR3(0, 0, 1), fTime);
	D3DXMatrixRotationQuaternion(&matRotX, &qX);
	D3DXMatrixRotationQuaternion(&matRotY, &qY);
	D3DXMatrixRotationQuaternion(&matRotZ, &qZ);
	//D3DXMatrixRotationX(&matRotX, fTime);
	//D3DXMatrixRotationY(&matRotY, fTime*3.0f);
	//D3DXMatrixRotationZ(&matRotZ, fTime);
	////D3DXMatrixMultiply(&m_matWorldBox,
	////					&matRotation, &matTrans);
	//D3DXMatrixMultiply(&m_matWorldBox,
	//	&matTrans, &matRotation);
	D3DXVECTOR3 vPos(10.0f, 0.0f, 0.0f);
	D3DXVec3TransformCoord(&vPos, &vPos, &matRotY);
	// 태양 = 단위행렬
	// 최상위 행렬
	m_matWorldBox[0] = matScale[0]* matTrans[0];
	// 지구 = 지구 행렬 * 태양행렬
	// 지구 = 지구 행렬 * 부모 행렬(최상위 행렬)
	m_matWorldBox[1] = matScale[1];// *matTrans[1] * matRotY *  matTrans[0];
	m_matWorldBox[1]._41 = vPos.x;
	m_matWorldBox[1]._42 = vPos.y;
	m_matWorldBox[1]._43 = vPos.z;
	// 달 = 달 행렬 * 지구행렬 * 태양 행렬
	// 달 = 달행렬 * 부모 행렬( 지구 * 태양)
	m_matWorldBox[2] = matTrans[2] * matRotY *	
						matTrans[1] * matRotY *  matTrans[0];
	//m_matWorldBox = matScale * matTrans * matRotation;
	//m_matWorldBox = matRotation * matTrans;
	//m_matWorldBox = matTrans * matRotation;f

	//D3DXMatrixMultiply(&m_matWorldBox,
    //&matTrans, &matRotation);
	//m_matWorldBox._41 = 0.0f;
	//m_matWorldBox._42 = 0.0f;
	//m_matWorldBox._43 = 3.0f;

	// 카메라 행렬 업데이트
	if (m_Input.KeyCheck('W') == KEY_HOLD)
	{
		m_Camera.Front();
	}
	if (m_Input.KeyCheck('S') == KEY_HOLD)
	{
		m_Camera.Back();		
	}
	if (m_Input.KeyCheck('A') == KEY_HOLD)
	{
		m_Camera.Left();
	}
	if (m_Input.KeyCheck('D') == KEY_HOLD)
	{
		m_Camera.Right();
	}
	if (m_Input.KeyCheck('Q') == KEY_HOLD)
	{
		m_Camera.Up();
	}
	if (m_Input.KeyCheck('E') == KEY_HOLD)
	{
		m_Camera.Down();
	}

	float fDir = (m_Input.m_sMouseWheelDelta / 120.0f);
	D3DXVECTOR4 vDir(m_Input.m_MovePt.x, 
		m_Input.m_MovePt.y, 
		0, fDir);
	

	m_Camera.Update(vDir);
	m_Camera.Frame();

	m_cbVsPerObject.m_matView = m_Camera.m_matView;
	m_cbVsPerObject.m_matProj = m_Camera.m_matProj;
	// 상수버퍼 갱신
	D3D11_MAPPED_SUBRESOURCE ms;
	GetContext()->Map(    m_pcbVSPerObject, 0,
		                  D3D11_MAP_WRITE_DISCARD, 0,
		                  &ms);
	CB_VS_PER_OBJECT* pData = (CB_VS_PER_OBJECT*)ms.pData;
	D3DXMatrixTranspose(&pData->m_matWorld,
		&m_cbVsPerObject.m_matWorld);
	D3DXMatrixTranspose(&pData->m_matView,
		&m_cbVsPerObject.m_matView);
	D3DXMatrixTranspose(&pData->m_matProj,
		&m_cbVsPerObject.m_matProj);	
	GetContext()->Unmap(m_pcbVSPerObject,0);

	
	for (int iObj = 0; iObj < MAX_OBJECT; iObj++)
	{
		m_Object[iObj].Frame();	
	}
	
	return true;
}
bool	Sample::Release()
{
	SAFE_RELEASE(m_pcbVSPerObject);
	for (int iObj = 0; iObj < MAX_OBJECT; iObj++)
	{		
		SAFE_RELEASE(m_pBoxObj[iObj]);
		SAFE_DEL(m_pBoxObj[iObj]);
		m_Object[iObj].Release();
	}
	SAFE_RELEASE(m_pLineObj);
	SAFE_DEL(m_pLineObj);
	
	
	return true;
}

Sample::Sample()
{
	SAFE_ZERO(m_pLineObj);
	for (int iObj = 0; iObj < MAX_OBJECT; iObj++)
	{	
		SAFE_ZERO(m_pBoxObj[iObj]);
	}
}

Sample::~Sample()
{

}
TCORE_RUN(DxSample_1, 800, 600)