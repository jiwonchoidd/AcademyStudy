#include "KPivot.h"

bool KPivot::Init(ID3D11DeviceContext* pContext)
{
	//x, y, z
	m_BoxAxis[0] = KBox(KVector3(0, 0, 0), KVector3(10000, 1, 1));
	m_BoxAxis[1] = KBox(KVector3(0, 0, 0), KVector3(1, 10000, 1));
	m_BoxAxis[2] = KBox(KVector3(0, 0, 0), KVector3(1, 1, 10000));

	m_BoxAxis[0].Axis[0] = KVector3(1, 0, 0);
	m_BoxAxis[0].Axis[1] = KVector3(0, 1, 0);
	m_BoxAxis[0].Axis[2] = KVector3(0, 0, 1);

	m_BoxAxis[1].Axis[0] = KVector3(1, 0, 0);
	m_BoxAxis[1].Axis[1] = KVector3(0, 1, 0);
	m_BoxAxis[1].Axis[2] = KVector3(0, 0, 1);

	m_BoxAxis[2].Axis[0] = KVector3(1, 0, 0);
	m_BoxAxis[2].Axis[1] = KVector3(0, 1, 0);
	m_BoxAxis[2].Axis[2] = KVector3(0, 0, 1);


	DrawDebugInit(pContext);
	return true;
}

bool KPivot::Frame()
{
	return false;
}

bool KPivot::Render(ID3D11DeviceContext* pContext)
{
	for (int i = 0; i < 3; i++)
	{
		KVector4 color;
		if (i == 0)color = { 1, 0, 0, 1 };
		if (i == 1)color = { 0, 1, 0, 1 };
		if (i == 2)color = { 0, 0, 1, 1 };
		DrawDebugRender(&m_BoxAxis[i], pContext, color);
	}
	return true;
}

bool KPivot::Release()
{
	m_Pivot_Box.Release();
	return true;
}

void KPivot::DrawDebugInit(ID3D11DeviceContext* pContext)
{
	if (m_Pivot_Box.Init(L"../../data/shader/VSPS_Frustum.hlsl", L"../../data/shader/VSPS_Frustum.hlsl"))
	{
		//debug object created
		m_Pivot_Box.m_pContext = pContext;
	}
}
void KPivot::DrawDebugRender(KBox* pBox, ID3D11DeviceContext* pContext, KVector4 color)
{
	pBox->List[0] = KVector3(pBox->min.x,
		pBox->min.y,
		pBox->min.z);
	pBox->List[1] = KVector3(pBox->max.x,
		pBox->min.y,
		pBox->min.z);
	pBox->List[2] = KVector3(pBox->min.x,
		pBox->max.y,
		pBox->min.z);
	pBox->List[3] = KVector3(pBox->max.x,
		pBox->max.y,
		pBox->min.z);

	pBox->List[4] = KVector3(pBox->min.x,
		pBox->min.y,
		pBox->max.z);
	pBox->List[5] = KVector3(pBox->max.x,
		pBox->min.y,
		pBox->max.z);
	pBox->List[6] = KVector3(pBox->min.x,
		pBox->max.y,
		pBox->max.z);
	pBox->List[7] = KVector3(pBox->max.x,
		pBox->max.y,
		pBox->max.z);

	// Á¤¸é
	m_Pivot_Box.m_VertexList[0].pos = pBox->List[0];
	m_Pivot_Box.m_VertexList[1].pos = pBox->List[1];
	m_Pivot_Box.m_VertexList[2].pos = pBox->List[2];
	m_Pivot_Box.m_VertexList[3].pos = pBox->List[3];
	// µÞ¸é
	m_Pivot_Box.m_VertexList[4].pos = pBox->List[5];
	m_Pivot_Box.m_VertexList[5].pos = pBox->List[4];
	m_Pivot_Box.m_VertexList[6].pos = pBox->List[7];
	m_Pivot_Box.m_VertexList[7].pos = pBox->List[6];
	// ¿À¸¥ÂÊ¸é
	m_Pivot_Box.m_VertexList[8].pos = pBox->List[1];
	m_Pivot_Box.m_VertexList[9].pos = pBox->List[5];
	m_Pivot_Box.m_VertexList[10].pos = pBox->List[3];
	m_Pivot_Box.m_VertexList[11].pos = pBox->List[7];
	// ¿ÞÂÊ¸é
	m_Pivot_Box.m_VertexList[12].pos = pBox->List[4];
	m_Pivot_Box.m_VertexList[13].pos = pBox->List[0];
	m_Pivot_Box.m_VertexList[14].pos = pBox->List[6];
	m_Pivot_Box.m_VertexList[15].pos = pBox->List[2];
	// À­ÂÊ¸é
	m_Pivot_Box.m_VertexList[16].pos = pBox->List[2];
	m_Pivot_Box.m_VertexList[17].pos = pBox->List[3];
	m_Pivot_Box.m_VertexList[18].pos = pBox->List[6];
	m_Pivot_Box.m_VertexList[19].pos = pBox->List[7];
	// ¾Æ·§¸é
	m_Pivot_Box.m_VertexList[20].pos = pBox->List[4];
	m_Pivot_Box.m_VertexList[21].pos = pBox->List[5];
	m_Pivot_Box.m_VertexList[22].pos = pBox->List[0];
	m_Pivot_Box.m_VertexList[23].pos = pBox->List[1];

	for (int index = 0; index < m_Pivot_Box.m_VertexList.size(); index++)
	{
		m_Pivot_Box.m_VertexList[index].color = color;
	}
	pContext->UpdateSubresource(
		m_Pivot_Box.m_pVertexBuffer.Get(), 0, NULL, &m_Pivot_Box.m_VertexList[0], 0, 0);
	m_Pivot_Box.Render(pContext);
}
KPivot::KPivot()
{
}

KPivot::~KPivot()
{
}
