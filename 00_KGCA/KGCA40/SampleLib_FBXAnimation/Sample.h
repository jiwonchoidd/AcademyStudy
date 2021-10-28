#pragma once
#include "KCore.h"
class Sample : public KCore
{
	KFbxObj		m_FbxObjA;
	KFbxObj		m_FbxObjB;
	KTexture	m_Texture;
public:
	bool		Init()override;
	bool		Frame()override;
	bool		Render()override;
	bool		Release()override;
public:
	Sample();
	virtual ~Sample();
};

