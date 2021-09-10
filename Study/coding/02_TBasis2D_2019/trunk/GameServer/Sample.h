#pragma once
#define SYNC_NPC_DIRECTION
#include "TCore.h"

class Sample : public TCore
{
public:
	static int  g_iNpcIndex;
	float		m_fNpcSpawnInterval;
	float		m_fNpcSpawnTimer;
	int			m_iMaxNpcSpawn;

	float		m_fSyncNpcTimer;
	float		m_fSyncNpcInterval;
public:
	bool  Init();
	bool  Frame();
	bool  Render();
	bool  Release();
	void  SendAllNpcMovement();
	void  SendNpcMovement(int iIndex);
	void  SendNpcSpawn(int iIndex);
};
