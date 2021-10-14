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
    m_KTexture.LoadTexture(L"../../data/texture_grass.jpg");

    if (!m_Map.Load(info, L"../../data/shader/DefaultShader.hlsl", 
                            L"../../data/shader/DefaultShader.hlsl"))
    {
        return false;
    }
    m_Quadtree.LoadObject(L"../../data/script/StaticLod.txt");
    m_Quadtree.Build(&m_Map);

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
    //�ؽ��� ����
    m_pImmediateContext->PSSetSamplers(0, 1, &m_KTexture.m_pSampler);
    m_pImmediateContext->PSSetShaderResources(1, 1, &m_KTexture.m_pTextureSRV);

    m_Quadtree.Render(m_pImmediateContext, m_DebugCamera.GetCameraPos());
    return false;
}

bool Sample::Release()
{
    m_Map.Release();
    m_Quadtree.Release();
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