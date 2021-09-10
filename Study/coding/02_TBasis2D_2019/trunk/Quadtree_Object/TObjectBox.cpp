#include "TObjectBox.h"

void TMapObj::Set(TObjectBox* pBaseShape, DWORD dwWidth, DWORD dwHeight)
{
	DWORD dwScaleWidth = dwWidth - 5.0f;
	DWORD dwScaleHeight = dwHeight - 5.0f;
	m_pBaseShape = pBaseShape;
	float fRandX = dwScaleWidth * 0.5f - (rand() % dwScaleWidth);
	float fRandY = 0.0f;
	float fRandZ = dwScaleHeight * 0.5f - (rand() % dwScaleHeight);
	D3DXVECTOR3 vPos(fRandX, fRandY, fRandZ);
	D3DXMatrixIdentity(&m_matWorld);
	m_matWorld._11 = (rand() % 255) / 254.0f * 2.0f;
	m_matWorld._22 = (rand() % 255) / 254.0f * 2.0f;
	m_matWorld._33 = (rand() % 255) / 254.0f * 2.0f;

	m_matWorld._41 = vPos.x;
	m_matWorld._42 = vPos.y;
	m_matWorld._43 = vPos.z;
	D3DXVec3TransformCoord(&m_tBox.vMax, &D3DXVECTOR3(1, 1, 1), &m_matWorld);
	D3DXVec3TransformCoord(&m_tBox.vMin, &D3DXVECTOR3(-1, -1, -1), &m_matWorld);
}

TObjectBox::TObjectBox()
{
}


TObjectBox::~TObjectBox()
{
}
