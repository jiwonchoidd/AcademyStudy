#include "K3DAsset.h"

void K3DAsset::SetMatrix(KMatrix* pMatWorld, KMatrix* pMatView, KMatrix* pMatProj)
{
    if (pMatWorld != nullptr)
    {
        m_cbData.matWorld = pMatWorld->Transpose();
    }
    if (pMatView != nullptr)
    {
        m_cbData.matView = pMatView->Transpose();
    }
    if (pMatProj != nullptr)
    {
        m_cbData.matProj = pMatProj->Transpose();
    }
    
    m_vRight.x = m_matWorld._11;
    m_vRight.y = m_matWorld._12;
    m_vRight.z = m_matWorld._13;
    m_vUp.x = m_matWorld._21;
    m_vUp.y = m_matWorld._22;
    m_vUp.z = m_matWorld._23;
    m_vLook.x = m_matWorld._31;
    m_vLook.y = m_matWorld._32;
    m_vLook.z = m_matWorld._33;
}

void K3DAsset::AddPosition(KVector3 vPos)
{
	m_pos += vPos;
	m_matWorld._41 = m_pos.x;
	m_matWorld._42 = m_pos.y;
	m_matWorld._43 = m_pos.z;
}

void K3DAsset::SetPosition(KVector3 vPos)
{
	m_pos = vPos;
	m_matWorld._41 = m_pos.x;
	m_matWorld._42 = m_pos.y;
	m_matWorld._43 = m_pos.z;
}

void K3DAsset::SetRotation(KVector3 vRot)
{
    D3DKMatrixRotationZ(&m_matWorld, vRot.z);
    D3DKMatrixRotationX(&m_matWorld, vRot.x);
    D3DKMatrixRotationY(&m_matWorld, vRot.y);
}

void K3DAsset::Convert(std::vector<PNCT_VERTEX>& list, std::vector<PNCT_VERTEX>& retList)
{
}

void K3DAsset::Convert(KVector2 center, float fWidth, float fHeight, std::vector<PNCT_VERTEX>& retList)
{
}

void K3DAsset::SetUVcoord(std::vector<PNCT_VERTEX>& retList)
{
}

bool K3DAsset::CreateVertexData()
{
	return true;
}

bool K3DAsset::CreateIndexData()
{
	m_IndexList.clear();
	m_IndexList.push_back(0); m_IndexList.push_back(1); m_IndexList.push_back(2);
	m_IndexList.push_back(2); m_IndexList.push_back(1); m_IndexList.push_back(3);
	return true;
}
//접선 ,법선, 종법선, 
bool K3DAsset::CreateTangentSpace(KVector3* v1, KVector3* v2, KVector3* v3, KVector2* uv1, KVector2* uv2, KVector2* uv3, KVector3* normal, KVector3* tangent, KVector3* binormal)
{
    KVector3 vEdge1 = *v2 - *v1;
    KVector3 vEdge2 = *v3 - *v1;

    float vEdge1_U = uv2->x - uv1->x;
    float vEdge1_V = uv2->y - uv1->y;
    float vEdge2_U = uv3->x - uv1->x;
    float vEdge2_V = uv3->y - uv1->y;


    float fDenominator = vEdge1_U * vEdge2_V - vEdge2_U * vEdge1_V;

    if (fDenominator < 0.0001f && fDenominator>-0.0001f)
    {
        *tangent = KVector3(1.0f, 0.0f, 0.0f);
        *binormal = KVector3(0.0f, 1.0f, 0.0f);
        *normal = KVector3(0.0f, 0.0f, 1.0f);
    }
    else
    {
        //계산
        float fScale = 1.0f / fDenominator;

        KVector3 T;
        KVector3 B;
        KVector3 N;
        T = KVector3((vEdge2_V * vEdge1.x - vEdge1_V * vEdge2.x) * fScale,
            (vEdge2_V * vEdge1.y - vEdge1_V * vEdge2.y) * fScale,
            (vEdge2_V * vEdge1.z - vEdge1_V * vEdge2.z) * fScale);
        B = KVector3((-vEdge2_U * vEdge1.x + vEdge1_U * vEdge2.x) * fScale,
            (-vEdge2_U * vEdge1.y + vEdge1_U * vEdge2.y) * fScale,
            (-vEdge2_U * vEdge1.z + vEdge1_U * vEdge2.z) * fScale);
        // The normal N is calculated as the cross product between T and B 
        D3DXVec3Cross(&N, &T, &B);
        // Calculate and cache the reciprocal value
        float fScale2 = 1.0f / ((T.x * B.y * N.z - T.z * B.y * N.x) +
            (B.x * N.y * T.z - B.z * N.y * T.x) +
            (N.x * T.y * B.z - N.z * T.y * B.x));
        KVector3 vTemp = { 0,0,0 };
        (*tangent).x = D3DXVec3Cross(&vTemp, &B, &N)->x * fScale2;
        (*tangent).y = -(D3DXVec3Cross(&vTemp, &N, &T)->x * fScale2);
        (*tangent).z = D3DXVec3Cross(&vTemp, &T, &B)->x * fScale2;
        D3DXVec3Normalize(&(*tangent), &(*tangent));
        (*binormal).x = -(D3DXVec3Cross(&vTemp, &B, &N)->y * fScale2);
        (*binormal).y = D3DXVec3Cross(&vTemp, &N, &T)->y * fScale2;
        (*binormal).z = -(D3DXVec3Cross(&vTemp, &T, &B)->y * fScale2);
        D3DXVec3Normalize(&(*binormal), &(*binormal));
        (*normal).x = D3DXVec3Cross(&vTemp, &B, &N)->z * fScale2;
        (*normal).y = -(D3DXVec3Cross(&vTemp, &N, &T)->z * fScale2);
        (*normal).z = D3DXVec3Cross(&vTemp, &T, &B)->z * fScale2;
        D3DXVec3Normalize(&(*normal), &(*normal));
    }
    return true;
}
K3DAsset::K3DAsset()
{
}

K3DAsset::~K3DAsset()
{
}
