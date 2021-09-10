#pragma once
#include "TBitmapMgr.h"

class xObject : public TShape
{
public:
	T_STR		m_szName;
	T_STR		m_szPath;
	int			m_iIndex;
	int			m_iDirection[2];
	float		m_fSpeed;	
public:
	bool		Init();
	bool		Frame();
	bool		Release();
public:
	void		SetSpeed(float fSpeed = 10.0f);
	void		SetHero(bool bHero = true);
	void		Left();
	void		Right();
	void		Up();
	void		Down();
public:
	xObject();
	virtual ~xObject();
};

