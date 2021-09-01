#pragma once
#include "TNpcObject.h"
#include "TProtocol.h"
class TNpcAirObject : public TNpcObject
{
public:
	DWORD		m_dwSvrID;
	bool		m_bAttack;	
	TPoint		m_tTargetPos;
public:
	TPACKET_CHARACTER_INFO m_Info;
public:
	virtual TObject* Clone() override;
	bool Frame() override;
	bool Render() override;
public:
	virtual bool Damage()				override;
	virtual bool Dead()					override;
	virtual void Attack(TObject* pPlayer)override;
};

