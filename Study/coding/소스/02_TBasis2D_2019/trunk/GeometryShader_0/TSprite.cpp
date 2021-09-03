#include "TSprite.h"
#include "TTexture.h"
#include "TSys.h"

bool  TSprite::SetTexArray(T_STR_ARRAY	 TexArray)
{
	for (int iTex = 0; iTex < TexArray.size(); iTex++)
	{
		INT iIndex = I_Texture.Load(m_pd3dDevice, TexArray[iTex].c_str());
		m_iTexList.push_back(iIndex);
	}
	m_iTexIndex = m_iTexList[0];
	return true;
};

void    TSprite::SetRect(RECT& rt)
{
	// 윈도우 좌표 -> 투영좌표로 변환
	float fLeft = (rt.left / 800.0f) * 2.0f + -1.0f;
	float fRight = (rt.right / 800.0f) * 2.0f + -1.0f;
	float fTop = ((rt.top / 600.0f) * 2.0f + -1.0f) * -1.0f;
	float fBottom = ((rt.bottom / 600.0f) * 2.0f + -1.0f) *-1.0f;

	m_VertexList[0].x = fLeft;
	m_VertexList[0].y = fTop;
	m_VertexList[0].z = 0.5f;
	m_VertexList[0].r = 1.0f; m_VertexList[0].g = 1.0f;
	m_VertexList[0].b = 1.0f; m_VertexList[0].a = 1.0f;
	m_VertexList[0].u = 0.0f;
	m_VertexList[0].v = 0.0f;

	m_VertexList[1].x = fRight;
	m_VertexList[1].y = fTop;
	m_VertexList[1].z = 0.5f;
	m_VertexList[1].r = 1.0f; m_VertexList[1].g = 1.0f;
	m_VertexList[1].b = 1.0f; m_VertexList[1].a = 1.0f;
	m_VertexList[1].u = 1.0f;
	m_VertexList[1].v = 0.0f;

	m_VertexList[2].x = fLeft;
	m_VertexList[2].y = fBottom;
	m_VertexList[2].z = 0.5f;
	m_VertexList[2].r = 1.0f; m_VertexList[2].g = 1.0f;
	m_VertexList[2].b = 1.0f; m_VertexList[2].a = 1.0f;
	m_VertexList[2].u = 0.0f;
	m_VertexList[2].v = 1.0f;

	m_VertexList[3].x = fRight;
	m_VertexList[3].y = fBottom;
	m_VertexList[3].z = 0.5f;
	m_VertexList[3].r = 1.0f; m_VertexList[3].g = 1.0f;
	m_VertexList[3].b = 1.0f; m_VertexList[3].a = 1.0f;
	m_VertexList[3].u = 1.0f;
	m_VertexList[3].v = 1.0f;


}
bool	TSprite::Create(ID3D11Device* pd3dDevice, 
						ID3D11DeviceContext*	pImmediateContext,
						RECT& rt,
						const TCHAR* pTextureName,
	const TCHAR* pVShaderName,
	const TCHAR* pPShaderName)
{
	HRESULT hr = S_OK;
	m_pd3dDevice		= pd3dDevice;
	m_pImmediateContext = pImmediateContext;

	if (pTextureName != NULL)
	{
		m_iTexIndex = I_Texture.Load(pd3dDevice, pTextureName);
	}

	m_Color[0] = rand() / (float)RAND_MAX;
	m_Color[1] = rand() / (float)RAND_MAX;
	m_Color[2] = rand() / (float)RAND_MAX;
	m_Color[3] = 1.0f;
	SetRect(rt);
	// (1)정점버퍼 생성
	m_VB.Create(pd3dDevice, D3D11_USAGE_DEFAULT,
		D3D11_BIND_VERTEX_BUFFER, sizeof(PCT_VERTEX), 4,
		m_VertexList);
	// (1)인덱스 버퍼 생성
	m_IndexList[0] = 0;
	m_IndexList[1] = 1;
	m_IndexList[2] = 2;
	m_IndexList[3] = 2;
	m_IndexList[4] = 1;
	m_IndexList[5] = 3;
	m_IB.Create(pd3dDevice, D3D11_USAGE_DEFAULT,
		D3D11_BIND_INDEX_BUFFER, sizeof(DWORD), 6,
		m_IndexList);
	// 상수 버퍼
	m_ConstantList.x = 1.0f;
	m_ConstantList.y = 0.0f;
	m_ConstantList.z = 0.0f;
	m_ConstantList.w = 1.0f;
	m_ConstantList.time = 0.5f;// m_Timer.m_fAccumulation;
	m_CB.Create(pd3dDevice, D3D11_USAGE_DYNAMIC,
		D3D11_BIND_CONSTANT_BUFFER, sizeof(VS_CONSTANT_BUFFER), 1,
		&m_ConstantList);

	// (2)정점 및 픽쉘 쉐이더 컴파일
	m_iVS = I_Shader.Load(pd3dDevice, pVShaderName, "VS", "vs_5_0", t_VS);
	m_iPS = I_Shader.Load(pd3dDevice, pPShaderName, "PS", "ps_5_0", t_PS);
	m_iGS = I_Shader.Load(pd3dDevice, L"GeometryShader.gsh",
		  "GS", "gs_5_0", t_GS);
	m_pVS = I_Shader.GetPtr(m_iVS);
	m_pPS = I_Shader.GetPtr(m_iPS);
	m_pGS = I_Shader.GetPtr(m_iGS);

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

	return true;
}
bool	TSprite::Render()
{
	HRESULT hr;
	I_Texture.GetPtr(m_iTexIndex)->Apply(m_pImmediateContext);	
	m_CB.Apply(m_pImmediateContext);
	m_pImmediateContext->IASetInputLayout(m_pIL);
	m_pVS->ApplyVS(m_pImmediateContext);
	m_pPS->ApplyPS(m_pImmediateContext);
	m_pGS->ApplyGS(m_pImmediateContext);
	m_VB.Apply(m_pImmediateContext);
	m_IB.Apply(m_pImmediateContext);
#ifdef USED_ONLY_VERTEX
	m_pImmediateContext->Draw(6, 0);
#else	
	m_pImmediateContext->DrawIndexed(6, 0, 0);
#endif	
	return true;
}
bool    TSprite::Frame()
{
	HRESULT hr;
	

	if (m_iTexList.size() > 0)
	{
		m_fCurrentTime += g_fSecPerFrame;
		if (m_fCurrentTime >= m_fStepTime)
		{
			if (++m_iTexIndex >= m_iTexList.size())
			{
				if (m_Loop)	m_iTexIndex = m_iTexList[0];
			}
			m_fCurrentTime -= m_fStepTime;
			m_Data.x = rand() / (float)RAND_MAX;
			m_Data.y = rand() / (float)RAND_MAX;
			m_Data.z = rand() / (float)RAND_MAX;
			m_Data.w = 1.0f;
			m_Data.time = g_fAccumulation;

			m_CB.Update(m_pImmediateContext,
				(void*)&m_Data,
				sizeof(VS_CONSTANT_BUFFER));
		}
	}
	return true;
}
void    TSprite::SetLifeTime(float fLifeTime, bool bLoop)
{
	m_fLifeTime = fLifeTime;
	m_fStepTime = fLifeTime / (float)m_iTexList.size();
	m_Loop = bLoop;
};
bool	TSprite::Release()
{
	SAFE_RELEASE(m_pIL);
	m_VB.Release();
	m_IB.Release();
	m_CB.Release();	
	return true;
}
TSprite::TSprite()
{
	m_iTexIndex = -1;
	m_fLifeTime = 0.0f;
	m_fStepTime = 0.0f;
	m_fCurrentTime = 0.0f;
	m_Loop = true;
	
}


TSprite::~TSprite()
{
}
