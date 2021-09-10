#include "Sample.h"
#include "TSys.h"
bool	Sample::Init()
{
	HRESULT hr = S_OK;
	// 월드 단위행렬
	D3DXMatrixIdentity(&m_cbVsPerObject.m_matWorld);
	// 뷰 행렬
	D3DXVECTOR3 Eye(0.0f, 0.0f, -5.0f);
	D3DXVECTOR3 At(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 Up(0.0f, 1.0f, 0.0f);
	D3DXMatrixLookAtLH(&m_cbVsPerObject.m_matView, &Eye, &At, &Up);

	D3DXMatrixPerspectiveFovLH(&m_cbVsPerObject.m_matProj,
		(float)D3DX_PI * 0.25f,
		800 / (FLOAT)600,
		1.0f,
		10.0f);
	//D3DXMatrixIdentity(&m_cbVsPerObject.m_matProj);

	D3D11_BUFFER_DESC bd;
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	bd.MiscFlags = 0;
	bd.ByteWidth = sizeof(CB_VS_PER_OBJECT);
	if (FAILED(hr = GetDevice()->CreateBuffer(&bd, NULL, 
								&m_pcbVSPerObject)))
	{
		return false;
	}

	RECT rt[3];
	rt[0].left = 0; rt[0].top = 0;	rt[0].right = 800;	rt[0].bottom = 600;
	rt[1].left = 0; rt[1].top = 0;	rt[1].right = 800;	rt[1].bottom = 600;
	//rt[1].left = 100; rt[1].top = 100;	rt[1].right = 500;	rt[1].bottom = 300;
	rt[2].left = 300; rt[2].top = 400;	rt[2].right = 500;	rt[2].bottom = 500;

	TCHAR* filename[] =
	{
		L"../../../data/lobby.bmp",
		L"../../../data/hud.dds",
		L"../../../data/be_a2.dds",
		L"../../../data/sidealpha.dds",
	};
	
	T_STR_ARRAY    filelist;
	for (int itex = 0; itex < 10; itex++)
	{
		TCHAR loadfile[50] = { 0, };
		_stprintf_s(loadfile, L"../../../data/SHINE%d.BMP", itex);
		filelist.push_back(loadfile);
	}	
	m_Object[0].Create(GetDevice(),
		GetContext(),
		rt[0], filename[0], L"VertexShader.vsh", L"PixelShader.txt");

	// 텍스쳐 교체 에니메이션
	m_Object[1].Create(GetDevice(),
		GetContext(),
		rt[1], 0, L"Sprite.vsh", L"Sprite.psh");
	m_Object[1].SetTexArray(filelist);
	m_Object[1].SetLifeTime(1.0f, true);

	
	m_Object[2].Create(GetDevice(),
		GetContext(),
		rt[2], filename[1], L"VertexShader.vsh", L"PixelShader.txt");
	
	GetContext()->IASetPrimitiveTopology(
		D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	return true;
}
bool	Sample::Render()
{
	HRESULT hr;
	GetContext()->PSSetSamplers(0, 1, &TDxState::g_pTexSS);
	GetContext()->OMSetBlendState(TDxState::g_pAlphaBlend, 0, -1);
	//GetContext()->VSSetConstantBuffers(1, 0, NULL);
	GetContext()->VSSetConstantBuffers(1, 1, &m_pcbVSPerObject);

	m_Object[0].Render();

	/*GetContext()->OMSetBlendState(TDxState::g_pColorBlend, 0, -1);
	m_Object[1].Render();

	GetContext()->OMSetBlendState(TDxState::g_pAlphaBlend, 0, -1);
	m_Object[2].Render();*/

	/*for (int iObj = 0; iObj < MAX_OBJECT; iObj++)
	{		
		m_Object[iObj].Render();
	}*/
	return true;
}
bool    Sample::Frame()
{
	HRESULT hr;
	float fTime = cosf(m_Timer.m_fAccumulation);	
	D3DXMatrixIdentity(&m_cbVsPerObject.m_matWorld);
	D3DXMatrixRotationX(&m_cbVsPerObject.m_matWorld,
		fTime);
	static float fZ = -5.0f;
	if (m_Input.KeyCheck('W') == KEY_HOLD)
	{
		fZ += 5.0f * g_fSecPerFrame;
	}
	if (m_Input.KeyCheck('S') == KEY_HOLD)
	{
		fZ -= 5.0f* g_fSecPerFrame;
	}
	D3DXVECTOR3 vecEye(0, 0.0f, fZ);
	D3DXVECTOR3 vecAt(0, 0.0f, 20.0f);
	D3DXMatrixLookAtLH(&m_cbVsPerObject.m_matView,
		&vecEye, &vecAt, &D3DXVECTOR3(0.0f, 1.0f, 0.0f));

	// 상수버퍼 갱신
	D3D11_MAPPED_SUBRESOURCE ms;
	GetContext()->Map(    m_pcbVSPerObject, 0,
		                  D3D11_MAP_WRITE_DISCARD, 0,
		                  &ms);
	CB_VS_PER_OBJECT* pData = (CB_VS_PER_OBJECT*)ms.pData;
	D3DXMatrixTranspose(&pData->m_matWorld,
		&m_cbVsPerObject.m_matWorld);
	D3DXMatrixTranspose(&pData->m_matView,
		&m_cbVsPerObject.m_matView);
	D3DXMatrixTranspose(&pData->m_matProj,
		&m_cbVsPerObject.m_matProj);	
	GetContext()->Unmap(m_pcbVSPerObject,0);

	
	for (int iObj = 0; iObj < MAX_OBJECT; iObj++)
	{
		m_Object[iObj].Frame();	
	}
	return true;
}
bool	Sample::Release()
{
	SAFE_RELEASE(m_pcbVSPerObject);
	for (int iObj = 0; iObj < MAX_OBJECT; iObj++)
	{		
		m_Object[iObj].Release();
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