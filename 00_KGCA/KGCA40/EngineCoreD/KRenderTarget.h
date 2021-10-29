#pragma once
#include "KStd.h"
class KRenderTarget
{
public:
	ID3D11Texture2D* m_pDSTexture;
	ID3D11ShaderResourceView* m_pTextureSRV;
	ID3D11RenderTargetView* m_pRenderTargetView;// 메인 랜더타켓 뷰
public:
	ID3D11Texture2D*	CreateTexture(UINT Width, UINT Height);
	HRESULT				SetRenderTargetView(ID3D11Texture2D* pBackBuffer);
	HRESULT				CreateRenderTargetView(UINT Width, UINT Height);
	bool				Release();
};

