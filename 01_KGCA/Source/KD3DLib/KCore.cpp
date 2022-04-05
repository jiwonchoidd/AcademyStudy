#include "KCore.h"
#include "KObjectManager.h"

KWrite*	g_Write= nullptr;

bool	KCore::GameRun()
{
    if (!GameFrame()) return false;
    if (!GameRender()) return false;
    return true;
}
bool	KCore::GameInit()
{
    if (KDevice::SetDevice())
    {
        KState::SetState();
        m_Timer.Init();
        g_Input.Init();
        if (m_Write.Init())
        {
            IDXGISurface1* m_pBackBuffer;
            m_pSwapChain->GetBuffer(0,
                __uuidof(IDXGISurface),
                (void**)&m_pBackBuffer);
            m_Write.CreateDeviceResources(m_pBackBuffer);
            if (m_pBackBuffer)m_pBackBuffer->Release();
        }
    }

    g_Write = &m_Write;
    Init();
    return true;
}
bool	KCore::GameFrame() 
{
    m_Timer.Frame();
    g_Input.Frame();
    g_ObjManager.Frame();
    m_ImGuiManager.Frame();
    if (g_InputData.bDebugRender)
    {
        m_bDebugMode = !m_bDebugMode;
    }
    if (g_Input.GetKey(DIK_F2) == KEY_PUSH)
    {
        m_ImGuiManager.OnOffImgui(); 
    }

    if (ImGui::Begin(u8"성능 디스플레이"))
    {
        float frame = ImGui::GetIO().Framerate;
        float average= 1000.0f / ImGui::GetIO().Framerate;
        ImGui::Text("Average %.3f ms/Frame (%.1f FPS)", average, frame);
        ImGui::Text("res %d, %d", g_rtClient.right, g_rtClient.bottom);
    }
    ImGui::End();

    Frame();
    return true;
}
bool	KCore::GameRender() 
{
    PreRender();
        // TODO : Render Timer
        m_Timer.Render();
        g_Input.Render();
        
        Render();
        m_ImGuiManager.Render();

        //F1 누르면 디버그 모드 :: 와이어 프레임 모드
        if (m_bDebugMode)
        {
            KState::g_pCurrentRS = KState::g_pRSWireFrame;
            RECT  rt = { 0, 0, m_rtClient.right, m_rtClient.bottom };
            m_Write.RenderText(rt, m_Timer.m_szTimerString,
                D2D1::ColorF(0.5f, 0.5f, 0.5f, 1), m_Write.m_pTextGame15);
        }
        else
        {
            KState::g_pCurrentRS = KState::g_pRSSolid;
        }
    PostRender();
    return true;
}

bool	KCore::PreRender() {

    //새 스왑체인 뷰포트
    float ClearColor[4] = { 0.0f, 0.0f, 0.0f, 1.0f }; //red,green,blue,alpha

    m_pImmediateContext.Get()->ClearRenderTargetView(m_DefaultRT.m_pRenderTargetView.Get(), ClearColor);

    m_pImmediateContext.Get()->ClearDepthStencilView(
        m_DefaultRT.m_DepthStencil.m_pDepthStencilView.Get(),
        D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
    m_pImmediateContext->OMSetRenderTargets(1,
        m_DefaultRT.m_pRenderTargetView.GetAddressOf(), m_DefaultRT.m_DepthStencil.m_pDepthStencilView.Get());

    m_pImmediateContext.Get()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    ApplyDSS(m_pImmediateContext.Get(), KState::g_pCurrentDSS);
    ApplySS(m_pImmediateContext.Get(), KState::g_pWrapSS, 0);
    ApplyRS(m_pImmediateContext.Get(), KState::g_pCurrentRS);
    ApplyBS(m_pImmediateContext.Get(), KState::g_pCurrentBS);
    return true;
}

bool	KCore::Render() 
{
    return true;
}

bool	KCore::PostRender() 
{
    //스왑체인 작업
    assert(m_pSwapChain.Get());
    m_pSwapChain.Get()->Present(0, 0);
    return true;
}

bool	KCore::GameRelease() 
{
    Release();
    KState::ReleaseState();

    m_Timer.Release();
    g_Input.Release();
    m_Write.Release();
    m_ImGuiManager.Release();
    CleanupDevice();
    return true;
}
bool    KCore::ResizeDevice(UINT iWidth, UINT iHeight)
{
    if (m_pd3dDevice.Get() == nullptr) return false;
    DeleteResizeDevice(iWidth, iHeight);

    m_Write.DeleteDeviceResize();
    KDevice::ResizeDevice(iWidth, iHeight);
    KWindow::ResizeDevice(iWidth, iHeight);

    IDXGISurface1* pSurface = nullptr;
    HRESULT hr = m_pSwapChain.Get()->GetBuffer(0,
        __uuidof(IDXGISurface1),
        (void**)&pSurface);
    if (SUCCEEDED(hr))
    {
        m_Write.CreateDeviceResources(pSurface);
    }
    if (pSurface) pSurface->Release();

    CreateResizeDevice(iWidth, iHeight);
    return true;
}
bool    KCore::CreateResizeDevice(UINT iWidth, UINT iHeight)
{
    return true;
}
bool    KCore::DeleteResizeDevice(UINT iWidth, UINT iHeight)
{
    return true;
}
bool	KCore::Init()
{
    return true;
}
bool	KCore::Frame() {
    return true;
}
bool	KCore::Release() {
    return true;
}

KCore::KCore()
{
}

KCore::~KCore()
{
}
