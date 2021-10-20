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

    D3D11_BLEND_DESC bd;
    ZeroMemory(&bd, sizeof(D3D11_BLEND_DESC));
    bd.AlphaToCoverageEnable = false;
    bd.IndependentBlendEnable = false;
    bd.RenderTarget[0].BlendEnable=false;
    bd.RenderTarget[0].RenderTargetWriteMask= D3D11_COLOR_WRITE_ENABLE_ALL;
    g_pd3dDevice->CreateBlendState(&bd, &m_pBlendState);

    D3D11_DEPTH_STENCIL_DESC ds;
    ZeroMemory(&ds, sizeof(D3D11_DEPTH_STENCIL_DESC));
    ds.DepthEnable=true;
    ds.DepthWriteMask=D3D11_DEPTH_WRITE_MASK_ALL;
    ds.DepthFunc=D3D11_COMPARISON_LESS;
    ds.StencilEnable=false;
    ds.StencilReadMask=D3D11_DEFAULT_STENCIL_READ_MASK;
    ds.StencilWriteMask=D3D11_DEFAULT_STENCIL_WRITE_MASK;
    ds.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
    ds.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
    ds.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
    ds.FrontFace.StencilFunc= D3D11_COMPARISON_ALWAYS;


    ds.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
    ds.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
    ds.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
    ds.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
    g_pd3dDevice->CreateDepthStencilState(&ds, &m_pDepthStencil);
    return true;
}

bool KRState::Frame()
{
    return true;
}

bool KRState::Render(ID3D11DeviceContext* context)
{
    //±íÀÌ ½ºÅÙ½Ç
    context->OMSetDepthStencilState(m_pDepthStencil, 0);
    // ºí·»µå ½ºÅ×ÀÌÆ® ¿ÀºêÁ§Æ®
    float BlendFactor[] = { 0.0f,0.0f,0.0f,1.0f };
    context->OMSetBlendState(m_pBlendState, BlendFactor, 0xffffff);
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
    m_pBlendState->Release();
    m_pDepthStencil->Release();
    return true;
}

KRState::KRState()
{
}

KRState::~KRState()
{
}
