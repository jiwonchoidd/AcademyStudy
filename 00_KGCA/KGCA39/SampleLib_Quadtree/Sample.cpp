#include "Sample.h"

//메세지 프로시저
LRESULT Sample::MsgProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    return g_Input.MsgProc(hWnd, message, wParam, lParam);
}

bool	Sample::Init()
{
    MapInfo info{ 
        128 + 1, 128 + 1,
        0, 0, 0, 1.0f
    };
    if (m_Map.Load(info))
    {
        m_Map.Init();
    }
    m_Quadtree.Build(&m_Map);

    m_Camera.Init();

    m_Camera.CreateViewMatrix(KVector3(16, 3, -31),
        KVector3(16, 3, 100000));

    m_Camera.CreateProjMatrix(1.0f,
        1000.0f, 3.14f * 0.5f,
        (float)g_rtClient.right / (float)g_rtClient.bottom);
    return true;
}
bool	Sample::Frame()
{
    if (g_Input.m_bDrag && g_Input.m_ptBeforePos.x == g_Input.m_pDragDown.x)
    {
        g_Input.m_pDrag.x = 0;
    }
    if (g_Input.m_bDrag && g_Input.m_ptBeforePos.y == g_Input.m_pDragDown.y)
    {
        g_Input.m_pDrag.y = 0;
    }
    m_fYaw += g_fSecPerFrame * g_Input.m_pDrag.x * 5.0f;
    m_fPitch += g_fSecPerFrame * g_Input.m_pDrag.y * 5.0f;
    m_Camera.Update(KVector4(m_fPitch, m_fYaw, 0.0f, 0.0f));
    m_Camera.Frame();

    m_Map.Frame();
    g_Input.m_ptBeforePos = g_Input.m_ptPos;
    return true;
}
bool	Sample::Render()
{
    m_Map.SetMatrix(nullptr, &m_Camera.m_matView, &m_Camera.m_matProj);
    m_Quadtree.Render(m_pImmediateContext);
    return false;
}
bool	Sample::Release()
{
    m_Map.Release();
    m_Camera.Release();
    return false;
}

WinMain_OPT(Quadtree, 800, 600);