#include "KCore.h"

LRESULT KCore::MsgProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    return g_Input.MsgProc(hWnd, message, wParam, lParam);
}
bool	KCore::GameRun()
{
    if (!GameFrame()) return false;
    if (!GameRender()) return false;
    return true;
}
bool	KCore::GameInit()
{
    //디바이스 생성
    KDevice::SetDevice();
    m_Timer.Init();
    g_Input.Init();
    m_Write.Init();
    m_DebugCamera.Init();

    m_RState.Init();

    m_DebugCamera.CreateViewMatrix(KVector3(0, 0, -30),KVector3(0, 0, 0));
    m_DebugCamera.CreateProjMatrix(1.0f,1000.0f, XM_PI * 0.5f,
        (float)g_rtClient.right / (float)g_rtClient.bottom);

    //백버퍼를 받아와서 2D Write와 Direct3D와 연동하는 작업
    IDXGISurface1* m_pBackBuffer;
    m_pSwapChain->GetBuffer(0, 
        __uuidof(IDXGISurface),
        (void**)&m_pBackBuffer);
    m_Write.CreateDeviceResources(m_pBackBuffer);
    if (m_pBackBuffer)m_pBackBuffer->Release();
    Init();
    return true;
}
bool	KCore::GameFrame() 
{
    m_Timer.Frame();
    g_Input.Frame();
    m_Write.Frame();
    m_RState.Frame();
    FrameCamera();
    if (g_Input.GetKey('1') == KEY_PUSH)
    {
        m_bDebugText = !m_bDebugText;
    }
    //마우스 위치 이전 값 저장
    g_Input.m_ptBeforePos = g_Input.m_ptPos;
    Frame();
    return true;
}
bool	KCore::GameRender() 
{
    PreRender();
    m_Timer.Render();
    g_Input.Render();
    m_Write.Render();
    m_RState.Render(m_pImmediateContext);
    //프레임 디스플레이 텍스트
    Render();
    //렌더 다음으로 텍스트 렌더링
    if (m_bDebugText)
    {
        RECT  rt = { 0, 0, 800, 600 };
        m_Write.DrawText(rt, m_Timer.m_szTimerString,
            D2D1::ColorF(1, 1, 1, 1));
    }
    m_Write.BlinkMessage(L"EngineCoreD");
    PostRender();
    return true;
}
void KCore::FrameCamera()
{
    if (g_Input.m_bDrag && g_Input.m_ptBeforePos.x == g_Input.m_pDragDown.x)
    {
        g_Input.m_pDrag.x = 0;
    }
    if (g_Input.m_bDrag && g_Input.m_ptBeforePos.y == g_Input.m_pDragDown.y)
    {
        g_Input.m_pDrag.y = 0;
    }
    float fYaw = g_Input.m_pDrag.x * 2.0f * g_fSecPerFrame;
    float fPitch = g_Input.m_pDrag.y * 2.0f * g_fSecPerFrame;
    m_DebugCamera.Update(KVector4(fPitch, fYaw, 0.0f, 0.0f));
    m_DebugCamera.Frame();
}
bool	KCore::GameRelease() 
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

bool	KCore::Init()
{
    return true;
}
bool	KCore::Frame() 
{
    return true;
}
bool	KCore::PreRender() {
    float ClearColor[4] = { 0.1f, 0.15f, 0.15f, 1.0f }; //red,green,blue,alpha
    m_pImmediateContext->ClearRenderTargetView(
        m_DefaultRT.m_pRenderTargetView, ClearColor);
    m_pImmediateContext->ClearDepthStencilView(
        m_DefaultDS.m_pDepthStenV,
        D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
    m_pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    m_pImmediateContext->OMSetRenderTargets(1,
        &m_DefaultRT.m_pRenderTargetView, m_DefaultDS.m_pDepthStenV);
    return true;
}
bool	KCore::Render() 
{
    return true;
}
bool	KCore::PostRender() {
    assert(m_pSwapChain);
    m_pSwapChain->Present(0, 0);
    return true;
}
bool	KCore::Release() {
    return true;
}