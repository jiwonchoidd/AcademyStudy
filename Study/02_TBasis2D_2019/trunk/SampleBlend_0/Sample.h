#pragma once
#include "TCore.h"
#include "xObject.h"
#include "TCollision.h"

#pragma comment (lib, "msimg32.lib")

const int g_iMaxNPC = 30;

class Sample : public TCore
{
public:
	TCollision	m_Collision;
	xObject		m_Lobby;
	xObject		m_Ground;
	xObject		m_Hero;
	xObject		m_npc[g_iMaxNPC];
	bool		m_bnpc[g_iMaxNPC];
	bool		m_bGameEnding;	
	bool		m_bGameStart;
	float		m_fDirection;
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

