#pragma once
#include "KCore.h"
class Sample : public KCore
{
	KVector4			m_lightPos;
	KPlane				m_Plane;
	KFbxObj				m_FbxObjA;
	KFbxObj				m_FbxObjB;
	KTexture			m_Texture;
	KMatrix				m_matShadow;
	ID3D11PixelShader*	m_pPSShadow = nullptr;
public:
	bool		Init()override;
	bool		Frame()override;
	bool		Render()override;
	bool		Release()override;
public:
	Sample();
	virtual ~Sample();
};

