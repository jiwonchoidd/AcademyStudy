#include "Sample.h"
bool	Sample::Init()
{
	HRESULT hr = S_OK;
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
	m_Object[0].Render();

	GetContext()->OMSetBlendState(TDxState::g_pColorBlend, 0, -1);
	m_Object[1].Render();

	GetContext()->OMSetBlendState(TDxState::g_pAlphaBlend, 0, -1);
	m_Object[2].Render();

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
	for (int iObj = 0; iObj < MAX_OBJECT; iObj++)
	{
		m_Object[iObj].Frame();	
	}
	return true;
}
bool	Sample::Release()
{
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