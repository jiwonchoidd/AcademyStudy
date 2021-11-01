#include "Sample.h"

bool	Sample::Init()
{
    MapInfo info{
           64 + 1,
           64 + 1, 0,0, 0,
           1.0f
    };
    if (m_Map.Load(info))
    {
        m_Map.Init();
    }
    for (int iObj = 0; iObj < 2; iObj++)
    {
        m_BoxObj[iObj].LoadObject(L"ObjectData.txt");
        m_BoxObj[iObj].Init();
    }

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
    m_Camera.Frame();

    m_Map.Frame();
    for (int iObj = 0; iObj < 2; iObj++)
    {
        m_BoxObj[iObj].Frame();
    }
    return true;
}
bool	Sample::Render()
{
    m_Map.SetMatrix(
        nullptr,
        &m_Camera.m_matView,
        &m_Camera.m_matProj);
    m_Map.Render(m_pImmediateContext);

    m_BoxObj[0].m_matWorld._41 = -3.0f;
    m_BoxObj[1].m_matWorld._41 = 3.0f;

    for (int iObj = 0; iObj < 2; iObj++)
    {
        m_BoxObj[iObj].SetMatrix(
            &m_BoxObj[iObj].m_matWorld,
            &m_Camera.m_matView,
            &m_Camera.m_matProj);
        m_BoxObj[iObj].Render(m_pImmediateContext);
    }
    return false;
}
bool	Sample::Release()
{
    m_Map.Release();
    for (int iObj = 0; iObj < 2; iObj++)
    {
        m_BoxObj[iObj].Release();
    }
    m_Camera.Release();
    return false;
}

WinMain_OPT(매크로함수,800,600);