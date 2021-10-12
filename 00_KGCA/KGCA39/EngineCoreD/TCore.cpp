#include "TCore.h"

LRESULT TCore::MsgProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    return g_Input.MsgProc(hWnd, message, wParam, lParam);
}
void TCore::FrameCamera()
{
    if (g_Input.m_bDrag && g_Input.m_ptBeforePos.x == g_Input.m_pDragDown.x)
    {
        g_Input.m_pDrag.x = 0;

    }
    if (g_Input.m_bDrag && g_Input.m_ptBeforePos.y == g_Input.m_pDragDown.y)
    {
        g_Input.m_pDrag.y = 0;
    }
    float fYaw = g_fSecPerFrame * g_Input.m_pDrag.x * 5.0f;
    float fPitch = g_fSecPerFrame * g_Input.m_pDrag.y * 5.0f;
    m_DebugCamera.Update(KVector4(fPitch, fYaw, 0.0f, 0.0f));
    m_DebugCamera.Frame();
}
bool	TCore::GameRun()
{
    if (!GameFrame()) return false;
    if (!GameRender()) return false;
    return true;
}
bool	TCore::GameInit()
{
    TDevice::SetDevice();
    m_Timer.Init();
    g_Input.Init();
    m_Write.Init();
    //RS Wireframe
    m_RState.Init();
    //Camera
    m_DebugCamera.Init();
    //
    m_DebugCamera.CreateViewMatrix(KVector3(0, 0, -30),KVector3(0, 0, 0));
    m_DebugCamera.CreateProjMatrix(1.0f,1000.0f, 3.14 * 0.5f,
        (float)g_rtClient.right / (float)g_rtClient.bottom);
    //
    IDXGISurface1* m_pBackBuffer;
    m_pSwapChain->GetBuffer(0, 
        __uuidof(IDXGISurface),
        (void**)&m_pBackBuffer);
    m_Write.CreateDeviceResources(m_pBackBuffer);
    if (m_pBackBuffer)m_pBackBuffer->Release();
    Init();
    return true;
}
bool	TCore::GameFrame() 
{
    // TODO : Frame Timer
    m_Timer.Frame();
    g_Input.Frame();
    m_Write.Frame();
    m_RState.Frame();
   
    FrameCamera();
    if (g_Input.GetKey('1') == KEY_PUSH)
    {
        m_bDebugText = !m_bDebugText;
    }
    g_Input.m_ptBeforePos = g_Input.m_ptPos;
    Frame();
    return true;
}
bool	TCore::GameRender() 
{
    PreRender();   
        // TODO : Render Timer
        m_Timer.Render();
        g_Input.Render();
        m_Write.Render();
        m_RState.Render(m_pImmediateContext);

        if (m_bDebugText)
        {
            RECT  rt = { 0, 0, 800, 600 };
            m_Write.DrawText(rt, m_Timer.m_szTimerString,
                D2D1::ColorF(1, 1, 1, 1));
        }
        Render();
    PostRender();    
    return true;
}
bool	TCore::GameRelease() 
{
    Release();
    // TODO : Render Timer
    m_Timer.Release();
    g_Input.Release();
    m_Write.Release();
    m_RState.Release();
    m_DebugCamera.Release();
    CleanupDevice();
    return true;
}

bool	TCore::Init()
{
    return true;
}
bool	TCore::Frame() {
    return true;
}
bool	TCore::PreRender() {
    float ClearColor[4] = { 0.3f, 0.5f, 0.3f, 1.0f }; //red,green,blue,alpha
    m_pImmediateContext->ClearRenderTargetView(m_pRenderTargetView, ClearColor);

    return true;
}
bool	TCore::Render() {
    return true;
}
bool	TCore::PostRender() {
    assert(m_pSwapChain);
    m_pSwapChain->Present(0, 0);
    return true;
}
bool	TCore::Release() {
    return true;
}