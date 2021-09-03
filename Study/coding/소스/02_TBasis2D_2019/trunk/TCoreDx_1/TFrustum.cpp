#include "TFrustum.h"

bool T_Plane::CreatePlane(D3DXVECTOR3 v0,
	D3DXVECTOR3 v1,
	D3DXVECTOR3 v2)
{
	D3DXVECTOR3 vEdge0 = v1 - v0;
	D3DXVECTOR3 vEdge1 = v2 - v0;
	D3DXVECTOR3 vNormal;
	D3DXVec3Cross(&vNormal, &vEdge0, &vEdge1);
	D3DXVec3Normalize(&vNormal, &vNormal);
	fA = vNormal.x;
	fB = vNormal.y;
	fC = vNormal.z;
	// fA * x + fB * y + fC * z + fD = 0
	// -(fA * x + fB * y + fC * z) = fD
	fD = -(vNormal.x * v2.x + vNormal.y * v2.y + vNormal.z * v2.z);

	return true;
};
bool T_Plane::CreatePlane(D3DXVECTOR3 v0,
	D3DXVECTOR3 vNormal)
{
	D3DXVec3Normalize(&vNormal, &vNormal);
	fA = vNormal.x;
	fB = vNormal.y;
	fC = vNormal.z;
	// fA * x + fB * y + fC * z + fD = 0
	// -(fA * x + fB * y + fC * z) = fD
	fD = -(vNormal.x * v0.x + vNormal.y * v0.y + vNormal.z * v0.z);
	return true;
};

bool TFrustum::Init()
{
	// 투영좌표
	m_vBaseFrustum[0] = D3DXVECTOR3(-1.0f, 1.0f, 0.0f);
	m_vBaseFrustum[1] = D3DXVECTOR3(1.0f, 1.0f, 0.0f);
	m_vBaseFrustum[2] = D3DXVECTOR3(1.0f, -1.0f, 0.0f);
	m_vBaseFrustum[3] = D3DXVECTOR3(-1.0f, -1.0f, 0.0f);

	m_vBaseFrustum[4] = D3DXVECTOR3(-1.0f, 1.0f, 1.0f);
	m_vBaseFrustum[5] = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	m_vBaseFrustum[6] = D3DXVECTOR3(1.0f, -1.0f, 1.0f);
	m_vBaseFrustum[7] = D3DXVECTOR3(-1.0f, -1.0f, 1.0f);
	return true;

}

void TFrustum::CreateFrustum()
{
	D3DXMATRIX matViewProj, matInvViewProj;
	D3DXMatrixMultiply(&matViewProj, &m_matView, &m_matProj);
	//matViewProj = m_matView * m_matProj;	

	D3DXMatrixInverse(&matInvViewProj, NULL, &matViewProj);
	// 월드 좌표
	for (int iVertex = 0; iVertex < 8; iVertex++)
	{
		D3DXVec3TransformCoord(&m_vFrustum[iVertex],
			&m_vBaseFrustum[iVertex], &matInvViewProj);
	}
	// 앞면 평면
	m_Plane[0].CreatePlane(m_vFrustum[2],m_vFrustum[1],m_vFrustum[0]);
	// 오른쪽
	m_Plane[1].CreatePlane(m_vFrustum[6],m_vFrustum[5],m_vFrustum[1]);
	// 뒤
	m_Plane[2].CreatePlane(m_vFrustum[7],m_vFrustum[4],m_vFrustum[5]);
	// 왼쪽
	m_Plane[3].CreatePlane(m_vFrustum[3],m_vFrustum[0],	m_vFrustum[4]);
	// 위
	m_Plane[4].CreatePlane(m_vFrustum[1],m_vFrustum[5],	m_vFrustum[4]);
	// 아래
	m_Plane[5].CreatePlane(m_vFrustum[2],m_vFrustum[7],	m_vFrustum[6]);
}
bool TFrustum::ClassifyPoint(D3DXVECTOR3 v)
{
	float fPlaneToPoint;
	for (int iPlane = 0; iPlane < 6; iPlane++)
	{
		fPlaneToPoint = m_Plane[iPlane].fA * v.x +
			m_Plane[iPlane].fB * v.y +
			m_Plane[iPlane].fC * v.z + m_Plane[iPlane].fD;
		if (fPlaneToPoint > 0.0f)
		{
			return false;
		}
	}
	return true;
}
void TFrustum::SetMatrix(D3DXMATRIX* pWorld,
	D3DXMATRIX* pView,
	D3DXMATRIX* pProj)
{
	if (pWorld != NULL)
	{
		m_matWorld = *pWorld;
	}
	if (pView != NULL)
	{
		m_matView = *pView;
	}
	if (pProj != NULL)
	{
		m_matProj = *pProj;
	}

	CreateFrustum();
	//m_ConstantList.m_matWorld = matWorld;
	//m_ConstantList.m_matView = matView;
	//m_ConstantList.m_matProj = matProj;

	//D3DXMatrixTranspose(&m_ConstantList.m_matWorld,
	//	&m_ConstantList.m_matWorld);
	//D3DXMatrixTranspose(&m_ConstantList.m_matView,
	//	&m_ConstantList.m_matView);
	//D3DXMatrixTranspose(&m_ConstantList.m_matProj,
	//	&m_ConstantList.m_matProj);

	//m_CB.Update(m_pImmediateContext,
	//	(void*)&m_ConstantList,
	//	sizeof(CB_VS_PER_OBJECT));
};
bool  TFrustum::CheckOBBInPlane(T_Box* box)
{
	return true;
}
T_POSITION  TFrustum::CheckPositionOBBInPlane(T_Box* pBox)
{
	float		fPlaneToCenter = 0.0;
	float		fDistance = 0.0f;
	D3DXVECTOR3 vDir;
	T_POSITION  t_Position;

	t_Position = P_FRONT;
	for (int iPlane = 0; iPlane < 6; iPlane++)
	{
		vDir = pBox->vAxis[0] * pBox->fExtent[0];
		fDistance = fabs(m_Plane[iPlane].fA * vDir.x + 
						 m_Plane[iPlane].fB*vDir.y + 
						 m_Plane[iPlane].fC * vDir.z);
		vDir = pBox->vAxis[1] * pBox->fExtent[1];
		fDistance += fabs(m_Plane[iPlane].fA * vDir.x + 
			m_Plane[iPlane].fB*vDir.y + 
			m_Plane[iPlane].fC * vDir.z);
		vDir = pBox->vAxis[2] * pBox->fExtent[2];
		fDistance += fabs(m_Plane[iPlane].fA * vDir.x + 
						m_Plane[iPlane].fB*vDir.y + 
						m_Plane[iPlane].fC * vDir.z);

		fPlaneToCenter = m_Plane[iPlane].fA * pBox->vCenter.x +
			m_Plane[iPlane].fB*pBox->vCenter.y +
			m_Plane[iPlane].fC * pBox->vCenter.z +
							m_Plane[iPlane].fD;
		
		if (fPlaneToCenter <= fDistance)
		{
			t_Position = P_SPANNING;
		}

		if (fPlaneToCenter + 1.0f < -fDistance)
		{
			return P_BACK;
		}
	}
	return t_Position;
}
TFrustum::TFrustum()
{
	D3DXMatrixIdentity(&m_matWorld);
	D3DXMatrixIdentity(&m_matView);
	D3DXMatrixIdentity(&m_matProj);
}


TFrustum::~TFrustum()
{
}
