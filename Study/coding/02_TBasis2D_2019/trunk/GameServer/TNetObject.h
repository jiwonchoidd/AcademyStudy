#pragma once
#include "TNpcObject.h"
#include "TNetStandState.h"
#include "TNetMoveState.h"
#include "TNetAttackState.h"

class TChatUser;
class TNetObject : public TNpcObject
{
public:
	float		m_fAttackTimer = 1.0f;
	TChatUser*  m_pTargetUser;
public:
	bool Frame() override;
	bool Render() override;
	void FSM() override;
public:
	virtual bool Damage()				override;
	virtual bool Dead()					override;
	virtual void Attack(TObject* pPlayer)override;
	virtual void  SetPos(TPoint p) override ;
};

