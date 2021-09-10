#include "Sample.h"
void    Sample::SetRect(RECT& rt, int iObj, float fDepth)
{
	// 윈도우 좌표 -> 투영좌표로 변환
	float fLeft = (rt.left / 800.0f) * 2.0f + -1.0f;
	float fRight = (rt.right / 800.0f) * 2.0f + -1.0f;
	float fTop = ((rt.top / 600.0f) * 2.0f + -1.0f) * -1.0f;
	float fBottom = ((rt.bottom / 600.0f) * 2.0f + -1.0f) *-1.0f;

	m_Object[iObj].m_VertexList[0].x = fLeft;
	m_Object[iObj].m_VertexList[0].y = fTop;
	m_Object[iObj].m_VertexList[0].z = fDepth;
	m_Object[iObj].m_VertexList[0].u = 0.0f;
	m_Object[iObj].m_VertexList[0].v = 0.0f;

	m_Object[iObj].m_VertexList[1].x = fRight;
	m_Object[iObj].m_VertexList[1].y = fTop;
	m_Object[iObj].m_VertexList[1].z = fDepth;
	m_Object[iObj].m_VertexList[1].u = 1.0f;
	m_Object[iObj].m_VertexList[1].v = 0.0f;

	m_Object[iObj].m_VertexList[2].x = fLeft;
	m_Object[iObj].m_VertexList[2].y = fBottom;
	m_Object[iObj].m_VertexList[2].z = fDepth;
	m_Object[iObj].m_VertexList[2].u = 0.0f;
	m_Object[iObj].m_VertexList[2].v = 1.0f;

	m_Object[iObj].m_VertexList[3].x = fRight;
	m_Object[iObj].m_VertexList[3].y = fBottom;
	m_Object[iObj].m_VertexList[3].z = fDepth;
	m_Object[iObj].m_VertexList[3].u = 1.0f;
	m_Object[iObj].m_VertexList[3].v = 1.0f;


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
	DWORD dwShaderFlags = 0;
	ID3DBlob* pVSBuffer = 0; // 컴파일한 결과의 스트링
	ID3DBlob* pPSBuffer = 0;
	ID3DBlob* pErrorBuffer = 0;
	//1>  compilation object save succeeded; see E : //\07_TBasisSampleCode\TBasis2DNet\output//\VertexShader.cso
	hr = D3DX11CompileFromFile(
		L"VShaderScript.h",
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
		L"PShaderScript.h",
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
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		/*	{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },*/
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

	TCHAR* filename[] =
	{
		L"../../../data/lobby.bmp",
		L"../../../data/hud.dds",
		L"../../../data/be_a2.dds",
		L"../../../data/sidealpha.dds",
	};

	RECT rt;
	rt.left = 300; rt.top = 100; rt.right = 500;	rt.bottom = 300;
	SetRect(rt, 2, 0.35f);
	// Texure생성
	hr = D3DX11CreateShaderResourceViewFromFile(GetDevice(), 
		filename[3], NULL, NULL, &m_Object[2].m_pTexSRV, NULL);

	rt.left = 300; rt.top = 300; rt.right = 500;	rt.bottom = 500;
	SetRect(rt, 3, 0.4f);	
	// Texure생성
	hr = D3DX11CreateShaderResourceViewFromFile(GetDevice(), 
		filename[2], NULL, NULL, &m_Object[3].m_pTexSRV, NULL);

	rt.left = 0; rt.top = 0; rt.right = 800;	rt.bottom = 600;
	SetRect(rt, 0, 0.46f);
	// Texure생성
	hr = D3DX11CreateShaderResourceViewFromFile(GetDevice(), 
		filename[0], NULL, NULL, &m_Object[0].m_pTexSRV, NULL);

	rt.left = 0; rt.top = 0; rt.right = 800;	rt.bottom = 600;
	SetRect(rt, 1, 0.43f);
	// Texure생성
	hr = D3DX11CreateShaderResourceViewFromFile(GetDevice(), 
		filename[1], NULL, NULL, &m_Object[1].m_pTexSRV, NULL);

	
	

	
	TCHAR* filenameEffect[] =
	{
		L"../../../data/SHINE0.BMP",
		L"../../../data/SHINE1.BMP",
		L"../../../data/SHINE2.BMP",
		L"../../../data/SHINE3.BMP",
		L"../../../data/SHINE4.BMP",
		L"../../../data/SHINE5.BMP",
		L"../../../data/SHINE6.BMP",
		L"../../../data/SHINE7.BMP",
		L"../../../data/SHINE8.BMP",
		L"../../../data/SHINE9.BMP",

	};
	for (int iTex = 0; iTex < MAX_TEXTURE; iTex++)
	{
		hr = D3DX11CreateShaderResourceViewFromFile(GetDevice(), filenameEffect[iTex], NULL, NULL, &m_pTexArraySRV[iTex], NULL);
	}
	for (int iObj = 0; iObj < MAX_OBJECT; iObj++)
	{	
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

		// 상수 버퍼
		m_Object[iObj].m_ConstantList.x = 1.0f;
		m_Object[iObj].m_ConstantList.y = 0.0f;
		m_Object[iObj].m_ConstantList.z = 0.0f;
		m_Object[iObj].m_ConstantList.w = 1.0f;
		m_Object[iObj].m_ConstantList.time = 0.5f;// m_Timer.m_fAccumulation;

	}

	//  0번 정점버퍼에서 시작하여 3개의 정점을 사용하여
	//  랜더링해라.
	GetContext()->IASetPrimitiveTopology(
		D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	return true;
}

bool	Sample::Render()
{
	HRESULT hr;
	float fTime = cosf(m_Timer.m_fAccumulation);
	m_pImmediateContext->RSSetViewports(1, &m_ViewPort);
	GetContext()->PSSetSamplers(0, 1, &TDxState::g_pTexSS);
	GetContext()->VSSetConstantBuffers(0, 1, &m_pCB);
	GetContext()->IASetInputLayout(TInputLayout::g_pPT_IL);
	GetContext()->VSSetShader(m_pVS, NULL, 0);
	GetContext()->PSSetShader(m_pPS, NULL, 0);
	GetContext()->IASetIndexBuffer(m_pIB, DXGI_FORMAT_R32_UINT, 0);

	for (int iObj = 0; iObj <4; iObj++)
	{		
		if (iObj == 3 )
		{
			GetContext()->PSSetShaderResources(0, 1,
				&m_pTexArraySRV[m_iIndex]);
			GetContext()->OMSetBlendState(TDxState::g_pColorBlend, 0, -1);
			
		}
		else
		{
			GetContext()->PSSetShaderResources(0, 1,
				&m_Object[iObj].m_pTexSRV);			
		}
		//if( iObj > 1) GetContext()->OMSetBlendState(TDxState::g_pAlphaBlend, 0, -1);
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
			pBuffer->time = m_Timer.m_fAccumulation;
			GetContext()->Unmap(m_pCB, 0);
		}

		if (iObj == 3)
		{
			//GPU메모리 접근
			D3D11_MAPPED_SUBRESOURCE mapVB;
			if (SUCCEEDED(hr = GetContext()->Map(m_Object[iObj].m_pVB, 0, D3D11_MAP_WRITE_DISCARD, 0,
				&mapVB)))
			{
				P_VERTEX* pBuffer = (P_VERTEX*)mapVB.pData;
				for (int iVertex = 0; iVertex < 4; iVertex++)
				{
					pBuffer[iVertex].x = m_Object[iObj].m_VertexList[iVertex].x;
					pBuffer[iVertex].y = m_Object[iObj].m_VertexList[iVertex].y;
					pBuffer[iVertex].z = m_Object[iObj].m_VertexList[iVertex].z;
					pBuffer[iVertex].u = m_Object[iObj].m_VertexList[iVertex].u;
					pBuffer[iVertex].v = m_Object[iObj].m_VertexList[iVertex].v;
				}
				GetContext()->Unmap(m_Object[iObj].m_pVB, 0);
			}
		}

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
	m_Object[0].m_Data.x = cosf(fTime);
	m_Object[0].m_Data.y = 1.0f;
	m_Object[0].m_Data.z = 0.1f;
	m_Object[0].m_Data.w = 1.0f;

	m_Object[1].m_Data.x = sinf(fTime);
	m_Object[1].m_Data.y = 1.0f;
	m_Object[1].m_Data.z = 0.2f;
	m_Object[1].m_Data.w = 1.0f;

	m_Object[2].m_Data.x = 1.0f - cosf(fTime);
	m_Object[2].m_Data.y = 1.0f;
	m_Object[2].m_Data.z = -0.3f;
	m_Object[2].m_Data.w = 1.0f;

	m_Object[3].m_Data.x = 0.0f;
	m_Object[3].m_Data.y = 1.0f;
	m_Object[3].m_Data.z = -0.3f;
	m_Object[3].m_Data.w = 1.0f;

	m_fEffectTime += m_Timer.m_fSecondPerFrame;
	float fTimeOfset = 1.0f / MAX_TEXTURE;
	if (m_fEffectTime > fTimeOfset)
	{
		m_iIndex++;
		if (m_iIndex > 9) m_iIndex = 0;
		m_fEffectTime -= fTimeOfset;
	}
	return true;
}
bool	Sample::Release()
{
	for (int iTex = 0; iTex < MAX_TEXTURE; iTex++)
	{
		SAFE_RELEASE(m_pTexArraySRV[iTex]);
	}
	for (int iObj = 0; iObj < MAX_OBJECT; iObj++)
	{
		SAFE_RELEASE(m_Object[iObj].m_pIL);
		SAFE_RELEASE(m_Object[iObj].m_pVB);
		SAFE_RELEASE(m_Object[iObj].m_pVS);
		SAFE_RELEASE(m_Object[iObj].m_pIB);
		SAFE_RELEASE(m_Object[iObj].m_pPS);
		SAFE_RELEASE(m_Object[iObj].m_pCB);
		SAFE_RELEASE(m_Object[iObj].m_pTexSRV);
		SAFE_RELEASE(m_Object[iObj].m_pTexSS);
	}
	return true;
}

Sample::Sample()
{
	m_iIndex = 0;
	m_fEffectTime = 0.0f;
	for (int iTex = 0; iTex < 10; iTex++)
	{
		SAFE_ZERO(m_pTexArraySRV[iTex]);
	}
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