#pragma once
#include <d3d11.h>
#include <d3dx11.h>
class TDxState
{
public:
	static ID3D11RasterizerState* g_pRSWireFrame;
	static ID3D11RasterizerState* g_pRSSolid;
	static ID3D11RasterizerState* g_pRS;
	static ID3D11RasterizerState* g_pRSNoneCullSolid;
	static ID3D11RasterizerState* g_pRSFrontCullSolid;
	static ID3D11DepthStencilState* g_pEnableDSS;
	static ID3D11DepthStencilState* g_pDisableDSS;
	static ID3D11SamplerState*	   g_pTexSS;	
	static ID3D11BlendState*	  g_pAlphaBlend;
	static ID3D11BlendState*	  g_pNoAlphaBlend;
	static ID3D11BlendState*	  g_pColorBlend;
	static HRESULT SetState(ID3D11Device*	pd3dDevice);
	static bool  Release();
};
