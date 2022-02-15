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
    static_cast<ID3D11Texture2D*>(m_pResourceTexture.Get())->GetDesc(&m_TextureDesc);
    if (FAILED(hr)) return hr;
    return hr;
}

HRESULT KTexture::LoadTextureWithMask(std::wstring texfile, std::wstring mask)
{
    LoadTexture(texfile);
    HRESULT hr = DirectX::CreateWICTextureFromFile(
        g_pd3dDevice,
        mask.c_str(),
        m_pMaskTexture.GetAddressOf(),
        m_pSRVMask.GetAddressOf());
    if (FAILED(hr))
    {
        hr = DirectX::CreateDDSTextureFromFile(
            g_pd3dDevice,
            mask.c_str(),
            m_pMaskTexture.GetAddressOf(),
            m_pSRVMask.GetAddressOf());
    }
    //static_cast<ID3D11Texture2D*>(m_pResourceTexture.Get())->GetDesc(&m_TextureDesc);
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
    m_pMaskTexture.Reset();
    m_pSRVMask.Reset();
}

KTexture::KTexture()
{
    m_pResourceTexture = nullptr;
	m_pSRVTexture = nullptr;
    m_pMaskTexture=nullptr;
    m_pSRVMask = nullptr;
}

KTexture::~KTexture()
{
    m_pResourceTexture.Reset();
	m_pSRVTexture.Reset();
}
