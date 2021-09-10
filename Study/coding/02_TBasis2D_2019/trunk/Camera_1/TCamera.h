#pragma once
#include "TUtils.h"

const D3DXVECTOR3 g_vUnitX(1, 0, 0);
const D3DXVECTOR3 g_vUnitY(0, 1, 0);
const D3DXVECTOR3 g_vUnitZ(0, 0, 1);

// used update = model view
class TCamera
{
public:
	D3DXVECTOR3 m_vEye;
	D3DXVECTOR3 m_vAt;
	D3DXVECTOR3 m_vUp;
	D3DXVECTOR3 m_vLook;
	D3DXVECTOR3 m_vRight;
	D3DXMATRIX  m_matView;

	float		m_fYaw;
	float		m_fPitch;
	float		m_fRoll;

	float		m_fFov;
	float		m_fAspect;
	float		m_fNearPlane;
	float		m_fFarPlane;
	D3DXMATRIX  m_matProj;

	float		m_fSpeed;
	float		m_fRadius;
	float		m_fWheelSpeed;
public:
	bool Init();
	bool Frame();
	bool SetViewMatrix( D3DXVECTOR3 vEye, D3DXVECTOR3 vAt,
						D3DXVECTOR3 vUp = D3DXVECTOR3(0, 1, 0));
	bool SetProjMatrix(float fFov, float fAspect, float fNear, float fFar);
	virtual void Left();
	virtual void Right();
	virtual void Up();
	virtual void Down();
	virtual void Front();
	virtual void Back();
	virtual void Update(D3DXVECTOR4& vDir);
public:
	TCamera();
	virtual ~TCamera();
};

