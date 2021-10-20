#pragma once
#include "TStd.h"
class KCamera
{
public:
	float				m_pSpeed;
	KVector3			m_vCameraPos;
	KVector3			m_vCameraTarget;
	KVector3			m_vLook;
	KVector3			m_vSide;
	KVector3			m_vUp;
public:
	KMatrix m_matWorld;
	KMatrix m_matView;
	KMatrix m_matProj;
public:
	virtual KMatrix     CreateViewMatrix(KVector3 vPos, KVector3 vTarget, KVector3 vUp= KVector3(0,1,0));
	virtual KMatrix  	CreateProjMatrix(float fNear, float fFar, float fFov, float fAspect);
public:
	virtual bool		Init();
	virtual bool		Frame();
	virtual bool		Render();
	virtual bool		Release();
	
public:
	KCamera();
	virtual ~KCamera();
};

class KDebugCamera : public KCamera
{
public:
	virtual bool		Frame() override;
	virtual KMatrix		Update(KVector4 vValue);
};
