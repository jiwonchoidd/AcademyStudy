#pragma once
#include "KCore.h"
class Sample : public KCore
{
	KFbxObj		m_FbxObjA;
	KFbxObj		m_FbxObjB;
	KTexture	m_Texture;
	float               m_fYRot = 0;
	KVector3			m_MovePos;
public:
	bool		Init()override;
	bool		Frame()override;
	bool		Render()override;
	bool		Release()override;
public:
	Sample();
	virtual ~Sample();
};

