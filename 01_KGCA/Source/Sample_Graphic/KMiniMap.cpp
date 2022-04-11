#include "KMiniMap.h"

bool KMiniMap::Init(float min, float max)
{
	m_MinMax.x = min;
	m_MinMax.y = max;
	KPlaneObj::Init(L"../../data/shader/VS_Plane.hlsl", L"../../data/shader/PS_Plane.hlsl");
	m_Rt.Create(256, 256);
	return true;
}

bool KMiniMap::Render(ID3D11DeviceContext* m_pContext)
{
	SetMatrix(nullptr, nullptr, nullptr);
	PreRender(m_pContext);
	//0������ : ����Ʈ �ؽ��� ���ҽ� 
	m_pContext->PSSetShaderResources(
		0, 1, m_Rt.m_pTextureSRV.GetAddressOf());
	PostRender(m_pContext,
		m_iNumIndex);

	return true;
}

bool KMiniMap::CreateVertexData()
{
    m_VertexList.resize(4);
	const KVector3 vertices[] =
	{
		{-1.0f,1.0f,1.0f},
		{ 1.0f,1.0f,1.0f},
		{ 1.0f,-1.0f,1.0f},
		{-1.0f,-1.0f,1.0f},
	};
	//-1.0f, -0.4f
   m_VertexList[0].pos = KVector3(m_MinMax.x, m_MinMax.y, 0.0f);
   m_VertexList[0].normal = KVector3(0.0f, 0.0f, -1.0f);
   m_VertexList[0].color = KVector4(1.0f, 1.0f, 1.0f, 1.0f);
   m_VertexList[0].tex = KVector2(0.0f, 0.0f);
   
   m_VertexList[1].pos = KVector3(m_MinMax.y, m_MinMax.y, 0.0f);
   m_VertexList[1].normal = KVector3(0.0f, 0.0f, -1.0f);
   m_VertexList[1].color = KVector4(1.0f, 1.0f, 1.0f, 1.0f);
   m_VertexList[1].tex = KVector2(1.0f, 0.0f);
   
   m_VertexList[2].pos = KVector3(m_MinMax.x, m_MinMax.x, 0.0f);
   m_VertexList[2].normal = KVector3(0.0f, 0.0f, -1.0f);
   m_VertexList[2].color = KVector4(1.0f, 1.0f, 1.0f, 1.0f);
   m_VertexList[2].tex = KVector2(0.0f, 1.0f);
   
   m_VertexList[3].pos = KVector3(m_MinMax.y, m_MinMax.x, 0.0f);
   m_VertexList[3].normal = KVector3(0.0f, 0.0f, -1.0f);
   m_VertexList[3].color = KVector4(1.0f, 1.0f, 1.0f, 1.0f);
   m_VertexList[3].tex = KVector2(1.0f, 1.0f);
    return true;
}

bool KMiniMap::CreateIndexData()
{
    return true;
}

bool KMiniMap::Release()
{
	m_Rt.Release();
	K3DAsset::Release();
	return true;
}
