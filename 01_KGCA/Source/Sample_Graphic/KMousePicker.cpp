#include "KMousePicker.h"
bool KMousePicker::PointInPolygon(KVector3 vert, KVector3 faceNormal,
	KVector3 v0, KVector3 v1, KVector3 v2)
{
	KVector3 e0, e1, e2, iInter, vNormal;
	e0 = v1 - v0;
	e1 = v2 - v1;
	e2 = v0 - v2;

	iInter = vert - v0;
	D3DXVec3Cross(&vNormal, &e0, &iInter);
	D3DXVec3Normalize(&vNormal, &vNormal);
	float fDot = D3DXVec3Dot(&faceNormal, &vNormal);
	if (fDot < 0.0f) return false;

	iInter = vert - v1;
	D3DXVec3Cross(&vNormal, &e1, &iInter);
	D3DXVec3Normalize(&vNormal, &vNormal);
	fDot = D3DXVec3Dot(&faceNormal, &vNormal);
	if (fDot < 0.0f) return false;

	iInter = vert - v2;
	D3DXVec3Cross(&vNormal, &e2, &iInter);
	D3DXVec3Normalize(&vNormal, &vNormal);
	fDot = D3DXVec3Dot(&faceNormal, &vNormal);
	if (fDot < 0.0f) return false;
	return true;
}

//���̽��� ���̺��ٴ� ���̿� �� ���̿� �ڽ� ���·� ����� ����
bool KMousePicker::GetIntersect(KVector3 vStart, KVector3 vEnd,
	KVector3 v0, KVector3 v1, KVector3 v2,
	KVector3 vNormal)
{
	KVector3 vDirection = vEnd - vStart;
	KVector3 v0toStart = v0 - vStart;
	float A = D3DXVec3Dot(&vNormal, &vDirection);
	float a = D3DXVec3Dot(&vNormal, &v0toStart);
	float t = a / A;
	if (t < 0.0f || t > 1.0f)
	{
		return false;
	}
	m_vIntersect = vStart + vDirection * t;
	return true;
}

bool KMousePicker::IntersectTriangle(const KVector3& orig, const KVector3& dir, KVector3& v0, KVector3& v1, KVector3& v2, FLOAT* t, FLOAT* u, FLOAT* v)
{
	// Find vectors for two edges sharing vert0
	KVector3 edge1 = v1 - v0;
	KVector3 edge2 = v2 - v0;

	// Begin calculating determinant - also used to calculate U parameter
	KVector3 pvec;
	D3DXVec3Cross(&pvec, &dir, &edge2);

	// If determinant is near zero, ray lies in plane of triangle
	FLOAT det = D3DXVec3Dot(&edge1, &pvec);

	KVector3 tvec; // ������ ����� �� �� �ֵ��� det ������ �����´�.
	if (det > 0)
	{
		tvec = orig - v0;
	}
	else
	{
		tvec = v0 - orig;
		det = -det;
	}

	if (det < 0.0001f)
		return false;

	// Calculate U parameter and test bounds
	*u = D3DXVec3Dot(&tvec, &pvec);
	if (*u < 0.0f || *u > det)
		return false;

	// Prepare to test V parameter
	KVector3 qvec;
	D3DXVec3Cross(&qvec, &tvec, &edge1);

	// Calculate V parameter and test bounds
	*v = D3DXVec3Dot(&dir, &qvec);
	if (*v < 0.0f || *u + *v > det)
		return false;

	// Calculate t, scale parameters, ray intersects triangle
	*t = D3DXVec3Dot(&edge2, &qvec);
	FLOAT fInvDet = 1.0f / det;
	*t *= fInvDet;
	*u *= fInvDet;
	*v *= fInvDet;
	return true;
}

bool KMousePicker::Init(ID3D11DeviceContext* pContext, KMapSpace* pSpace, KCamera* pCam)
{
	m_pContext = pContext;
	m_pSpace = pSpace;
	m_pCamera = pCam;
	return true;
}

bool KMousePicker::Frame()
{
	//���콺 ��ŷ
	//������ ���콺 ��������,
	if (g_InputData.bMouseState[0])
	{
		//ȭ�� ��ǥ���̱⶧����, y�� ����
		//ȭ�� ũ��
		POINT ptCursor;
		GetCursorPos(&ptCursor);
		ScreenToClient(g_hWnd, &ptCursor);
		KVector3 vView, vProj;

		//Direction ���
		//���� �Ʒ� ������δ� �̴ϸ� Ŭ���� �����, ������ ��ü ȭ�� ũ�� ��������
		//�����ؼ�..�ٵ� �̴ϸʿ��� Ŭ���� ���� ������;
		vProj.x = (((2.0f * ptCursor.x - 2.0f * g_rtClient.left) / g_rtClient.right) - 1);
		vProj.y = -(((2.0f * ptCursor.y - 2.0f * g_rtClient.top) / g_rtClient.bottom) - 1);
		vProj.z = 1.0f;
		vView.x = vProj.x / m_pCamera->m_matProj._11;
		vView.y = vProj.y / m_pCamera->m_matProj._22;
		vView.z = vProj.z;

		//����ǥ�迡���� ������ ������ 0,0,0
		KMatrix matInverse;
		D3DKMatrixInverse(&matInverse, nullptr, &m_pCamera->m_matView);

		//ī�޶��� ���� ��ǥ�� ���̰� �����.
		TRay ray;
		//ray.position = KVector3(0, 0, 0);
		//ray.direction = v;
		//ray.position = ray.position * matInverse;
		//ray.direction= ray.direction * matInverse;
		ray.direction.x = vView.x * matInverse._11 + vView.y * matInverse._21 + vView.z * matInverse._31;
		ray.direction.y = vView.x * matInverse._12 + vView.y * matInverse._22 + vView.z * matInverse._32;
		ray.direction.z = vView.x * matInverse._13 + vView.y * matInverse._23 + vView.z * matInverse._33;

		ray.position.x = matInverse._41;
		ray.position.y = matInverse._42;
		ray.position.z = matInverse._43;
		//����ȭ
		D3DXVec3Normalize(&ray.direction, &ray.direction);
		KVector3  vStart = ray.position;
		KVector3  vEnd = ray.position + ray.direction * m_pCamera->m_fFar;

		for (int iNode = 0; iNode < m_pSpace->m_pDrawableLeafList.size(); iNode++)
		{
			KNode* pNode = m_pSpace->m_pDrawableLeafList[iNode];

			for (int i = 0; i < m_pSpace->m_IndexList.size(); i += 3)
			{
				KVector3 v0, v1, v2;
				DWORD i0 = m_pSpace->m_IndexList[i + 0];
				DWORD i1 = m_pSpace->m_IndexList[i + 1];
				DWORD i2 = m_pSpace->m_IndexList[i + 2];
				v0 = pNode->m_VertexList[i0].pos;
				v1 = pNode->m_VertexList[i1].pos;
				v2 = pNode->m_VertexList[i2].pos;

				//2�� ����
				float t, u, v;
				if (IntersectTriangle(ray.position, ray.direction,
					v0, v1, v2, &t, &u, &v))
				{
					m_vIntersect = ray.position + ray.direction * t;
					pNode->m_VertexList[i0].color = KVector4(1, 0, 0, 1);
					pNode->m_VertexList[i1].color = KVector4(1, 0, 0, 1);
					pNode->m_VertexList[i2].color = KVector4(1, 0, 0, 1);
					m_pContext->UpdateSubresource(
						pNode->m_pVertexBuffer.Get(), 0, NULL,
						&pNode->m_VertexList.at(0), 0, 0);
				}
			}
		}
	}
	return true;
}

bool KMousePicker::Release()
{
	return true;
}

KMousePicker::KMousePicker()
{
}

KMousePicker::~KMousePicker()
{
}
