#pragma once
#include "KMapSpace.h"
#include "KDebugBoxRenderer.h"

class KMousePicker
{
public:
	enum ControlState
	{
		C_None,
		C_Height,
		C_Texture,
		C_Object,
	};
public:
	int					 m_iState = C_None;
public:
	ID3D11DeviceContext* m_pContext;
	KMapSpace*			 m_pSpace;
	KCamera*			 m_pCamera;
public:
	KVector3				m_vIntersect;
	KBox					m_Sel_Box;
	KDebugBoxRenderer		m_Sel_BoxRender;
	float					m_Sel_Brush_Size = 5.0f;
	float					m_Sel_Brush_Strenght= 50.0f;
public:
	bool PointInPolygon(KVector3 vert, KVector3 faceNormal,
		KVector3 v0, KVector3 v1, KVector3 v2);
	bool GetIntersect(KVector3 vStart, KVector3 vEnd,
		KVector3 v0,KVector3 vNormal);
	bool IntersectTriangle(const KVector3& orig, const KVector3& dir, KVector3& v0, KVector3& v1, KVector3& v2, FLOAT* t, FLOAT* u, FLOAT* v);
public:
	bool Init(ID3D11DeviceContext* pContext, KMapSpace* pSpace, KCamera* pCam);
	bool Frame();
	bool Render(ID3D11DeviceContext* pContext);
	bool Release();
public:
	bool Map_HeightControl(KNode* pNode, KVector3& v0, KVector3& v1, KVector3& v2, DWORD i0, DWORD i1, DWORD i2, float HeightScale, float BrushSize);
	bool Map_HeightControl_MakeSameHeight(KNode* pNode);
public:
	KMousePicker();
	~KMousePicker();
};

