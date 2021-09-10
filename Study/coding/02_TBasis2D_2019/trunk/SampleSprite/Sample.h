#pragma once
#include "TCore.h"
#include "THero.h"
#include "TEnemy.h"
#include "TGround.h"
#include "TCollision.h"
#include "TParticleSystem.h"
const int g_iMaxNPC = 30;

typedef std::vector<RECT>  RECT_ARRAY;

class Sample : public TCore
{
public:
	float			m_fScrollX;
	TParticleSystem	m_ParticleSystem;
	DWORD			m_dwEffectType;
	float			m_fSumTimer;
	std::vector<RECT_ARRAY>  m_rtSpriteList;
public:
	TCollision	m_Collision;
	TGround		m_Ground;
	THero		m_Hero;
	TEnemy		m_npc[g_iMaxNPC];
	bool		m_bnpc[g_iMaxNPC];
	bool		m_bGameEnding;	

public:	
	bool		Opening();
	bool		Scene0();
	bool		Scene1();
	bool		Scene2();
	bool		GameDataLoad(TCHAR* pszLoad);
	bool		Init();
	bool		Frame();
	bool		Render();
	bool		Release();
public:
	Sample();
	virtual ~Sample();
};

