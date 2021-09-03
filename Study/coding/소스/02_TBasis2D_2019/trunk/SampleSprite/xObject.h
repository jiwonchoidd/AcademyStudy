#pragma once
#include "TBitmapMgr.h"
class xObject : public TShape
{
public:
	float		m_fSpeed;
public:
	bool		Init();
	bool		Frame();
	bool		Release();
public:
	void		SetSpeed(float fSpeed = 10.0f);
	void		Left();
	void		Right();
	void		Up();
	void		Down();
public:
	xObject();
	virtual ~xObject();
};

