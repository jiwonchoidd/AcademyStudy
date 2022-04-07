#pragma once
#include "KStd.h"
class KLight
{
    //카메라랑 직접 바꿔도되는데 카메라와 속성이 달라서
public:
    KVector3    m_vPos;
    KVector3    m_vTarget;
    KVector3    m_vDir;
    KVector4    m_vLightColor;
public:
    KMatrix     m_matView;
    KMatrix     m_matProj;
    KMatrix     m_matWorld;
public:
    void    SetLight(KVector3 vPos, KVector3 vTarget)
    {
        //색깔 기본
        m_vLightColor = { 1.0f,1.0f,1.0f,1.0f};
        //이거 가지고 뷰행렬을 만들어 낸다.
        m_vPos = vPos;
        m_vTarget = vTarget;
        //방향 정규화
        //(vTarget - vPos).Normalize(m_vDir);
        m_vDir = m_vTarget - m_vPos;
        D3DXVec3Normalize(&m_vDir, &m_vDir);
        //뷰행렬만들기
        KVector3 vUp(0, 1, 0);
        D3DKMatrixLookAtLH(&m_matView, &m_vPos, &m_vTarget, &vUp);
        //원근투영 투영행렬 만들기
 
        D3DKMatrixPerspectiveFovLH(&m_matProj, XM_PI * 0.45f, 1.0f, 1.0f, 10000.0f);
    }
    bool    Frame()
    {
        //D3DKMatrixRotationY(&m_matWorld, XM_PI * g_fSecPerFrame * 0.01f);
        //정점과 행렬을 곱하는 함수 coord는 w값이 1이 들어감
        D3DXVec3TransformCoord(&m_vPos, &m_vPos, &m_matWorld);
        //error C2102: '&'에 l-value가 있어야 합니다.
        m_vDir = m_vTarget - m_vPos;
        D3DXVec3Normalize(&m_vDir, &m_vDir);
        KVector3 vUp(0, 1, 0);
        D3DKMatrixLookAtLH(&m_matView, &m_vPos, &m_vTarget, &vUp);
        D3DKMatrixPerspectiveFovLH(&m_matProj, XM_PI * 0.45f, 1.0f, 1.0f, 10000.0f);
        return true;
    }
};

