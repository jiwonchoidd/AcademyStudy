#pragma once
#include "TShape.h"

struct T_Plane
{
	// normal = { fA, fB, fC } 
	// fD =  원점으로 부터 떨어지 거리
	// V = {x,y,z }
	//  fA * x + fB * y + fC * z + fD = 평면방정식
	float fA, fB, fC, fD;
	bool CreatePlane(D3DXVECTOR3 v0, D3DXVECTOR3 v1, D3DXVECTOR3 v2);
	bool CreatePlane(D3DXVECTOR3 v0, D3DXVECTOR3 vNoraml);
};
class TFrustum
{
public:
	D3DXMATRIX   m_matWorld;
	D3DXMATRIX   m_matView;
	D3DXMATRIX   m_matProj;
	D3DXVECTOR3  m_vBaseFrustum[8];
	D3DXVECTOR3  m_vFrustum[8];
	T_Plane      m_Plane[6];
public:
	bool    Init();
	bool	ClassifyPoint(D3DXVECTOR3 v);
	bool    CheckOBBInPlane(T_Box* box);
	T_POSITION  CheckPositionOBBInPlane(T_Box* box);
	void	SetMatrix(D3DXMATRIX* pWorld,
		D3DXMATRIX* pView,
		D3DXMATRIX* pProj);
	void    CreateFrustum();
public:
	TFrustum();
	virtual ~TFrustum();
};

