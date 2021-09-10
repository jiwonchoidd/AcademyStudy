#include "TUtils.h"
#include "TDxState.h"

ID3D11RasterizerState* TDxState::g_pRSWireFrame = 0;
ID3D11RasterizerState* TDxState::g_pRSSolid = 0;
ID3D11RasterizerState* TDxState::g_pRS = 0;
ID3D11RasterizerState* TDxState::g_pRSNoneCullSolid = 0;
ID3D11DepthStencilState* TDxState::g_pEnableDSS = 0;
ID3D11DepthStencilState* TDxState::g_pDisableDSS = 0;
ID3D11BlendState*	  TDxState::g_pAlphaBlend = 0;
ID3D11BlendState*	  TDxState::g_pNoAlphaBlend = 0;
ID3D11BlendState*	  TDxState::g_pColorBlend = 0;
ID3D11SamplerState*	   TDxState::g_pTexSS = 0;
ID3D11RasterizerState* TDxState::g_pRSFrontCullSolid = 0;
HRESULT TDxState::SetState(ID3D11Device*	pd3dDevice)
{
	HRESULT hr = S_OK;
	D3D11_SAMPLER_DESC samplerDesc;
	ZeroMemory(&samplerDesc, sizeof(samplerDesc));
	// 필터링 방식
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	// 텍스처 주소 지정 방식
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.BorderColor[0] = 1.0f;
	samplerDesc.BorderColor[1] = 0.0f;
	samplerDesc.BorderColor[2] = 0.0f;
	samplerDesc.BorderColor[3] = 1.0f;
	samplerDesc.MaxAnisotropy = 16;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	samplerDesc.MaxLOD = FLT_MAX;
	samplerDesc.MipLODBias = 0.0f;
	samplerDesc.MinLOD = FLT_MIN;
	if (FAILED(hr = pd3dDevice->CreateSamplerState(&samplerDesc, &g_pTexSS)))
		return hr;

	D3D11_BLEND_DESC blendDesc;
	ZeroMemory(&blendDesc, sizeof(D3D11_BLEND_DESC));
	blendDesc.AlphaToCoverageEnable = FALSE;
	blendDesc.RenderTarget[0].BlendEnable = TRUE;
	blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;

	blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;

	blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	pd3dDevice->CreateBlendState(&blendDesc, &g_pAlphaBlend);

	blendDesc.RenderTarget[0].BlendEnable = FALSE;
	pd3dDevice->CreateBlendState(&blendDesc, &g_pNoAlphaBlend);

	// DECAL BLENDING ONE, INV_SRC_ALPHA , ADD
	// ADDIVIVE BLENDING ONE, ONE, ADD
	// SUBRACITIVE BLENDING ONE, ONE REV_SUBTRACT
	blendDesc.RenderTarget[0].BlendEnable = TRUE;
	blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;// D3D11_BLEND_OP_REV_SUBTRACT;
	blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_COLOR;// DEST_COLOR;// D3D11_BLEND_DEST_COLOR;
	blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;// DEST_COLOR;
	pd3dDevice->CreateBlendState(&blendDesc, &TDxState::g_pColorBlend);


	D3D11_DEPTH_STENCIL_DESC dsd;
	ZeroMemory(&dsd, sizeof(D3D11_DEPTH_STENCIL_DESC));
	dsd.DepthEnable = TRUE;
	dsd.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	// 비교 함수
	dsd.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;

	pd3dDevice->CreateDepthStencilState(&dsd, &g_pEnableDSS);
	dsd.DepthEnable = FALSE;
	pd3dDevice->CreateDepthStencilState(&dsd, &g_pDisableDSS);

	//레스터라이즈 상태 객체 생성
	D3D11_RASTERIZER_DESC rsDesc;
	ZeroMemory(&rsDesc, sizeof(rsDesc));
	rsDesc.DepthClipEnable = TRUE;
	rsDesc.FillMode = D3D11_FILL_SOLID;
	rsDesc.CullMode = D3D11_CULL_BACK;
	if (FAILED(hr = pd3dDevice->CreateRasterizerState(&rsDesc, &TDxState::g_pRS))) return hr;
	
	rsDesc.FillMode = D3D11_FILL_WIREFRAME;
	rsDesc.CullMode = D3D11_CULL_BACK;
	if (FAILED(hr = pd3dDevice->CreateRasterizerState(&rsDesc, &TDxState::g_pRSWireFrame)))
		return hr;
	rsDesc.FillMode = D3D11_FILL_SOLID;
	rsDesc.CullMode = D3D11_CULL_BACK;
	if (FAILED(hr = pd3dDevice->CreateRasterizerState(&rsDesc, &TDxState::g_pRSSolid)))
		return hr;

	rsDesc.FillMode = D3D11_FILL_SOLID;
	rsDesc.CullMode = D3D11_CULL_NONE;
	if (FAILED(hr = pd3dDevice->CreateRasterizerState(&rsDesc, &TDxState::g_pRSNoneCullSolid)))
		return hr;

	rsDesc.FillMode = D3D11_FILL_SOLID;
	rsDesc.CullMode = D3D11_CULL_FRONT;
	if (FAILED(hr = pd3dDevice->CreateRasterizerState(&rsDesc, &TDxState::g_pRSFrontCullSolid)))
		return hr;

	return hr;
}
bool TDxState::Release()
{
	SAFE_RELEASE(g_pTexSS);

	SAFE_RELEASE(g_pAlphaBlend);
	SAFE_RELEASE(g_pNoAlphaBlend);
	SAFE_RELEASE(g_pColorBlend);

	
	SAFE_RELEASE(g_pEnableDSS);
	SAFE_RELEASE(g_pDisableDSS);

	SAFE_RELEASE(g_pRS);
	SAFE_RELEASE(g_pRSWireFrame);	
	SAFE_RELEASE(g_pRSSolid);
	SAFE_RELEASE(g_pRSNoneCullSolid);

	SAFE_RELEASE(g_pRSFrontCullSolid);
	return true;
}