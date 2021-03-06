#pragma once
#include "KStd.h"
class KDepthStencil
{
public:
	ID3D11Texture2D*			m_pTexture;
	ID3D11ShaderResourceView*	m_pTextureSRV;
	ID3D11DepthStencilView*		m_pDepthStenV;
public:
	ID3D11Texture2D* CreateTexture(UINT Width, UINT Height);
	HRESULT CreateDepthStencilView(UINT Width, UINT Height);
	bool	Release();
public:
	KDepthStencil();
	virtual ~KDepthStencil();
};

