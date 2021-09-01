#include "Sample.h"
#include "TSys.h"

string data = "VS_OUTPUT VS(VS_INPUT input){VS_OUTPUT output = (VS_OUTPUT)0;	float4 pos = float4(input.p, 1.0f);	float4 world = mul(pos, g_matWorld);	float4 view = mul(world, g_matView);	float4 proj = mul(view, g_matProj);	output.p = proj;	output.t = input.t;	output.c = input.c;	return output;}";
//compilation object save succeeded; see E : \00_LectureData\00_TBasisSample\TBasis2D\output\HeightMap_0\MapShader.cso
bool	Sample::Init()
{
	HRESULT hr = S_OK;	
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
	D3DXMatrixIdentity(&m_matWorldMap);	

	m_pMainCamera->SetViewMatrix(D3DXVECTOR3(10, 10, -10.0f),
		D3DXVECTOR3(0, 0, 0.0f));
	m_pMainCamera->SetProjMatrix((float)D3DX_PI * 0.25f,
		m_rtClient.right / (FLOAT)m_rtClient.bottom,
		0.1f, 10000.0f);
	return true;
}
bool	Sample::Render()
{
	HRESULT hr;
	GetContext()->IASetPrimitiveTopology(
		D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	m_Map.SetMatrix(m_matWorldMap,
		m_pMainCamera->m_matView,
		m_pMainCamera->m_matProj);
	m_Map.Render();
	return true;
}
bool    Sample::Frame()
{
	HRESULT hr;
	// 월드 행렬 업데이트
	float fTime = m_Timer.m_fAccumulation;
	
	m_Map.Frame();
	return true;
}
bool	Sample::Release()
{
	m_Map.Release();
	return true;
}

Sample::Sample()
{
}

Sample::~Sample()
{

}
TCORE_RUN(DxSample_1, 800, 600)