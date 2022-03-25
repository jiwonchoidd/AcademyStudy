#include "Sample.h"

#pragma region Hint
// 1)������� ������
//   - ���� ��������(��), ������� �� �ε��� ���� ���
// 2)������� ������
//   - ������� �� ��������, �����Ǵ� �ε��� ���� ���
//   = LOD -> ��ġ������ 16���� ���� �ε��� ���۰� �־�� �ȴ�. 
//  LOD
// 1) ������忡 ��ġ ��� ? 
//   - PatchList[][16] �ε������۰� �ִ�.
#pragma endregion

bool Sample::Init()
{
    KMapInfo info{
            32 + 1,
            32 + 1, 0,0, 0,
            10.0f
    };
    //�ؽ��� ���̱�
    m_KTexture.LoadTexture(L"../../data/charport.bmp");

    if (!m_Map.Load(info, L"../../data/shader/DefaultShader.hlsl", 
                            L"../../data/shader/DefaultShader.hlsl"))
    {
        return false;
    }
    m_Quadtree.LoadObject(L"../../data/script/StaticLod.txt");
    m_Quadtree.Build(&m_Map);

    m_DebugCamera.CreateViewMatrix(KVector3(0, 10, 0), KVector3(0, 0, 0));
    m_DebugCamera.CreateProjMatrix(1.0f, 1000.0f, XM_PI * 0.25f, (float)g_rtClient.right / (float)g_rtClient.bottom);
    return true;
}
bool Sample::Frame()
{
    m_Map.Frame();
    /*for (int iObj = 0; iObj < 2; iObj++)
    {
        m_BoxObj[iObj].Frame();
    }*/
    return true;
}

bool Sample::Render()
{
    m_Map.SetMatrix(
        nullptr,
        &m_DebugCamera.m_matView,
        &m_DebugCamera.m_matProj);
    //m_Map.Render(m_pImmediateContext);
    m_Quadtree.Render(m_pImmediateContext, m_DebugCamera.GetCameraPos());
    //�ؽ��� ����
    m_pImmediateContext->PSSetShaderResources(1, 1, &m_KTexture.m_pTextureSRV);

    return false;
}

bool Sample::Release()
{
    m_Map.Release();
    m_Quadtree.Release();
    m_DebugCamera.Release();
    m_KTexture.Release();
    /*for (int iObj = 0; iObj < 2; iObj++)
    {
        m_BoxObj[iObj].Release();
    }*/

    return false;
}

Sample::Sample()
{
}
Sample::~Sample()
{
}

WinMain_OPT(�ؽ��� LOD, 800, 600);