#pragma once
#include "KStd.h"
class KLight
{
    //ī�޶�� ���� �ٲ㵵�Ǵµ� ī�޶�� �Ӽ��� �޶�
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
        //���� �⺻
        m_vLightColor = { 1.0f,1.0f,1.0f,1.0f};
        //�̰� ������ ������� ����� ����.
        m_vPos = vPos;
        m_vTarget = vTarget;
        //���� ����ȭ
        //(vTarget - vPos).Normalize(m_vDir);
        m_vDir = m_vTarget - m_vPos;
        D3DXVec3Normalize(&m_vDir, &m_vDir);
        //����ĸ����
        KVector3 vUp(0, 1, 0);
        D3DKMatrixLookAtLH(&m_matView, &m_vPos, &m_vTarget, &vUp);
        //�������� ������� �����
 
        D3DKMatrixPerspectiveFovLH(&m_matProj, XM_PI * 0.45f, 1.0f, 1.0f, 10000.0f);
    }
    bool    Frame()
    {
        //D3DKMatrixRotationY(&m_matWorld, XM_PI * g_fSecPerFrame * 0.01f);
        //������ ����� ���ϴ� �Լ� coord�� w���� 1�� ��
        D3DXVec3TransformCoord(&m_vPos, &m_vPos, &m_matWorld);
        //error C2102: '&'�� l-value�� �־�� �մϴ�.
        m_vDir = m_vTarget - m_vPos;
        D3DXVec3Normalize(&m_vDir, &m_vDir);
        KVector3 vUp(0, 1, 0);
        D3DKMatrixLookAtLH(&m_matView, &m_vPos, &m_vTarget, &vUp);
        D3DKMatrixPerspectiveFovLH(&m_matProj, XM_PI * 0.45f, 1.0f, 1.0f, 10000.0f);
        return true;
    }
};

