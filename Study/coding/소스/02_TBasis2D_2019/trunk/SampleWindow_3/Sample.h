#pragma once
#include "TCore.h"
#include "TEnemy.h"
#include "THero.h"
#define MAX_OBJECT 10
class Sample : public TCore
{
public:
	TEnemy		m_BackGround;
	THero		m_Hero;
	TEnemy		m_Object[MAX_OBJECT];
	float			m_iX;
	float			m_iY;
public:
	bool	 Init();
	bool	 Frame();
	bool	 Render();
	bool	 Release();
public:
	Sample();
	virtual ~Sample();
};

