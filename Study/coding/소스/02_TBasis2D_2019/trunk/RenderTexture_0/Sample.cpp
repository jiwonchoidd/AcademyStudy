#include "Sample.h"
#include "TTexture.h"
#include "TSys.h"

string data = "VS_OUTPUT VS(VS_INPUT input){VS_OUTPUT output = (VS_OUTPUT)0;	float4 pos = float4(input.p, 1.0f);	float4 world = mul(pos, g_matWorld);	float4 view = mul(world, g_matView);	float4 proj = mul(view, g_matProj);	output.p = proj;	output.t = input.t;	output.c = input.c;	return output;}";
//compilation object save succeeded; see E : \00_LectureData\00_TBasisSample\TBasis2D\output\HeightMap_0\MapShader.cso
bool	Sample::Init()
{
	HRESULT hr = S_OK;	
	m_Plane.Create(GetDevice(), GetContext(), NULL, 
		_T("plane.hlsl"), 
		_T("plane.hlsl"));
	if (!m_Map.CreateHeightMap(GetDevice(), GetContext(),
						L"../../../data/WaterHeight.jpg"))
	{
		return false;
	}
	TMapDesc desc =
	{
		m_Map.m_Desc.Width, m_Map.m_Desc.Height, 
		1.0f, L"../../../data/map512Color.png",
		L"mapShader.hlsl",
	};
	m_Map.Load(GetDevice(),	GetContext(), desc);


	TMapDesc waterdesc =
	{
		m_Map.m_Desc.Width, m_Map.m_Desc.Height,
		1.0f, L"../../../data/water.bmp",
		L"mapShader.hlsl",
	};
	m_WaterMap.Load(GetDevice(), GetContext(), waterdesc);
	m_WaterMap.m_iHeightMap = I_Texture.Load(GetDevice(),
		 L"../../../data/WaterHeight.jpg");
	m_WaterMap.m_iNormalMap = I_Texture.Load(GetDevice(),
		L"../../../data/oceanBump.png");
	D3DXMatrixIdentity(&m_matWorldMap);	

	m_ReflcetRT.Create(GetDevice(), GetContext());
	m_RefractRT.Create(GetDevice(), GetContext());

	m_pMainCamera->SetViewMatrix(D3DXVECTOR3(0, 200, -100.0f),
		D3DXVECTOR3(0, 0, 0.0f));
	m_pMainCamera->SetProjMatrix((float)D3DX_PI * 0.25f,
		m_rtClient.right / (FLOAT)m_rtClient.bottom,
		0.1f, 10000.0f);
	return true;
}
bool	Sample::Render()
{
	HRESULT hr;
	m_fWaterHeight = 10.0f +cosf(m_Timer.m_fAccumulation) * 2;
	m_Map.m_WaterCList.w = -m_fWaterHeight;

	if (m_ReflcetRT.Begin(GetContext()))
	{
		GetContext()->IASetPrimitiveTopology(
			D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);		
		
		// 반사 벡터에 대한 투영되는 행렬 생성
		D3DXMATRIX matReflectView;
		D3DXMatrixReflect(&matReflectView, 
			&D3DXPLANE(0.0f, 1.0f, 0.0f, -m_fWaterHeight));
		D3DXMATRIX matView = m_pMainCamera->m_matView;
		D3DXMatrixMultiply(&matReflectView, &matReflectView, &matView);

		m_matWorldMap._42 = 0.0f;
		m_Map.SetMatrix(m_matWorldMap,
			matReflectView,
			m_pMainCamera->m_matProj);
		GetContext()->RSSetState(TDxState::g_pRSFrontCullSolid);
		m_Map.RenderReflect();
		m_ReflcetRT.End(GetContext());		
		GetContext()->RSSetState(TDxState::g_pRSSolid);
	}
	m_Map.m_WaterCList.w = m_fWaterHeight;
	if (m_RefractRT.Begin(GetContext()))
	{
		GetContext()->IASetPrimitiveTopology(
			D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);		
		m_matWorldMap._42 = 0.0f;
		m_Map.SetMatrix(m_matWorldMap,
			m_pMainCamera->m_matView,
			m_pMainCamera->m_matProj);
		m_Map.RenderRefract();
		m_RefractRT.End(GetContext());		
	}
	if (m_Input.KeyCheck('P') == KEY_PUSH)
	{
		D3DX11SaveTextureToFile(GetContext(), m_RefractRT.m_RT.m_pTexture,
				D3DX11_IFF_BMP, _T("refract.bmp"));
		D3DX11SaveTextureToFile(GetContext(), m_ReflcetRT.m_RT.m_pTexture,
			D3DX11_IFF_BMP, _T("reflect.bmp"));
	}
	// 화면전체 랜더링
	//GetContext()->PSSetShaderResources(0, 1, &m_ReflcetRT.m_RT.m_pSRV);
	//m_Plane.Render();
	////// 지형 뒤집어서 랜더링
	//D3DXMATRIX matReflectView;
	//D3DXMatrixReflect(&matReflectView,
	//	&D3DXPLANE(0.0f, 1.0f, 0.0f, -10));
	//D3DXMATRIX matView = m_pMainCamera->m_matView;
	//D3DXMatrixMultiply(&matReflectView, &matReflectView, &matView);
	//GetContext()->RSSetState(TDxState::g_pRSFrontCullSolid);
	////GetContext()->RSSetState(NULL);
	//GetContext()->RSSetViewports(1, &m_ViewPort);
	//GetContext()->IASetPrimitiveTopology(
	//	D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	//m_matWorldMap._42 = 0.0f;
	//m_Map.SetMatrix(m_matWorldMap,
	//	matReflectView,
	//	m_pMainCamera->m_matProj);
	//m_Map.Render();
	
	GetContext()->RSSetState(TDxState::g_pRSSolid);
	m_pImmediateContext->RSSetViewports(1, &m_ViewPort);
	GetContext()->IASetPrimitiveTopology(
		D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	m_matWorldMap._42 = 0.0f;
	m_Map.SetMatrix(m_matWorldMap,
		m_pMainCamera->m_matView,
		m_pMainCamera->m_matProj);
	m_Map.Render();

	GetContext()->PSSetShaderResources(3, 1, &m_ReflcetRT.m_RT.m_pSRV);
	GetContext()->PSSetShaderResources(2, 1, &I_Texture.GetPtr(
		m_WaterMap.m_iHeightMap)->m_pSRV );
	GetContext()->PSSetShaderResources(1, 1, &m_RefractRT.m_RT.m_pSRV);
	GetContext()->PSSetShaderResources(4, 1, &I_Texture.GetPtr(
		m_WaterMap.m_iNormalMap)->m_pSRV);

	m_WaterMap.m_WaterCList.w = m_fWaterHeight;
	m_WaterMap.m_WaterCList.time = g_fAccumulation;
	m_WaterMap.m_WaterCB.Update(m_pImmediateContext,
		(void*)&m_WaterMap.m_WaterCList,
		sizeof(VSPS_WATER_CBUFFER));

	m_matWorldMap._42 = m_fWaterHeight;
	m_WaterMap.SetMatrix(m_matWorldMap,
		m_pMainCamera->m_matView,
		m_pMainCamera->m_matProj);
	m_WaterMap.m_WaterCB.Apply(GetContext(), 1, 1);
	m_WaterMap.Render();

	ID3D11ShaderResourceView* pSRVNull[1] = { NULL };
	GetContext()->PSSetShaderResources(0, 1, pSRVNull);
	GetContext()->PSSetShaderResources(1, 1, pSRVNull);
	GetContext()->PSSetShaderResources(2, 1, pSRVNull);
	GetContext()->PSSetShaderResources(3, 1, pSRVNull);
	GetContext()->PSSetShaderResources(4, 1, pSRVNull);
	GetContext()->RSSetState(NULL);
	return true;
}
bool    Sample::Frame()
{
	HRESULT hr;
	//// 월드 행렬 업데이트
	float fTime = m_Timer.m_fAccumulation;
	m_Map.Frame();
	return true;
}
bool	Sample::Release()
{
	m_Plane.Release();
	m_Map.Release();
	m_WaterMap.Release();
	
	m_ReflcetRT.Release();
	m_RefractRT.Release();
	return true;
}

Sample::Sample()
{
}

Sample::~Sample()
{

}
TCORE_RUN(DxSample_1, 800, 600)