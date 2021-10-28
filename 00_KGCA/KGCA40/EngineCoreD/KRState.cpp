#include "KRState.h"
#include "KInput.h"
bool KRState::Init()
{
    D3D11_RASTERIZER_DESC rd;
    ZeroMemory(&rd, sizeof(D3D11_RASTERIZER_DESC));
    rd.FillMode = D3D11_FILL_WIREFRAME;
    rd.CullMode = D3D11_CULL_BACK;
    g_pd3dDevice->CreateRasterizerState(&rd, &m_pRSWireFrame);
    ZeroMemory(&rd, sizeof(D3D11_RASTERIZER_DESC));
    rd.FillMode = D3D11_FILL_SOLID;
    rd.CullMode = D3D11_CULL_BACK;
    g_pd3dDevice->CreateRasterizerState(&rd, &m_pRSSolid);

    return true;
}

bool KRState::Frame()
{
    return true;
}

bool KRState::Render(ID3D11DeviceContext* context)
{
    if (g_Input.GetKey(VK_F2) == KEY_PUSH)
    {
        (m_isChecked) ? m_isChecked=false : m_isChecked = true;
    }
    if (m_isChecked)
    {
        context->RSSetState(m_pRSWireFrame);
    }
    else
    {
        context->RSSetState(m_pRSSolid);
    }
    return true;
}

bool KRState::Release()
{
    m_pRSWireFrame->Release();
    m_pRSSolid->Release();
    return true;
}

KRState::KRState()
{
}

KRState::~KRState()
{
}
