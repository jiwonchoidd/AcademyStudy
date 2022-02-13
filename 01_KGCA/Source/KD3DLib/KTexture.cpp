#include "KTexture.h"

HRESULT KTexture::LoadTexture(std::wstring texfile)
{
    HRESULT hr = DirectX::CreateWICTextureFromFile(
        g_pd3dDevice,
        texfile.c_str(),
        m_pResourceTexture.GetAddressOf(),
        m_pSRVTexture.GetAddressOf());
    if (FAILED(hr))
    {
        hr = DirectX::CreateDDSTextureFromFile(
            g_pd3dDevice,
            texfile.c_str(),
            m_pResourceTexture.GetAddressOf(),
            m_pSRVTexture.GetAddressOf());
    }
    if (FAILED(hr)) return hr;
    return hr;
}

void KTexture::Frame()
{
}

void KTexture::Render()
{
	
}

void KTexture::Release()
{
    m_pResourceTexture.Reset();
	m_pSRVTexture.Reset();
}

KTexture::KTexture()
{
    m_pResourceTexture = nullptr;
	m_pSRVTexture = nullptr;
}

KTexture::~KTexture()
{
    m_pResourceTexture.Reset();
		m_pSRVTexture.Reset();
}
