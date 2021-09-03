#pragma once
#include "TProjectile.h"

class TGameUser : public TObject
{
public:
	bool		m_bAttack;	
	float		m_fKeyTimer;
	TPoint		m_tTargetPos;
	DWORD		m_dwMove;
	DWORD		m_dwPreMove;
	TEffect*    m_pProjectile;
	std::vector<TProjectileInfo>	m_ProjectileList;
public:
	bool Reset() override;
	bool Init() override;
	bool Frame() override;
	bool Render() override;
public:
	virtual TObject* Clone() override;
	virtual bool Damage();
	virtual bool Dead();
	virtual bool Hit(TPoint p, int iIndex);
	virtual bool ProjectileTick();
	TGameUser()
	{
		m_dwSvrID = 0;
		m_iObjectType = 100;
		m_fSpeed = 300.0f;
	}
};

