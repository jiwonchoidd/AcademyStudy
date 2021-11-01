#pragma once
#include "KStd.h"
class KDepthStencil
{
public:
	ID3D11Texture2D*			m_pTexture;
	ID3D11ShaderResourceView*	m_pTextureSRV;
	ID3D11DepthStencilView*		m_pDepthStenV;
	ID3D11DepthStencilState*	m_pDepthStenS;
	ID3D11BlendState*			m_pBlendState;
public:
	HRESULT CreateDepthStenState();
public:
	ID3D11Texture2D* CreateTexture(UINT Width, UINT Height);
	HRESULT CreateDepthStencilView(UINT Width, UINT Height);
	HRESULT CreateBlendState();
	bool	Release();
public:
	KDepthStencil();
	virtual ~KDepthStencil();
};

