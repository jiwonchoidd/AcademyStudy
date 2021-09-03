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

	m_Line.Draw(v[0], v[1], vColor);
	m_Line.Draw(v[1], v[2], vColor);
	m_Line.Draw(v[2], v[3], vColor);
	m_Line.Draw(v[3], v[0], vColor);

	m_Line.Draw(v[4], v[5], vColor);
	m_Line.Draw(v[5], v[6], vColor);
	m_Line.Draw(v[6], v[7], vColor);
	m_Line.Draw(v[7], v[4], vColor);

	m_Line.Draw(v[0], v[4]);
	m_Line.Draw(v[1], v[5]);
	m_Line.Draw(v[2], v[6]);
	m_Line.Draw(v[3], v[7]);
	
}

void   Sample::DrawQuadtreeBoundingBox(TNode* pNode)
{	
	if (pNode != NULL)
	{
		D3DXVECTOR4 vColor = D3DXVECTOR4(1, 0, 0, 1);
		switch (pNode->m_dwDepth)
		{
		case 0: vColor = D3DXVECTOR4(1, 0, 0, 1); break;
		case 1: vColor = D3DXVECTOR4(0, 1, 0, 1); break;
		case 2: vColor = D3DXVECTOR4(0, 0, 1, 1); break;
		case 3: vColor = D3DXVECTOR4(1, 1, 0, 1); break;
		}
		if (pNode->m_bLeafNode == true)
		{
			DrawBoundingBox(pNode->m_box, vColor);
		}
	}
	if (pNode->m_bLeafNode == false)
	{
		DrawQuadtreeBoundingBox(pNode->m_pChild[0]);
		DrawQuadtreeBoundingBox(pNode->m_pChild[1]);
		DrawQuadtreeBoundingBox(pNode->m_pChild[2]);
		DrawQuadtreeBoundingBox(pNode->m_pChild[3]);
	}
};
bool	Sample::Init()
{
	HRESULT hr = S_OK;	

	m_Line.Create(GetDevice(),
		GetContext(),
		L"../../../data/lobby.bmp",
		L"../../../data/shader/Shape.vsh",
		L"../../../data/shader/Shape.psh");
	D3DXMatrixIdentity(&m_matWorldLine);

	if (!m_Quadtree.CreateHeightMap(GetDevice(), GetContext(),
						L"../../../data/map65.bmp"))
	{
		return false;
	}
	TMapDesc desc =
	{
		m_Quadtree.m_Desc.Width, m_Quadtree.m_Desc.Height,
		1.0f, L"../../../data/castle.jpg",
		L"mapShader.hlsl",
	};
	m_Quadtree.Load(GetDevice(),	GetContext(), m_pMainCamera, desc);
	D3DXMatrixIdentity(&m_matWorldMap);		

	m_Quadtree.m_pRootNode = 
		m_Quadtree.CreateNode(NULL, 0, m_Quadtree.m_Desc.Width-1,
			(m_Quadtree.m_Desc.Width * m_Quadtree.m_Desc.Height)-1,
			(m_Quadtree.m_Desc.Height-1) * m_Quadtree.m_Desc.Width);
	m_Quadtree.BuildTree(m_Quadtree.m_pRootNode);

	// 상수 버퍼
	ZeroMemory(&m_WaterCList, sizeof(VSPS_LIGHT_CBUFFER));
	m_WaterCB.Create(GetDevice(), D3D11_USAGE_DYNAMIC,
		D3D11_BIND_CONSTANT_BUFFER, sizeof(VSPS_LIGHT_CBUFFER), 1,
		&m_WaterCList);

	m_pMainCamera->SetViewMatrix(D3DXVECTOR3(0, 10, -100.0f),
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
	m_Quadtree.SetMatrix(m_matWorldMap,
		m_pMainCamera->m_matView,
		m_pMainCamera->m_matProj);
	m_WaterCB.Apply(GetContext(), 1, 0);
	m_Quadtree.Render();
	TCHAR pbuffer[256] = { 0, };
	_stprintf_s(pbuffer, L"\n ->>FACES=%d",
		m_Quadtree.m_iNumFace);
	OutputDebugString(pbuffer);
	//DrawQuadtreeBoundingBox(m_Quadtree.m_pRootNode);
	//DrawBoundingBox(m_Map.m_Box, D3DXVECTOR4(1,0,0,1));
	return true;
}
bool    Sample::Frame()
{
	HRESULT hr;
	// 월드 행렬 업데이트
	float fTime = m_Timer.m_fAccumulation;
	// 지형 랜더링
	m_Quadtree.Frame();

	D3DXMATRIX matRotation;
	D3DXVECTOR3 vLight={100, 100, -100.0f};
	D3DXMatrixRotationY(&matRotation, fTime);
	D3DXVec3TransformCoord(&vLight, &vLight, &matRotation);
	D3DXVec3Normalize(&vLight, &-vLight);

	m_WaterCList.vLight.x = vLight.x;
	m_WaterCList.vLight.y = vLight.y;
	m_WaterCList.vLight.z = vLight.z;
	m_WaterCList.vLight.w = 1000.0f;
	m_WaterCList.vLightColor.x = cosf(fTime) * 0.5f + 0.5f;
	m_WaterCList.vLightColor.y = sinf(fTime) * 0.5f + 0.5f;
	m_WaterCList.vLightColor.z = cosf(1-fTime) * 0.5f + 0.5f;
	m_WaterCList.vLightColor.w = 1.0f;
	m_WaterCList.vMaterialColor.x = 1.0f;
	m_WaterCList.vMaterialColor.y = 1.0f;
	m_WaterCList.vMaterialColor.z = 1.0f;
	m_WaterCList.vMaterialColor.w = 1.0f;
	m_WaterCList.vAmbientColor.x = 0.3f;
	m_WaterCList.vAmbientColor.y = 0.3f;
	m_WaterCList.vAmbientColor.z = 0.3f;
	m_WaterCList.vAmbientColor.w = 1.0f;
	m_WaterCList.time = g_fAccumulation;
	m_WaterCB.Update(m_pImmediateContext,
		(void*)&m_WaterCList,
		sizeof(VSPS_LIGHT_CBUFFER));

	return true;
}
bool	Sample::Release()
{
	m_Line.Release();
	m_WaterCB.Release();
	m_Quadtree.Release();
	return true;
}

Sample::Sample()
{
}

Sample::~Sample()
{

}
TCORE_RUN(DxSample_1, 800, 600)