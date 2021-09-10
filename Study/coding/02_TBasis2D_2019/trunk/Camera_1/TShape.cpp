#include "TShape.h"
#include "TTexture.h"
bool TBoxShape::CreateVertexBuffer(
			D3DXVECTOR3 vMan, D3DXVECTOR3 vMin)
{
	m_VertexList.resize(24);	
	m_IndexList.resize(36);
	// FRONT
	m_VertexList[0].p = D3DXVECTOR3(-1.0f, 1.0f, -1.0f);
	m_VertexList[1].p = D3DXVECTOR3(1.0f, 1.0f, -1.0f);
	m_VertexList[2].p = D3DXVECTOR3(-1.0f, -1.0f, -1.0f);
	m_VertexList[3].p = D3DXVECTOR3(1.0f, -1.0f, -1.0f);
	m_VertexList[0].c = D3DXVECTOR4(1.0f, 0.0f, 0.0f, 1.0f);
	m_VertexList[1].c = D3DXVECTOR4(1.0f, 1.0f, 0.0f, 1.0f);
	m_VertexList[2].c = D3DXVECTOR4(1.0f, 0.0f, 1.0f, 1.0f);
	m_VertexList[3].c = D3DXVECTOR4(1.0f, 1.0f, 0.0f, 1.0f);
	m_VertexList[0].t = D3DXVECTOR2(0.0f, 0.0f);
	m_VertexList[1].t = D3DXVECTOR2(1.0f, 0.0f);
	m_VertexList[2].t = D3DXVECTOR2(0.0f, 1.0f);
	m_VertexList[3].t = D3DXVECTOR2(1.0f, 1.0f);
	m_IndexList[0] = 0; m_IndexList[1] = 1; m_IndexList[2] = 2;
	m_IndexList[3] = 2; m_IndexList[4] = 1; m_IndexList[5] = 3;
	// LEFT
	m_VertexList[4].p = D3DXVECTOR3(1.0f, 1.0f, -1.0f);
	m_VertexList[5].p = D3DXVECTOR3(1.0f, 1.0f,  1.0f);
	m_VertexList[6].p = D3DXVECTOR3(1.0f, -1.0f,-1.0f);
	m_VertexList[7].p = D3DXVECTOR3(1.0f, -1.0f, 1.0f);
	m_VertexList[4].c = D3DXVECTOR4(1.0f, 0.0f, 0.0f, 1.0f);
	m_VertexList[5].c = D3DXVECTOR4(0.0f, 1.0f, 0.0f, 1.0f);
	m_VertexList[6].c = D3DXVECTOR4(0.0f, 0.0f, 1.0f, 1.0f);
	m_VertexList[7].c = D3DXVECTOR4(1.0f, 1.0f, 0.0f, 1.0f);
	m_VertexList[4].t = D3DXVECTOR2(0.0f, 0.0f);
	m_VertexList[5].t = D3DXVECTOR2(1.0f, 0.0f);
	m_VertexList[6].t = D3DXVECTOR2(0.0f, 1.0f);
	m_VertexList[7].t = D3DXVECTOR2(1.0f, 1.0f);
	m_IndexList[6] = 4; m_IndexList[7] = 5; m_IndexList[8] = 6;
	m_IndexList[9] = 6; m_IndexList[10] = 5; m_IndexList[11] = 7;
	// BACK
	m_VertexList[8].p = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	m_VertexList[9].p = D3DXVECTOR3(-1.0f, 1.0f, 1.0f);
	m_VertexList[10].p = D3DXVECTOR3(1.0f, -1.0f,1.0f);
	m_VertexList[11].p = D3DXVECTOR3(-1.0f, -1.0f,1.0f);
	m_VertexList[8].c = D3DXVECTOR4(1.0f, 0.0f, 0.0f, 1.0f);
	m_VertexList[9].c = D3DXVECTOR4(0.0f, 1.0f, 0.0f, 1.0f);
	m_VertexList[10].c = D3DXVECTOR4(0.0f, 0.0f, 1.0f, 1.0f);
	m_VertexList[11].c = D3DXVECTOR4(1.0f, 1.0f, 0.0f, 1.0f);
	m_VertexList[8].t = D3DXVECTOR2(0.0f, 0.0f);
	m_VertexList[9].t = D3DXVECTOR2(1.0f, 0.0f);
	m_VertexList[10].t = D3DXVECTOR2(0.0f, 1.0f);
	m_VertexList[11].t = D3DXVECTOR2(1.0f, 1.0f);
	m_IndexList[12] = 8; m_IndexList[13] = 9; m_IndexList[14] = 10;
	m_IndexList[15] = 10; m_IndexList[16] = 9; m_IndexList[17] = 11;
	// RIGHT
	m_VertexList[12].p = D3DXVECTOR3(-1.0f, 1.0f, 1.0f);
	m_VertexList[13].p = D3DXVECTOR3(-1.0f, 1.0f, -1.0f);
	m_VertexList[14].p = D3DXVECTOR3(-1.0f, -1.0f, 1.0f);
	m_VertexList[15].p = D3DXVECTOR3(-1.0f, -1.0f, -1.0f);
	m_VertexList[12].c = D3DXVECTOR4(1.0f, 0.0f, 0.0f, 1.0f);
	m_VertexList[13].c = D3DXVECTOR4(0.0f, 1.0f, 0.0f, 1.0f);
	m_VertexList[14].c = D3DXVECTOR4(0.0f, 0.0f, 1.0f, 1.0f);
	m_VertexList[15].c = D3DXVECTOR4(1.0f, 1.0f, 0.0f, 1.0f);
	m_VertexList[12].t = D3DXVECTOR2(0.0f, 0.0f);
	m_VertexList[13].t = D3DXVECTOR2(1.0f, 0.0f);
	m_VertexList[14].t = D3DXVECTOR2(0.0f, 1.0f);
	m_VertexList[15].t = D3DXVECTOR2(1.0f, 1.0f);
	m_IndexList[18] = 12; m_IndexList[19] = 13; m_IndexList[20] = 14;
	m_IndexList[21] = 14; m_IndexList[22] = 13; m_IndexList[23] = 15;
	// UP
	m_VertexList[16].p = D3DXVECTOR3(-1.0f, 1.0f, 1.0f);
	m_VertexList[17].p = D3DXVECTOR3(1.0f, 1.0f,  1.0f);
	m_VertexList[18].p = D3DXVECTOR3(-1.0f, 1.0f,-1.0f);
	m_VertexList[19].p = D3DXVECTOR3(1.0f, 1.0f, -1.0f);
	m_VertexList[16].c = D3DXVECTOR4(1.0f, 0.0f, 0.0f, 1.0f);
	m_VertexList[17].c = D3DXVECTOR4(0.0f, 1.0f, 0.0f, 1.0f);
	m_VertexList[18].c = D3DXVECTOR4(0.0f, 0.0f, 1.0f, 1.0f);
	m_VertexList[19].c = D3DXVECTOR4(1.0f, 1.0f, 0.0f, 1.0f);
	m_VertexList[16].t = D3DXVECTOR2(0.0f, 0.0f);
	m_VertexList[17].t = D3DXVECTOR2(1.0f, 0.0f);
	m_VertexList[18].t = D3DXVECTOR2(0.0f, 1.0f);
	m_VertexList[19].t = D3DXVECTOR2(1.0f, 1.0f);
	m_IndexList[24] = 16; m_IndexList[25] = 17; m_IndexList[26] = 18;
	m_IndexList[27] = 18; m_IndexList[28] = 17; m_IndexList[29] = 19;
	// DOWN
	m_VertexList[20].p = D3DXVECTOR3(1.0f, -1.0f, 1.0f);
	m_VertexList[21].p = D3DXVECTOR3(-1.0f, -1.0f, 1.0f);
	m_VertexList[22].p = D3DXVECTOR3(1.0f, -1.0f, -1.0f);
	m_VertexList[23].p = D3DXVECTOR3(-1.0f, -1.0f, -1.0f);
	m_VertexList[20].c = D3DXVECTOR4(1.0f, 0.0f, 0.0f, 1.0f);
	m_VertexList[21].c = D3DXVECTOR4(0.0f, 1.0f, 0.0f, 1.0f);
	m_VertexList[22].c = D3DXVECTOR4(0.0f, 0.0f, 1.0f, 1.0f);
	m_VertexList[23].c = D3DXVECTOR4(1.0f, 1.0f, 0.0f, 1.0f);
	m_VertexList[20].t = D3DXVECTOR2(0.0f, 0.0f);
	m_VertexList[21].t = D3DXVECTOR2(1.0f, 0.0f);
	m_VertexList[22].t = D3DXVECTOR2(0.0f, 1.0f);
	m_VertexList[23].t = D3DXVECTOR2(1.0f, 1.0f);
	m_IndexList[30] = 20; m_IndexList[31] = 21; m_IndexList[32] = 22;
	m_IndexList[33] = 22; m_IndexList[34] = 21; m_IndexList[35] = 23;
	return true;
}
bool TBoxShape::Create(ID3D11Device* pd3dDevice,
	ID3D11DeviceContext*	pImmediateContext,
	const TCHAR* pTextureName,
	const TCHAR* pVShaderName,
	const TCHAR* pPShaderName)
{
	HRESULT hr;
	m_pd3dDevice = pd3dDevice;
	m_pImmediateContext = pImmediateContext;

	CreateVertexBuffer( D3DXVECTOR3( -1.0f, -1.0f, -1.0f),
						D3DXVECTOR3(1.0f, 1.0f, 1.0f));

	// (1)정점버퍼 생성
	m_VB.Create(pd3dDevice, D3D11_USAGE_DEFAULT,
		D3D11_BIND_VERTEX_BUFFER, sizeof(PCT_VERTEX),
		m_VertexList.size(),
		&m_VertexList.at(0));
	m_IB.Create(pd3dDevice, D3D11_USAGE_DEFAULT,
		D3D11_BIND_INDEX_BUFFER, sizeof(DWORD),
		m_IndexList.size(),
		&m_IndexList.at(0));

	// 상수 버퍼
	m_CB.Create(pd3dDevice, D3D11_USAGE_DYNAMIC,
		D3D11_BIND_CONSTANT_BUFFER, sizeof(CB_VS_PER_OBJECT), 1,
		NULL);

	// (2)정점 및 픽쉘 쉐이더 컴파일
	m_iVS = I_Shader.Load(pd3dDevice, pVShaderName, "VS", "vs_5_0", t_VS);
	m_iPS = I_Shader.Load(pd3dDevice, pPShaderName, "PS", "ps_5_0", t_PS);
	m_pVS = I_Shader.GetPtr(m_iVS);
	m_pPS = I_Shader.GetPtr(m_iPS);

	// (3) 정점 레이아웃( 정점성분의 구조도 )
	D3D11_INPUT_ELEMENT_DESC ied[] = {
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 28, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	int iNumElement = sizeof(ied) / sizeof(ied[0]);
	hr = pd3dDevice->CreateInputLayout(ied, iNumElement,
		m_pVS->m_pBlob->GetBufferPointer(),
		m_pVS->m_pBlob->GetBufferSize(),
		&m_pIL);
	if (FAILED(hr))
	{
		return false;
	}


	if (pTextureName != NULL)
	{
		m_iTexIndex = I_Texture.Load(pd3dDevice, pTextureName);
	}
	return true;
}
bool TBoxShape::Render()
{
	HRESULT hr;
	I_Texture.GetPtr(m_iTexIndex)->Apply(m_pImmediateContext);
	m_CB.Apply(m_pImmediateContext);
	m_pImmediateContext->IASetInputLayout(m_pIL);
	m_pVS->ApplyVS(m_pImmediateContext);
	m_pPS->ApplyPS(m_pImmediateContext);
	m_VB.Apply(m_pImmediateContext);
	m_IB.Apply(m_pImmediateContext);
	m_pImmediateContext->DrawIndexed(m_IndexList.size(), 0, 0);
	return true;
}
bool	TBoxShape::Release()
{
	SAFE_RELEASE(m_pIL);
	m_VB.Release();
	m_IB.Release();
	return true;
}
TBoxShape::TBoxShape()
{
}

TBoxShape::~TBoxShape()
{
}
bool TPlaneShape::Create(ID3D11Device* pd3dDevice,
	ID3D11DeviceContext*	pImmediateContext,
	const TCHAR* pTextureName,
	const TCHAR* pVShaderName,
	const TCHAR* pPShaderName)
{
	HRESULT hr;
	m_pd3dDevice = pd3dDevice;
	m_pImmediateContext = pImmediateContext;


	m_VertexList.resize(4);
	m_VertexList[0].p = D3DXVECTOR3(-1.0f, 1.0f, 0.0f);
	m_VertexList[1].p = D3DXVECTOR3( 1.0f, 1.0f, 0.0f);
	m_VertexList[2].p = D3DXVECTOR3(-1.0f, -1.0f, 0.0f);
	m_VertexList[3].p = D3DXVECTOR3( 1.0f, -1.0f, 0.0f);

	m_VertexList[0].c = D3DXVECTOR4(1.0f, 0.0f, 0.0f, 1.0f);
	m_VertexList[1].c = D3DXVECTOR4(0.0f, 1.0f, 0.0f, 1.0f);
	m_VertexList[2].c = D3DXVECTOR4(0.0f, 0.0f, 1.0f, 1.0f);
	m_VertexList[3].c = D3DXVECTOR4(1.0f, 1.0f, 0.0f, 1.0f);

	m_VertexList[0].t = D3DXVECTOR2(0.0f, 0.0f);
	m_VertexList[1].t = D3DXVECTOR2(1.0f, 0.0f);
	m_VertexList[2].t = D3DXVECTOR2(0.0f, 1.0f);
	m_VertexList[3].t = D3DXVECTOR2(1.0f, 1.0f);

	m_IndexList.resize(6);
	m_IndexList[0] = 0; m_IndexList[1] = 1; m_IndexList[2] = 2;
	m_IndexList[3] = 2; m_IndexList[4] = 1; m_IndexList[5] = 3;

	// (1)정점버퍼 생성
	m_VB.Create(pd3dDevice, D3D11_USAGE_DEFAULT,
		D3D11_BIND_VERTEX_BUFFER, sizeof(PCT_VERTEX),
		m_VertexList.size(),
		&m_VertexList.at(0));
	m_IB.Create(pd3dDevice, D3D11_USAGE_DEFAULT,
		D3D11_BIND_INDEX_BUFFER, sizeof(DWORD), 
		m_IndexList.size(),
		&m_IndexList.at(0));

	// 상수 버퍼
	m_CB.Create(pd3dDevice, D3D11_USAGE_DYNAMIC,
		D3D11_BIND_CONSTANT_BUFFER, sizeof(CB_VS_PER_OBJECT), 1,
		NULL);

	// (2)정점 및 픽쉘 쉐이더 컴파일
	m_iVS = I_Shader.Load(pd3dDevice, pVShaderName, "VS", "vs_5_0", t_VS);
	m_iPS = I_Shader.Load(pd3dDevice, pPShaderName, "PS", "ps_5_0", t_PS);
	m_pVS = I_Shader.GetPtr(m_iVS);
	m_pPS = I_Shader.GetPtr(m_iPS);

	// (3) 정점 레이아웃( 정점성분의 구조도 )
	D3D11_INPUT_ELEMENT_DESC ied[] = {
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 28, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	int iNumElement = sizeof(ied) / sizeof(ied[0]);
	hr = pd3dDevice->CreateInputLayout(ied, iNumElement,
		m_pVS->m_pBlob->GetBufferPointer(),
		m_pVS->m_pBlob->GetBufferSize(),
		&m_pIL);
	if( FAILED(hr))
	{ 
		return false;
	}


	if (pTextureName != NULL)
	{
		m_iTexIndex = I_Texture.Load(pd3dDevice, pTextureName);
	}
	return true;
}
bool TPlaneShape::Render()
{
	HRESULT hr;
	I_Texture.GetPtr(m_iTexIndex)->Apply(m_pImmediateContext);
	m_CB.Apply(m_pImmediateContext);
	m_pImmediateContext->IASetInputLayout(m_pIL);
	m_pVS->ApplyVS(m_pImmediateContext);
	m_pPS->ApplyPS(m_pImmediateContext);
	m_VB.Apply(m_pImmediateContext);
	m_IB.Apply(m_pImmediateContext);
	m_pImmediateContext->DrawIndexed(m_IndexList.size(), 0, 0);
	return true;
}
bool	TPlaneShape::Release()
{
	SAFE_RELEASE(m_pIL);
	m_VB.Release();
	m_IB.Release();
	return true;
}
TPlaneShape::TPlaneShape()
{
}

TPlaneShape::~TPlaneShape()
{
}
bool TLineShape::Create(ID3D11Device* pd3dDevice,
	ID3D11DeviceContext*	pImmediateContext,
	const TCHAR* pTextureName,
	const TCHAR* pVShaderName,
	const TCHAR* pPShaderName)
{
	HRESULT hr;
	m_pd3dDevice = pd3dDevice;
	m_pImmediateContext = pImmediateContext;


	m_VertexList.resize(6);	
	m_VertexList[0].p = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_VertexList[1].p = D3DXVECTOR3(1000.0f, 0.0f, 0.0f);

	m_VertexList[2].p = D3DXVECTOR3(0.0f, .0f, 0.0f);
	m_VertexList[3].p = D3DXVECTOR3(0.0f, 1000.0f, 0.0f);

	m_VertexList[4].p = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_VertexList[5].p = D3DXVECTOR3(0.0f, 0.0f, 1000.0f);

	m_VertexList[0].c = D3DXVECTOR4(1.0f, 0.0f, 0.0f, 1.0f);
	m_VertexList[1].c = D3DXVECTOR4(1.0f, 0.0f, 0.0f, 1.0f);
	m_VertexList[2].c = D3DXVECTOR4(0.0f, 1.0f, 0.0f, 1.0f);
	m_VertexList[3].c = D3DXVECTOR4(0.0f, 1.0f, 0.0f, 1.0f);
	m_VertexList[4].c = D3DXVECTOR4(0.0f, 0.0f, 1.0f, 1.0f);
	m_VertexList[5].c = D3DXVECTOR4(0.0f, 0.0f, 1.0f, 1.0f);

	m_VertexList[0].t = D3DXVECTOR2(0.0f, 0.0f);
	m_VertexList[1].t = D3DXVECTOR2(1.0f, 0.0f);
	m_VertexList[2].t = D3DXVECTOR2(0.0f, 1.0f);
	m_VertexList[3].t = D3DXVECTOR2(1.0f, 1.0f);
	m_VertexList[4].t = D3DXVECTOR2(0.0f, 1.0f);
	m_VertexList[5].t = D3DXVECTOR2(1.0f, 1.0f);

	m_IndexList.resize(6);
	m_IndexList[0] = 0; m_IndexList[1] = 1;
	m_IndexList[2] = 2;	m_IndexList[3] = 3; 
	m_IndexList[4] = 4; m_IndexList[5] = 5;

	// (1)정점버퍼 생성
	m_VB.Create(pd3dDevice, D3D11_USAGE_DEFAULT,
		D3D11_BIND_VERTEX_BUFFER, sizeof(PCT_VERTEX),
		m_VertexList.size(),
		&m_VertexList.at(0));
	m_IB.Create(pd3dDevice, D3D11_USAGE_DEFAULT,
		D3D11_BIND_INDEX_BUFFER, sizeof(DWORD),
		m_IndexList.size(),
		&m_IndexList.at(0));

	// 상수 버퍼
	m_CB.Create(pd3dDevice, D3D11_USAGE_DYNAMIC,
		D3D11_BIND_CONSTANT_BUFFER, sizeof(CB_VS_PER_OBJECT), 1,
		NULL);

	// (2)정점 및 픽쉘 쉐이더 컴파일
	m_iVS = I_Shader.Load(pd3dDevice, pVShaderName, "VS", "vs_5_0", t_VS);
	m_iPS = I_Shader.Load(pd3dDevice, pPShaderName, "PS_Line", "ps_5_0", t_PS);
	m_pVS = I_Shader.GetPtr(m_iVS);
	m_pPS = I_Shader.GetPtr(m_iPS);

	// (3) 정점 레이아웃( 정점성분의 구조도 )
	D3D11_INPUT_ELEMENT_DESC ied[] = {
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 28, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	int iNumElement = sizeof(ied) / sizeof(ied[0]);
	hr = pd3dDevice->CreateInputLayout(ied, iNumElement,
		m_pVS->m_pBlob->GetBufferPointer(),
		m_pVS->m_pBlob->GetBufferSize(),
		&m_pIL);
	if (FAILED(hr))
	{
		return false;
	}


	if (pTextureName != NULL)
	{
		m_iTexIndex = I_Texture.Load(pd3dDevice, pTextureName);
	}
	return true;
}
bool TLineShape::Render()
{
	HRESULT hr;
	m_pImmediateContext->IASetPrimitiveTopology(
		D3D_PRIMITIVE_TOPOLOGY_LINELIST);
	I_Texture.GetPtr(m_iTexIndex)->Apply(m_pImmediateContext);
	m_CB.Apply(m_pImmediateContext);
	m_pImmediateContext->IASetInputLayout(m_pIL);
	m_pVS->ApplyVS(m_pImmediateContext);
	m_pPS->ApplyPS(m_pImmediateContext);
	m_VB.Apply(m_pImmediateContext);
	m_IB.Apply(m_pImmediateContext);
	m_pImmediateContext->DrawIndexed(m_IndexList.size(), 0, 0);
	return true;
}
bool	TLineShape::Release()
{
	SAFE_RELEASE(m_pIL);
	m_VB.Release();
	m_IB.Release();
	return true;
}
TLineShape::TLineShape()
{
}


TLineShape::~TLineShape()
{
}

void TShape::SetMatrix(D3DXMATRIX matWorld,
	D3DXMATRIX matView,
	D3DXMATRIX matProj)
{
	m_ConstantList.m_matWorld = matWorld;
	m_ConstantList.m_matView = matView;
	m_ConstantList.m_matProj = matProj;

	D3DXMatrixTranspose(&m_ConstantList.m_matWorld,
		&m_ConstantList.m_matWorld);
	D3DXMatrixTranspose(&m_ConstantList.m_matView,
		&m_ConstantList.m_matView);
	D3DXMatrixTranspose(&m_ConstantList.m_matProj,
		&m_ConstantList.m_matProj);

	m_CB.Update(m_pImmediateContext,
		(void*)&m_ConstantList,
		sizeof(CB_VS_PER_OBJECT));
};
TShape::TShape()
{
}


TShape::~TShape()
{
}
