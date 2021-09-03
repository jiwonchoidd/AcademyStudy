#pragma once
#include "TCore.h"
#include "xObject.h"
#include "TCollision.h"
#include "TSound.h"

const int g_iMaxNPC = 30;

class Sample : public TCore
{
public:
	TSound		m_Sound;
	TCollision	m_Collision;
	xObject		m_Ground;
	xObject		m_Hero;
	xObject		m_npc[g_iMaxNPC];
	bool		m_bnpc[g_iMaxNPC];
	bool		m_bGameEnding;	
public:
	
	bool		Scene0();
	bool		Scene1();
	bool		Scene2();
	bool		Init();
	bool		Frame();
	bool		Render();
	bool		Release();

public:
	Sample();
	virtual ~Sample();
};

