#pragma once
#include "KStd.h"
class KTexture
{
public:
	wrl::ComPtr <ID3D11Resource>			m_pResourceTexture;
	wrl::ComPtr <ID3D11ShaderResourceView>	m_pSRVTexture;

	wrl::ComPtr <ID3D11Resource>			m_pMaskTexture;
	wrl::ComPtr <ID3D11ShaderResourceView>	m_pSRVMask;
	D3D11_TEXTURE2D_DESC					m_TextureDesc;
public:
	HRESULT LoadTexture(std::wstring texfile);
	HRESULT LoadTextureWithMask(std::wstring texfile, std::wstring mask);
	void Frame();
	void Render();
	void Release();
public:
	KTexture();
	~KTexture();
};

