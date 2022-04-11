#pragma once
#include "KStd.h"
#include "KMapSpace.h"
class KMousePicker
{
public:
	ID3D11DeviceContext* m_pContext;
	KMapSpace*			 m_pSpace;
	KCamera*			 m_pCamera;
public:
	KVector3 m_vIntersect;
public:
	bool PointInPolygon(KVector3 vert, KVector3 faceNormal,
		KVector3 v0, KVector3 v1, KVector3 v2);
	bool GetIntersect(KVector3 vStart, KVector3 vEnd,
		KVector3 v0, KVector3 v1, KVector3 v2,
		KVector3 vNormal);
	bool IntersectTriangle(const KVector3& orig, const KVector3& dir, KVector3& v0, KVector3& v1, KVector3& v2, FLOAT* t, FLOAT* u, FLOAT* v);
public:
	bool Init(ID3D11DeviceContext* pContext, KMapSpace* pSpace, KCamera* pCam);
	bool Frame();
	bool Release();
public:
	KMousePicker();
	~KMousePicker();
};

