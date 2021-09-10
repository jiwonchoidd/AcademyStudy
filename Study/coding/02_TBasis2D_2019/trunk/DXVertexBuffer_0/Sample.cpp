#include "Sample.h"
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

	m_Object[iObj].m_VertexList[1].x = fRight;
	m_Object[iObj].m_VertexList[1].y = fTop;
	m_Object[iObj].m_VertexList[1].z = 0.5f;

	m_Object[iObj].m_VertexList[2].x = fLeft;
	m_Object[iObj].m_VertexList[2].y = fBottom;
	m_Object[iObj].m_VertexList[2].z = 0.5f;

	m_Object[iObj].m_VertexList[3].x = fRight;
	m_Object[iObj].m_VertexList[3].y = fBottom;
	m_Object[iObj].m_VertexList[3].z = 0.5f;
}
bool	Sample::Init()
{	
	HRESULT hr = S_OK;
	RECT rt;
	rt.left = 0; rt.top = 0;	rt.right = 800;	rt.bottom = 600;
	SetRect(rt, 0);
	rt.left = 300; rt.top = 200;	rt.right = 500;	rt.bottom = 300;
	SetRect(rt, 1);
	rt.left = 300; rt.top = 400;	rt.right = 500;	rt.bottom = 500;
	SetRect(rt, 2);

	for (int iObj = 0; iObj < MAX_OBJECT; iObj++)
	{
		// (1)정점버퍼 생성
		m_Object[iObj].m_VB.Create(GetDevice(), D3D11_USAGE_DEFAULT,
			D3D11_BIND_VERTEX_BUFFER, sizeof(P_VERTEX), 4, 
			m_Object[iObj].m_VertexList	);
		// (1)인덱스 버퍼 생성
		m_Object[iObj].m_IndexList[0] = 0;
		m_Object[iObj].m_IndexList[1] = 1;
		m_Object[iObj].m_IndexList[2] = 2;
		m_Object[iObj].m_IndexList[3] = 2;
		m_Object[iObj].m_IndexList[4] = 1;
		m_Object[iObj].m_IndexList[5] = 3;
		m_Object[iObj].m_IB.Create(GetDevice(), D3D11_USAGE_DEFAULT,
			D3D11_BIND_INDEX_BUFFER, sizeof(DWORD), 6,
			m_Object[iObj].m_IndexList);
		// 상수 버퍼
		m_Object[iObj].m_ConstantList.x = 1.0f;
		m_Object[iObj].m_ConstantList.y = 0.0f;
		m_Object[iObj].m_ConstantList.z = 0.0f;
		//m_Object[iObj].m_ConstantList.w = 1.0f;
		m_Object[iObj].m_ConstantList.time = 0.5f;// m_Timer.m_fAccumulation;
		m_Object[iObj].m_CB.Create(GetDevice(), D3D11_USAGE_DYNAMIC,
			D3D11_BIND_CONSTANT_BUFFER, sizeof(VS_CONSTANT_BUFFER), 1,
			&m_Object[iObj].m_ConstantList);

		// (2)정점 및 픽쉘 쉐이더 컴파일
		m_Object[iObj].m_iVS = I_Shader.Load(GetDevice(), L"VertexShader.vsh", "VS3", "vs_5_0", t_VS);
		m_Object[iObj].m_iPS = I_Shader.Load(GetDevice(), L"PixelShader.txt", "PS3", "ps_5_0", t_PS);
		m_Object[iObj].m_pVS = I_Shader.GetPtr(m_Object[iObj].m_iVS);
		m_Object[iObj].m_pPS = I_Shader.GetPtr(m_Object[iObj].m_iPS);
	
		// (3) 정점 레이아웃( 정점성분의 구조도 )
		D3D11_INPUT_ELEMENT_DESC ied[] = {
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			/*	{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
				{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },*/
		};

		int iNumElement = 1;
		hr = GetDevice()->CreateInputLayout(ied, iNumElement,
			m_Object[iObj].m_pVS->m_pBlob->GetBufferPointer(),
			m_Object[iObj].m_pVS->m_pBlob->GetBufferSize(),
			&m_Object[iObj].m_pIL);		
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
	for (int iObj = 0; iObj < MAX_OBJECT; iObj++)
	{	
		m_Object[iObj].m_CB.Apply(GetContext());
		GetContext()->IASetInputLayout(m_Object[iObj].m_pIL);
		m_Object[iObj].m_pVS->ApplyVS(GetContext());
		m_Object[iObj].m_pPS->ApplyPS(GetContext());
		m_Object[iObj].m_VB.Apply(GetContext());
		m_Object[iObj].m_IB.Apply(GetContext());	
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
	m_Object[0].m_Data.time = m_Timer.m_fAccumulation;
	m_Object[1].m_Data.x = sinf(fTime);
	m_Object[1].m_Data.y = 1.0f;
	m_Object[1].m_Data.z = 0.2f;
	m_Object[1].m_Data.w = 1.0f;
	m_Object[1].m_Data.time = m_Timer.m_fAccumulation;
	m_Object[2].m_Data.x = 1.0f-cosf(fTime);
	m_Object[2].m_Data.y = 1.0f;
	m_Object[2].m_Data.z = -0.3f;
	m_Object[2].m_Data.w = 1.0f;
	m_Object[2].m_Data.time = m_Timer.m_fAccumulation;

	for (int iObj = 0; iObj < MAX_OBJECT; iObj++)
	{
		m_Object[iObj].m_CB.Update(GetContext(),
			(void*)&m_Object[iObj].m_Data,
			sizeof(VS_CONSTANT_BUFFER));
	}
	return true;
}
bool	Sample::Release()
{
	for (int iObj = 0; iObj < MAX_OBJECT; iObj++)
	{
		SAFE_RELEASE(m_Object[iObj].m_pIL);
		m_Object[iObj].m_VB.Release();
		m_Object[iObj].m_IB.Release();
		m_Object[iObj].m_CB.Release();		
	}
	return true;
}

Sample::Sample()
{
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