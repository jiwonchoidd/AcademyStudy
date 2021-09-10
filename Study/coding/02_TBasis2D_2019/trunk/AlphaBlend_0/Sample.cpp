#include "Sample.h"
#include "D3DCOMPILER.h"

void    Sample::SetRect(RECT& rt, int iObj)
{
	// 윈도우 좌표 -> 투영좌표로 변환
	float fLeft = (rt.left / 800.0f) * 2.0f + -1.0f;
	float fRight = (rt.right / 800.0f) * 2.0f + -1.0f;
	float fTop = ((rt.top / 600.0f) * 2.0f + -1.0f) * -1.0f;
	float fBottom = ((rt.bottom / 600.0f) * 2.0f + -1.0f) *-1.0f;

	m_Object[iObj].m_VertexList[0].x = fLeft;
	m_Object[iObj].m_VertexList[0].y = fTop;
	m_Object[iObj].m_VertexList[0].z = 0.5f;
	m_Object[iObj].m_VertexList[0].u = 0.0f;
	m_Object[iObj].m_VertexList[0].v = 0.0f;
	m_Object[iObj].m_VertexList[0].r = 1.0f;
	m_Object[iObj].m_VertexList[0].g = 0.5f;
	m_Object[iObj].m_VertexList[0].b = 0.5f;
	m_Object[iObj].m_VertexList[0].a = 1.0f;

	m_Object[iObj].m_VertexList[1].x = fRight;
	m_Object[iObj].m_VertexList[1].y = fTop;
	m_Object[iObj].m_VertexList[1].z = 0.5f;
	m_Object[iObj].m_VertexList[1].u = 1.0f;
	m_Object[iObj].m_VertexList[1].v = 0.0f;
	m_Object[iObj].m_VertexList[1].r = 1.0f;
	m_Object[iObj].m_VertexList[1].g = 0.0f;
	m_Object[iObj].m_VertexList[1].b = 0.0f;
	m_Object[iObj].m_VertexList[1].a = 1.0f;

	m_Object[iObj].m_VertexList[2].x = fLeft;
	m_Object[iObj].m_VertexList[2].y = fBottom;
	m_Object[iObj].m_VertexList[2].z = 0.5f;
	m_Object[iObj].m_VertexList[2].u = 0.0f;
	m_Object[iObj].m_VertexList[2].v = 1.0f;
	m_Object[iObj].m_VertexList[2].r = 0.0f;
	m_Object[iObj].m_VertexList[2].g = 1.0f;
	m_Object[iObj].m_VertexList[2].b = 0.0f;
	m_Object[iObj].m_VertexList[2].a = 1.0f;

	m_Object[iObj].m_VertexList[3].x = fRight;
	m_Object[iObj].m_VertexList[3].y = fBottom;
	m_Object[iObj].m_VertexList[3].z = 0.5f;
	m_Object[iObj].m_VertexList[3].u = 1.0f;
	m_Object[iObj].m_VertexList[3].v = 1.0f;
	m_Object[iObj].m_VertexList[3].r = 0.0f;
	m_Object[iObj].m_VertexList[3].g = 0.0f;
	m_Object[iObj].m_VertexList[3].b = 1.0f;
	m_Object[iObj].m_VertexList[3].a = 1.0f;
}
void    Sample::SetRectUV(RECT& rt, int iObj)
{
	// 윈도우 좌표 -> 투영좌표로 변환
	float fLeft = (rt.left / 800.0f) * 2.0f + -1.0f;
	float fRight = (rt.right / 800.0f) * 2.0f + -1.0f;
	float fTop = ((rt.top / 600.0f) * 2.0f + -1.0f) * -1.0f;
	float fBottom = ((rt.bottom / 600.0f) * 2.0f + -1.0f);
	m_Object[iObj].m_VertexList[0].u = fLeft;
	m_Object[iObj].m_VertexList[0].v = fTop;


	m_Object[iObj].m_VertexList[1].u = fRight;
	m_Object[iObj].m_VertexList[1].v = fTop;


	m_Object[iObj].m_VertexList[2].u = fLeft;
	m_Object[iObj].m_VertexList[2].v = fBottom;


	m_Object[iObj].m_VertexList[3].u = fRight;
	m_Object[iObj].m_VertexList[3].v = fBottom;
}
bool	Sample::Init()
{
	HRESULT hr = S_OK;

	// 뷰포트 설정
	m_vp.TopLeftX = 300; // 가로 시작 지점
	m_vp.TopLeftY = 0; // 세로 시작 지점
	m_vp.Width = m_SwapChainDesc.BufferDesc.Width / 4;
	m_vp.Height = m_SwapChainDesc.BufferDesc.Height / 4;
	m_vp.MinDepth = 0;
	m_vp.MaxDepth = 1;



	D3D11_SAMPLER_DESC samplerDesc;
	ZeroMemory(&samplerDesc, sizeof(D3D11_SAMPLER_DESC));
	// 필터링 방식
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	// 텍스처 주소 지정 방식
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.BorderColor[0] = 1.0f;
	samplerDesc.BorderColor[1] = 0.0f;
	samplerDesc.BorderColor[2] = 0.0f;
	samplerDesc.BorderColor[3] = 1.0f;
	samplerDesc.MaxAnisotropy = 16;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	samplerDesc.MaxLOD = FLT_MAX;
	if (FAILED(hr = GetDevice()->CreateSamplerState(&samplerDesc, &TDxState::g_pTexSS)))
		return hr;

	D3D11_BLEND_DESC blendDesc;
	ZeroMemory(&blendDesc, sizeof(D3D11_BLEND_DESC));
	blendDesc.AlphaToCoverageEnable = FALSE;
	blendDesc.RenderTarget[0].BlendEnable = TRUE;
	blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;

	blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;

	blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	GetDevice()->CreateBlendState(&blendDesc, &g_pAlphaBlend);
	
	blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_BLEND_FACTOR;
	blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_BLEND_FACTOR;

	GetDevice()->CreateBlendState(&blendDesc, &g_pFactorBlend);


	blendDesc.RenderTarget[0].BlendEnable = FALSE;
	GetDevice()->CreateBlendState(&blendDesc, &g_pNoAlphaBlend);

	// DECAL BLENDING ONE, INV_SRC_ALPHA , ADD
	// ADDIVIVE BLENDING ONE, ONE, ADD
	// SUBRACITIVE BLENDING ONE, ONE REV_SUBTRACT
	blendDesc.RenderTarget[0].BlendEnable = TRUE;
	blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_REV_SUBTRACT;
	blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;// DEST_COLOR;// D3D11_BLEND_DEST_COLOR;
	blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_COLOR;// DEST_COLOR;
	GetDevice()->CreateBlendState(&blendDesc, &g_pColorBlend);

	// (1)인덱스 버퍼 생성
	D3D11_BUFFER_DESC bd;
	D3D11_SUBRESOURCE_DATA sd;
	m_IndexList[0] = 0;
	m_IndexList[1] = 1;
	m_IndexList[2] = 2;
	m_IndexList[3] = 2;
	m_IndexList[4] = 1;
	m_IndexList[5] = 3;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;// 용도
	bd.ByteWidth = sizeof(DWORD) * 6;
	ZeroMemory(&sd, sizeof(D3D11_SUBRESOURCE_DATA));
	sd.pSysMem = m_IndexList;
	hr = GetDevice()->CreateBuffer(&bd, &sd, &m_pIB);
	// (2)정점 및 픽쉘 쉐이더 컴파일
	DWORD dwShaderFlags = D3DCOMPILE_DEBUG;
	ID3DBlob* pVSBuffer = 0; // 컴파일한 결과의 스트링
	ID3DBlob* pPSBuffer = 0;
	ID3DBlob* pErrorBuffer = 0;
	//1>  compilation object save succeeded; see E : //\07_TBasisSampleCode\TBasis2DNet\output//\VertexShader.cso
	hr = D3DX11CompileFromFile(
		L"VShader.h",
		NULL, NULL,
		"VS3", "vs_5_0",
		dwShaderFlags,
		NULL, NULL,
		&pVSBuffer, &pErrorBuffer, NULL);
	if (FAILED(hr))
	{
		OutputDebugStringA((char*)pErrorBuffer->GetBufferPointer());
	}
	hr = D3DX11CompileFromFile(
		L"PShader.h",
		NULL, NULL,
		"PS3", "ps_5_0",
		dwShaderFlags,
		NULL, NULL,
		&pPSBuffer, &pErrorBuffer, NULL);
	if (FAILED(hr))
	{
		OutputDebugStringA((char*)pErrorBuffer->GetBufferPointer());
	}

	hr = GetDevice()->CreateVertexShader(
		pVSBuffer->GetBufferPointer(),
		pVSBuffer->GetBufferSize(),
		NULL,
		&m_pVS);

	hr = GetDevice()->CreatePixelShader(
		pPSBuffer->GetBufferPointer(),
		pPSBuffer->GetBufferSize(),
		NULL,
		&m_pPS);
	// (3) 정점 레이아웃( 정점성분의 구조도 )
	D3D11_INPUT_ELEMENT_DESC ied[] = {
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 28, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	int iNumElement = sizeof(ied) / sizeof(ied[0]);
	hr = GetDevice()->CreateInputLayout(ied, iNumElement,
		pVSBuffer->GetBufferPointer(),
		pVSBuffer->GetBufferSize(),
		&TInputLayout::g_pPT_IL);

	if (pVSBuffer != NULL) pVSBuffer->Release();
	if (pPSBuffer != NULL) pPSBuffer->Release();




	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;// 용도
	bd.ByteWidth = sizeof(VS_CONSTANT_BUFFER);
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	hr = GetDevice()->CreateBuffer(&bd, NULL, &m_pCB);

	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;// 용도
	bd.ByteWidth = sizeof(VS_CONSTANT_BUFFER);
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	hr = GetDevice()->CreateBuffer(&bd, NULL, &m_pCBPixel);

	RECT rt;
	rt.left = 100; rt.top = 100; rt.right = 700;	rt.bottom = 500;
	SetRect(rt, 0);
	rt.left = 100; rt.top = 100; rt.right = 700;	rt.bottom = 500;
	SetRect(rt, 1);
	//rt.left = 300; rt.top = 200; rt.right = 500;	rt.bottom = 300;
	//SetRect(rt, 2);
	//rt.left = 300; rt.top = 400; rt.right = 500;	rt.bottom = 500;
	//SetRect(rt, 3);

	TCHAR* filename[] =
	{
		L"../../data/lobby.bmp",
		L"../../data/hud.dds",
		L"../../data/be_a2.dds",
		L"../../data/sidealpha.dds",
		L"../../data/side.dds"
	};
	TCHAR* filenameEffect[] =
	{
		L"../../data/SHINE0.BMP",
		L"../../data/SHINE1.BMP",
		L"../../data/SHINE2.BMP",
		L"../../data/SHINE3.BMP",
		L"../../data/SHINE4.BMP",
		L"../../data/SHINE5.BMP",
		L"../../data/SHINE6.BMP",
		L"../../data/SHINE7.BMP",
		L"../../data/SHINE8.BMP",
		L"../../data/SHINE9.BMP",

	};
	for (int iTex = 0; iTex < MAX_TEXTURE; iTex++)
	{
		hr = D3DX11CreateShaderResourceViewFromFile(GetDevice(), 
			filenameEffect[iTex],
			NULL, NULL, &m_pTexArraySRV[iTex], NULL);
	}
	for (int iObj = 0; iObj < MAX_OBJECT; iObj++)
	{
		// Texure생성
		hr = D3DX11CreateShaderResourceViewFromFile(GetDevice(), filename[iObj], NULL, NULL, &m_Object[iObj].m_pTexSRV, NULL);
		hr = D3DX11CreateShaderResourceViewFromFile(GetDevice(), filenameEffect[0], NULL, NULL, &m_Object[iObj].m_pTexBlendA, NULL);
		hr = D3DX11CreateShaderResourceViewFromFile(GetDevice(), filenameEffect[1], NULL, NULL, &m_Object[iObj].m_pTexBlendB, NULL);


		
		// (1)인덱스 버퍼 생성
		//D3D11_BUFFER_DESC bd;
		//D3D11_SUBRESOURCE_DATA sd;
		m_IndexList[0] = 0;
		m_IndexList[1] = 2;
		m_IndexList[2] = 1;
		m_IndexList[3] = 2;
		m_IndexList[4] = 1;
		m_IndexList[5] = 3;
		ZeroMemory(&bd, sizeof(bd));
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.BindFlags = D3D11_BIND_INDEX_BUFFER;// 용도
		bd.ByteWidth = sizeof(DWORD) * 6;
		ZeroMemory(&sd, sizeof(D3D11_SUBRESOURCE_DATA));
		sd.pSysMem = m_IndexList;
		hr = GetDevice()->CreateBuffer(&bd, &sd, &m_Object[iObj].m_pIB);

		// (1)정점버퍼 생성
		D3D11_BUFFER_DESC bd;
		ZeroMemory(&bd, sizeof(bd));
		bd.Usage = D3D11_USAGE_DYNAMIC;
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;// 용도
		bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		bd.ByteWidth = sizeof(P_VERTEX) * 4;
		D3D11_SUBRESOURCE_DATA sd;
		ZeroMemory(&sd, sizeof(D3D11_SUBRESOURCE_DATA));
		sd.pSysMem = m_Object[iObj].m_VertexList;
		hr = GetDevice()->CreateBuffer(&bd, &sd, &m_Object[iObj].m_pVB);

		//// (1)인덱스 버퍼 생성
		//m_Object[iObj].m_IndexList[0] = 0;
		//m_Object[iObj].m_IndexList[1] = 1;
		//m_Object[iObj].m_IndexList[2] = 2;

		//m_Object[iObj].m_IndexList[3] = 2;
		//m_Object[iObj].m_IndexList[4] = 1;
		//m_Object[iObj].m_IndexList[5] = 3;
		//ZeroMemory(&bd, sizeof(bd));
		//bd.Usage = D3D11_USAGE_DEFAULT;
		//bd.BindFlags = D3D11_BIND_INDEX_BUFFER;// 용도
		//bd.ByteWidth = sizeof(DWORD) * 6;
		//ZeroMemory(&sd, sizeof(D3D11_SUBRESOURCE_DATA));
		//sd.pSysMem = m_Object[iObj].m_IndexList;
		//hr = GetDevice()->CreateBuffer(&bd, &sd, &m_Object[iObj].m_pIB);

		// 상수 버퍼
		m_Object[iObj].m_ConstantList.x = 1.0f;
		m_Object[iObj].m_ConstantList.y = 0.0f;
		m_Object[iObj].m_ConstantList.z = 0.0f;
		//m_Object[iObj].m_ConstantList.w = 1.0f;
		m_Object[iObj].m_ConstantList.time = m_Timer.m_fAccumulation;

												  //ZeroMemory(&bd, sizeof(bd));
												  //bd.Usage = D3D11_USAGE_DYNAMIC;
												  //bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;// 용도
												  //bd.ByteWidth = sizeof(VS_CONSTANT_BUFFER);
												  //bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
												  //ZeroMemory(&sd, sizeof(D3D11_SUBRESOURCE_DATA));
												  //sd.pSysMem = &m_Object[iObj].m_ConstantList;
												  //hr = GetDevice()->CreateBuffer(&bd, &sd, &m_Object[iObj].m_pCB);

												  //// (2)정점 및 픽쉘 쉐이더 컴파일
												  //DWORD dwShaderFlags = 0;
												  //ID3DBlob* pVSBuffer = 0; // 컴파일한 결과의 스트링
												  //ID3DBlob* pPSBuffer = 0;
												  //ID3DBlob* pErrorBuffer = 0;
												  ////1>  compilation object save succeeded; see E : //\07_TBasisSampleCode\TBasis2DNet\output//\VertexShader.cso
												  //hr = D3DX11CompileFromFile(
												  //	L"VShaderScript.h",
												  //	NULL, NULL,
												  //	"VS3", "vs_5_0",
												  //	dwShaderFlags,
												  //	NULL, NULL,
												  //	&pVSBuffer, &pErrorBuffer, NULL);
												  //if (FAILED(hr))
												  //{
												  //	OutputDebugStringA((char*)pErrorBuffer->GetBufferPointer());
												  //}
												  //hr = D3DX11CompileFromFile(
												  //	L"PShaderScript.h",
												  //	NULL, NULL,
												  //	"PS3", "ps_5_0",
												  //	dwShaderFlags,
												  //	NULL, NULL,
												  //	&pPSBuffer, &pErrorBuffer, NULL);
												  //if (FAILED(hr))
												  //{
												  //	OutputDebugStringA((char*)pErrorBuffer->GetBufferPointer());
												  //}

												  //hr = GetDevice()->CreateVertexShader(
												  //	pVSBuffer->GetBufferPointer(),
												  //	pVSBuffer->GetBufferSize(),
												  //	NULL,
												  //	&m_Object[iObj].m_pVS);

												  //hr = GetDevice()->CreatePixelShader(
												  //	pPSBuffer->GetBufferPointer(),
												  //	pPSBuffer->GetBufferSize(),
												  //	NULL,
												  //	&m_Object[iObj].m_pPS);

												  //// (3) 정점 레이아웃( 정점성분의 구조도 )
												  //D3D11_INPUT_ELEMENT_DESC ied[] = {
												  //	{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
												  //	{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
												  //	/*	{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
												  //	{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },*/
												  //};

												  //int iNumElement = sizeof(ied) / sizeof(ied[0]);
												  //hr = GetDevice()->CreateInputLayout(ied, iNumElement,
												  //	pVSBuffer->GetBufferPointer(),
												  //	pVSBuffer->GetBufferSize(),
												  //	&m_Object[iObj].m_pIL);

												  //if (pVSBuffer != NULL) pVSBuffer->Release();
												  //if (pPSBuffer != NULL) pPSBuffer->Release();
	}

	//  0번 정점버퍼에서 시작하여 3개의 정점을 사용하여
	//  랜더링해라.
	GetContext()->IASetPrimitiveTopology(
		D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	return true;
}
//D3D11 WARNING: ID3D11DeviceContext::DrawIndexed: The Pixel Shader unit expects a Sampler to be set at Slot 0, but none is bound. This is perfectly valid, as a NULL Sampler maps to default Sampler state. However, the developer may not want to rely on the defaults.  [ EXECUTION WARNING #352: DEVICE_DRAW_SAMPLER_NOT_SET]
bool	Sample::Render()
{
	HRESULT hr;
	float fTime = cosf(m_Timer.m_fAccumulation);
	m_pImmediateContext->RSSetViewports(1, &m_ViewPort);
	GetContext()->PSSetSamplers(0, 1, &TDxState::g_pTexSS);
	
	GetContext()->IASetInputLayout(TInputLayout::g_pPT_IL);
	GetContext()->VSSetShader(m_pVS, NULL, 0);
	GetContext()->PSSetShader(m_pPS, NULL, 0);
	GetContext()->IASetIndexBuffer(m_pIB, DXGI_FORMAT_R32_UINT, 0);
	GetContext()->PSSetShaderResources(1, MAX_TEXTURE, m_pTexArraySRV);
	GetContext()->PSSetConstantBuffers(0, 1, &m_pCBPixel);

	for (int iObj = 0; iObj < 1; iObj++)
	{		
		float fFactor[4] = { 0, };
		float fTime2 = cosf(m_Timer.m_fAccumulation) * 0.5f + 0.5f;
		fFactor[0] = fTime2;
		fFactor[1] = fTime2;
		fFactor[2] = fTime2;
		fFactor[3] = 0.5f;
		
		if (m_Input.KeyCheck('B') == KEY_HOLD)
			GetContext()->OMSetBlendState(g_pNoAlphaBlend, 0, -1);
		else
			GetContext()->OMSetBlendState(g_pAlphaBlend, 0, -1);
		
		
		GetContext()->PSSetShaderResources(0, 1,&m_Object[iObj].m_pTexSRV);
		//GetContext()->PSSetShaderResources(1, 1,
		//		&m_Object[iObj].m_pTexBlendA);
		///*GetContext()->PSSetShaderResources(2, 1,
		//		&m_Object[iObj].m_pTexBlendB);*/
			
		
		//GPU메모리 접근
		D3D11_MAPPED_SUBRESOURCE mapResource;
		if (SUCCEEDED(hr = GetContext()->Map(m_pCB, 0, D3D11_MAP_WRITE_DISCARD, 0,
			&mapResource)))
		{
			VS_CONSTANT_BUFFER* pBuffer = (VS_CONSTANT_BUFFER*)mapResource.pData;
			pBuffer->x = m_Object[iObj].m_Data.x;
			pBuffer->y = m_Object[iObj].m_Data.y;
			pBuffer->z = m_Object[iObj].m_Data.z;
			pBuffer->w = 1.0f;
			pBuffer->time = m_Timer.m_fAccumulation; // c1.x
			pBuffer->a = m_iIndex;  // c1.y
			//pBuffer->b = m_iIndex;
			//pBuffer->c = m_iIndex;
			GetContext()->Unmap(m_pCB, 0);
		}	
		GetContext()->VSSetConstantBuffers(0, 1, &m_pCB);

		UINT stride = sizeof(P_VERTEX);
		UINT offset = 0;
		GetContext()->IASetVertexBuffers(0, 1, &m_Object[iObj].m_pVB, &stride, &offset);

#ifdef USED_ONLY_VERTEX
		GetContext()->Draw(6, 0);
#else	
		GetContext()->DrawIndexed(6, 0, 0);
#endif
	}
	return true;
}
bool    Sample::Frame()
{
	HRESULT hr;
	float fTime = cosf(m_Timer.m_fAccumulation);
	
	for (int iObj = 0; iObj < MAX_OBJECT; iObj++)
	{
		m_Object[iObj].m_Data.x = cosf(fTime);
		m_Object[iObj].m_Data.y = cosf(fTime);
		m_Object[iObj].m_Data.z = cosf(fTime);
		m_Object[iObj].m_Data.w = 1.0f;
	}

	if (((int)m_Timer.g_dwAccumulation) % 100 == 0)
	{
		m_iIndex++;
		if (m_iIndex > MAX_TEXTURE-1 )
			m_iIndex = 0;
	}
	//GPU메모리 접근
	
	D3D11_MAPPED_SUBRESOURCE mapResource;
	if (SUCCEEDED(hr = GetContext()->Map(m_pCBPixel, 0, D3D11_MAP_WRITE_DISCARD, 0,
		&mapResource)))
	{
		PS_CONSTANT_BUFFER* pBuffer = (PS_CONSTANT_BUFFER*)mapResource.pData;
		//pBuffer->x = 1.0f;
		//pBuffer->y = 1.0f;
		//pBuffer->z = 1.0f;
		//pBuffer->w = 1.0f;
		pBuffer->index = m_iIndex;
		//pBuffer->a = m_iIndex;
		//pBuffer->b = m_iIndex;
		//pBuffer->c = m_iIndex;
		GetContext()->Unmap(m_pCBPixel, 0);
	}
	return true;
}
bool	Sample::Release()
{
	for (int iObj = 0; iObj < MAX_OBJECT; iObj++)
	{
		SAFE_RELEASE(m_Object[iObj].m_pIL);
		SAFE_RELEASE(m_Object[iObj].m_pVB);
		SAFE_RELEASE(m_Object[iObj].m_pVS);
		SAFE_RELEASE(m_Object[iObj].m_pPS);
	}
	return true;
}

Sample::Sample()
{
	m_iIndex = 0;
}

Sample::~Sample()
{

}
TCORE_RUN(DxSample_1, 800, 600)
//TCORE_START;
//if (sample.SetWindow(hInstance,
//	L"SampleScene_0", 800, 600, WS_POPUPWINDOW))
//{
//	sample.Run();
//}
//TCORE_END;