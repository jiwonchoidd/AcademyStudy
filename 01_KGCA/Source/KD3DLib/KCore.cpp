#include "KCore.h"
bool	KCore::GameRun()
{
    if (!GameFrame()) return false;
    if (!GameRender()) return false;
    return true;
}
bool	KCore::GameInit()
{
    KDevice::SetDevice();
    KState::SetState();
    m_Timer.Init();
    g_Input.Init();
    m_Write.Init();
    m_Camera.Init();
    m_Skybox.Init(L"../../data/shader/Skybox.txt",
        L"../../data/texture/Skybox_dd.dds");
    //g_Input.m_bInputAvailable = false;
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
    // TODO : Frame Timer
    m_Timer.Frame();
    g_Input.Frame();
    //실시간 모든 콜라이더 충돌 프레임
    g_ObjManager.Frame();
    m_Write.Frame();
    if(m_bFreeCamera)m_Camera.Frame();
    m_Skybox.Frame();
    m_ImGuiManager.Frame();
    if (g_Input.GetKey(DIK_F1) == KEY_PUSH)
    {
        m_bDebugText = !m_bDebugText;
    }
    if (g_Input.GetKey(DIK_F2) == KEY_PUSH)
    {
        m_ImGuiManager.OnOffImgui();   
    }

    if (ImGui::Begin(u8"성능 디스플레이"))
    {
        ImGui::SliderFloat("Speed Factor", &m_Speed, 0.0f, 10.0f);
        ImGui::Text("Average %.3f ms/Frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    }
    ImGui::End();

    if (ImGui::Begin(u8"디버깅"))
    {
        ImGui::Checkbox(u8"자유 카메라", &m_bFreeCamera);
 
    }
    ImGui::End();


    Frame();
    return true;
}
bool	KCore::GameRender() 
{
    PreRender();
    //환경 텍스쳐
    ApplyRS(m_pImmediateContext, KState::g_pRSBackface);
    m_Skybox.SetMatrix(&m_Camera.m_matWorld, &m_Camera.m_matView, &m_Camera.m_matProj);
    m_Skybox.Render(m_pImmediateContext);
    ApplyRS(m_pImmediateContext, KState::g_pRSSolid);
        // TODO : Render Timer
        m_Timer.Render();
        g_Input.Render();
        m_Write.Render();
        if (m_bDebugText)
        {
            RECT  rt = { 0, 0, m_rtClient.right, m_rtClient.bottom };
            m_Write.DrawText(rt, m_Timer.m_szTimerString,
                D2D1::ColorF(1, 1, 1, 1));
        }
        
        Render();
        m_ImGuiManager.Render();
    PostRender();
    return true;
}
bool	KCore::PreRender() {
    float ClearColor[4] = { 0.5f, 0.5f, 0.5f, 1.0f }; //red,green,blue,alpha
    m_pImmediateContext->ClearRenderTargetView(m_pRenderTargetView, ClearColor);

    m_pImmediateContext->ClearDepthStencilView(
        m_DepthStencilView,
        D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
    m_pImmediateContext->OMSetRenderTargets(1,
        &m_pRenderTargetView, m_DepthStencilView);

    m_pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    ApplyDSS(m_pImmediateContext, KState::g_pDSS_Disabled);
    ApplySS(m_pImmediateContext, KState::g_pClampSS, 0);
    ApplyRS(m_pImmediateContext, KState::g_pCurrentRS);
    ApplyBS(m_pImmediateContext, KState::g_pBlendState);

    return true;
}
bool	KCore::Render() {
    return true;
}
bool	KCore::PostRender() {
    assert(m_pSwapChain);
    m_pSwapChain->Present(0, 0);
    return true;
}
bool	KCore::GameRelease() 
{
    Release();
    KState::ReleaseState();
    // TODO : Render Timer
    m_Timer.Release();
    g_Input.Release();
    m_Write.Release();
    m_Camera.Release();
    m_Skybox.Release();
    CleanupDevice();
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
