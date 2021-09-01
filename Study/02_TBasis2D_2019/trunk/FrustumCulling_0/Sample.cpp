#include "Sample.h"
#include "TSys.h"

string data = "VS_OUTPUT VS(VS_INPUT input){VS_OUTPUT output = (VS_OUTPUT)0;	float4 pos = float4(input.p, 1.0f);	float4 world = mul(pos, g_matWorld);	float4 view = mul(world, g_matView);	float4 proj = mul(view, g_matProj);	output.p = proj;	output.t = input.t;	output.c = input.c;	return output;}";
//compilation object save succeeded; see E : \00_LectureData\00_TBasisSample\TBasis2D\output\HeightMap_0\MapShader.cso
void   Sample::DrawBoundingBox(T_Box box, D3DXVECTOR4  vColor)
{
	GetContext()->OMSetBlendState(TDxState::g_pAlphaBlend, 0, -1);
	m_Line.SetMatrix(m_matWorldLine, m_pMainCamera->m_matView, m_pMainCamera->m_matProj);
	D3DXVECTOR3 v[8];
	v[0].x = box.vMin.x;v[0].y = box.vMax.y;v[0].z = box.vMax.z;
	v[1].x = box.vMax.x;v[1].y = box.vMax.y;v[1].z = box.vMax.z;
	v[2].x = box.vMax.x;v[2].y = box.vMax.y;v[2].z = box.vMin.z;
	v[3].x = box.vMin.x;v[3].y = box.vMax.y;v[3].z = box.vMin.z;

	v[4].x = box.vMin.x;v[4].y = box.vMin.y;v[4].z = box.vMax.z;
	v[5].x = box.vMax.x;v[5].y = box.vMin.y;v[5].z = box.vMax.z;
	v[6].x = box.vMax.x;v[6].y = box.vMin.y;v[6].z = box.vMin.z;
	v[7].x = box.vMin.x;v[7].y = box.vMin.y;v[7].z = box.vMin.z;

	m_Line.Draw(v[0], v[1]);
	m_Line.Draw(v[1], v[2]);
	m_Line.Draw(v[2], v[3]);
	m_Line.Draw(v[3], v[0]);

	m_Line.Draw(v[4], v[5]);
	m_Line.Draw(v[5], v[6]);
	m_Line.Draw(v[6], v[7]);
	m_Line.Draw(v[7], v[4]);

	m_Line.Draw(v[0], v[4]);
	m_Line.Draw(v[1], v[5]);
	m_Line.Draw(v[2], v[6]);
	m_Line.Draw(v[3], v[7]);
	
}
bool	Sample::Init()
{
	HRESULT hr = S_OK;	

	m_Line.Create(GetDevice(),
		GetContext(),
		L"../../../data/lobby.bmp",
		L"../../../data/shader/Shape.vsh",
		L"../../../data/shader/Shape.psh");
	D3DXMatrixIdentity(&m_matWorldLine);

	if (!m_Map.CreateHeightMap(GetDevice(), GetContext(),
						L"../../../data/HEIGHT_CASTLE.bmp"))
	{
		return false;
	}
	TMapDesc desc =
	{
		m_Map.m_Desc.Width, m_Map.m_Desc.Height, 
		1.0f, L"../../../data/CASTLE.bmp",
		L"mapShader.hlsl",
	};
	m_Map.Load(GetDevice(),	GetContext(), m_pMainCamera, desc);
	D3DXMatrixIdentity(&m_matWorldMap);		

	m_pMainCamera->SetViewMatrix(D3DXVECTOR3(100, 100, -100.0f),
		D3DXVECTOR3(0, 0, 1.0f));
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
	TCHAR pbuffer[256] = { 0, };
	_stprintf_s(pbuffer, L"\n ->>FACES=%d",
		m_Map.m_iNumFace);
	OutputDebugString(pbuffer);

	DrawBoundingBox(m_Map.m_Box, D3DXVECTOR4(1,0,0,1));
	return true;
}
bool    Sample::Frame()
{
	HRESULT hr;
	// 월드 행렬 업데이트
	float fTime = m_Timer.m_fAccumulation;
	// 지형 랜더링
	m_Map.Frame();	
	return true;
}
bool	Sample::Release()
{
	m_Line.Release();
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