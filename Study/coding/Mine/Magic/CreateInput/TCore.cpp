#include "TCore.h"
bool TCore::GameRun()
{
	if (!GameFrame())return false;
	if (!GameRender())return false;
	return true;
}
bool TCore::GameInit()
{
    HRESULT hr;
    if (FAILED(CreateGIFactory()))
    {
        return false;
    }
    if (FAILED(CreateDevice()))
    {
        return false;
    }
    if (FAILED(CreateSwapChain(m_hWnd,
        m_rtClient.right,
        m_rtClient.bottom)))
    {
        return false;
    }
    if (FAILED(SetRenderTargetView()))
    {
        return false;
    }
    if (FAILED(SetViewPort()))
    {
        return false;
    }
    // ������ Ÿ�̸� ����
    m_Timer.Init();

    Init();
    return true;
}
bool TCore::GameFrame()
{
    //������ Ÿ�̸�
    m_Timer.Frame();
	Frame();
	return true;
}
bool TCore::GameRender()
{
    PreRender();
    //Ÿ�̸� ����
    m_Timer.Render();
    Render();
    PostRender();
    return true;
}
bool TCore::GameRelease()
{
    Release();
    //Ÿ�̸� ����
    m_Timer.Release();
    CleanupDevice();
    return true;
}

bool TCore::Init()
{
	return true;
}
bool TCore::Frame()
{
	return true;
}
bool	TCore::PreRender() {
    float R,G,B;
    R = (float)(rand() % 10) / 10;
    G = (float)(rand() % 10) / 10;
    B = (float)(rand() % 10) / 10;
    float ClearColor[4] = {R, G, B, 1.0f }; //red,green,blue,alpha
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
bool TCore::Release()
{
	return true;
}