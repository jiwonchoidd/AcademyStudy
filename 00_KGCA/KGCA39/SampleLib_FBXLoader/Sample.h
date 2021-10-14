#pragma once
#include "TCore.h"
class Sample : public TCore
{
	KFbxObj		m_FbxObj;
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

