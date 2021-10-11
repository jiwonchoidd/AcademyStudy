#include "Sample.h"

//마우스 동작 메세지로 받아옴.
LRESULT Sample::MsgProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    return g_Input.MsgProc(hWnd, message, wParam, lParam);
}
bool Sample::Init()
{
    m_Rs.Init();
    KMapInfo info{
            32 + 1, 
            32 + 1, 0,0, 0,
            10.0f
    };
    if (m_Map.Load(info))
    {
        m_Map.Init();
    }
    m_Quadtree.Build(&m_Map);
    
    m_Camera.Init();
    m_Camera.CreateViewMatrix(  KVector3(16,3,-31), 
                                KVector3(16,3,100000));
    m_Camera.CreateProjMatrix(1.0f,
        1000.0f, 3.14f * 0.5f,
        (float)g_rtClient.right / (float)g_rtClient.bottom);
    return true;
}
bool Sample::Frame()
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

bool Sample::Render()
{
    m_Rs.Render(m_pImmediateContext);
    m_Map.SetMatrix(nullptr,&m_Camera.m_matView,&m_Camera.m_matProj);
    //m_Map.Render(m_pImmediateContext);
    m_Quadtree.Render(m_pImmediateContext, m_Camera.GetCameraPos());
    return false;
}

bool Sample::Release()
{
    m_Rs.Release();
    m_Map.Release();
    m_Quadtree.Release();
    m_Camera.Release();
    return false;
}

Sample::Sample()
{
}
Sample::~Sample()
{
}

WinMain_OPT(정적 LOD, 800, 600);