#include "KFrustum.h"

bool KFrustum::Init()
{
	//왼쪽 아래부터 
	m_Frustum[0] = KVector3(-1.0f, -1.0f, 0.0f);
	m_Frustum[1] = KVector3(-1.0f, 1.0f, 0.0f);
	m_Frustum[2] = KVector3(1.0f, 1.0f, 0.0f);
	m_Frustum[3] = KVector3(1.0f, -1.0f, 0.0f);

	m_Frustum[4] = KVector3(-1.0f, -1.0f, 1.0f);
	m_Frustum[5] = KVector3(-1.0f, 1.0f, 1.0f);
	m_Frustum[6] = KVector3(1.0f, 1.0f, 1.0f);
	m_Frustum[7] = KVector3(1.0f, -1.0f, 1.0f);
	return true;
}

//버텍스 버퍼를 프러스텀 크기에 맞게 변형해서 업데이트
void KFrustum::CreateFrustum(KMatrix& matView, KMatrix& matProj)
{
	m_Frustum[0] = KVector3(-1.0f, -1.0f, 0.0f);
	m_Frustum[1] = KVector3(-1.0f, 1.0f, 0.0f);
	m_Frustum[2] = KVector3(1.0f, 1.0f, 0.0f);
	m_Frustum[3] = KVector3(1.0f, -1.0f, 0.0f);
				   
	m_Frustum[4] = KVector3(-1.0f, -1.0f, 1.0f);
	m_Frustum[5] = KVector3(-1.0f, 1.0f, 1.0f);
	m_Frustum[6] = KVector3(1.0f, 1.0f, 1.0f);
	m_Frustum[7] = KVector3(1.0f, -1.0f, 1.0f);

	KMatrix matInverse;
	D3DKMatrixMultiply(&matInverse, &matView, &matProj);
	D3DKMatrixInverse(&matInverse, NULL, &matInverse);
	for (int iVer = 0; iVer < 8; iVer++)
	{
		D3DXVec3TransformCoord(
			&m_Frustum[iVer], &m_Frustum[iVer], &matInverse);
	}
	if (m_VertexList.size() > 0)
	{
		int index = 0;
		// +z
		m_VertexList[index].pos = m_Frustum[6];
		m_VertexList[++index].pos = m_Frustum[5];
		m_VertexList[++index].pos = m_Frustum[7];
		m_VertexList[++index].pos = m_Frustum[4];
		// -Z plane
		m_VertexList[++index].pos = m_Frustum[1];
		m_VertexList[++index].pos = m_Frustum[2];
		m_VertexList[++index].pos = m_Frustum[0];
		m_VertexList[++index].pos = m_Frustum[3];
		// +X plane
		m_VertexList[++index].pos = m_Frustum[2];
		m_VertexList[++index].pos = m_Frustum[6];
		m_VertexList[++index].pos = m_Frustum[3];
		m_VertexList[++index].pos = m_Frustum[7];
		// -X plane
		m_VertexList[++index].pos = m_Frustum[5];;
		m_VertexList[++index].pos = m_Frustum[1];
		m_VertexList[++index].pos = m_Frustum[4];
		m_VertexList[++index].pos = m_Frustum[0];
		// -y plane
		m_VertexList[++index].pos = m_Frustum[0];
		m_VertexList[++index].pos = m_Frustum[3];
		m_VertexList[++index].pos = m_Frustum[4];
		m_VertexList[++index].pos = m_Frustum[7];
		// +y plane
		m_VertexList[++index].pos = m_Frustum[5];
		m_VertexList[++index].pos = m_Frustum[6];
		m_VertexList[++index].pos = m_Frustum[1];
		m_VertexList[++index].pos = m_Frustum[2];
		m_pContext->UpdateSubresource(
			m_pVertexBuffer.Get(), 0, NULL, &m_VertexList.at(0), 0, 0);
	}
	
	m_Plane[0] = KPlane(m_Frustum[0], m_Frustum[1], m_Frustum[2]);  //-z

	m_Plane[1] = KPlane(m_Frustum[6], m_Frustum[5], m_Frustum[4]);	//+z

	m_Plane[2] = KPlane(m_Frustum[5], m_Frustum[1], m_Frustum[0]);	// -x

	m_Plane[3] = KPlane(m_Frustum[2], m_Frustum[6], m_Frustum[7]);	//+x

	m_Plane[4] = KPlane(m_Frustum[7], m_Frustum[4], m_Frustum[0]);	//-y

	m_Plane[5] = KPlane(m_Frustum[5], m_Frustum[6], m_Frustum[1]);	//+y
}

BOOL KFrustum::ClassifyPoint(KVector3* v)
{
	for (int i = 0; i < 6; i++)
	{
		float pToc = m_Plane[i].x * v->x +
			m_Plane[i].y * v->y +
			m_Plane[i].z * v->z +
			m_Plane[i].w;
		if (pToc > 0.0f)
		{
			return FALSE;
		}
	}
	return TRUE;
}

BOOL KFrustum::ClassifySphere(KSphere* v)
{
	for (int i = 0; i < 6; i++)
	{
		float pToc = m_Plane[i].x * v->vCenter.x +
			m_Plane[i].y * v->vCenter.y +
			m_Plane[i].z * v->vCenter.z + m_Plane[i].w;
		if (pToc >= v->fRadius)
		{
			return FALSE;
		}
	}
	return TRUE;
}

BOOL KFrustum::ClassifyOBB(KBox* box)
{
	float sum = 0.0f;
	KVector3 vDir;
	for (int i = 0; i < 6; i++)
	{
		vDir = box->Axis[0] * box->size.x;
		sum = fabs(m_Plane[i].x * vDir.x +
			m_Plane[i].y * vDir.y +
			m_Plane[i].z * vDir.z);
		vDir = box->Axis[1] * box->size.y;
		sum += fabs(m_Plane[i].x * vDir.x +
			m_Plane[i].y * vDir.y +
			m_Plane[i].z * vDir.z);
		vDir = box->Axis[2] * box->size.z;
		sum += fabs(m_Plane[i].x * vDir.x +
			m_Plane[i].y * vDir.y +
			m_Plane[i].z * vDir.z);

		float pToc = m_Plane[i].x * box->middle.x +
			m_Plane[i].y * box->middle.y +
			m_Plane[i].z * box->middle.z + m_Plane[i].w;
		if (pToc >= sum)
		{
			return FALSE;
		}
	}
	return TRUE;
}
