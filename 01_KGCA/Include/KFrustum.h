#pragma once
#include "KBoxObj.h"
class KFrustum : public KBoxObj
{
public:
	KPlane		m_Plane[6];
	KVector3	m_Frustum[8];
public:
	bool		Init();
public:
	void CreateFrustum(KMatrix& matView, KMatrix& matProj);
	BOOL ClassifyPoint(KVector3* v);
	BOOL ClassifySphere(KSphere* v);
	BOOL ClassifyOBB(KBox* v);
};

