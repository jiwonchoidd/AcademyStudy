#include "Sample.h"

#pragma region Hint
// 1)리프노드 랜더링
//   - 공유 정점버퍼(맵), 리프노드 당 인덱스 버퍼 사용
// 2)리프노드 랜더링
//   - 리프노드 당 정점버퍼, 공유되는 인덱스 버퍼 사용
//   = LOD -> 패치단위로 16개의 정점 인덱스 버퍼가 있어야 된다. 
//  LOD
// 1) 리프노드에 패치 몇개야 ? 
//   - PatchList[][16] 인덱스버퍼가 있다.
#pragma endregion

bool Sample::Init()
{
    KMapInfo info{
            32 + 1,
            32 + 1, 0,0, 0,
            10.0f
    };
    //텍스쳐 붙이기
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
    //텍스쳐 렌더
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

WinMain_OPT(텍스쳐 LOD, 800, 600);