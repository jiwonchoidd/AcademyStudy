#pragma once
#include "TCharacter.h"

class TNpc 
{
public:
	// Sets default values for this character's properties
	TNpc();
public:
	tGUID	m_tGuid;		// 플레이어 고유 아이디
	float   m_fHealth;
public:	
	void  UpdateHealth(float HealthChange);
	float GetHealth();
	void  MoveToLocation(const FVector& dest);
	void  AttackMontage() {};
	void  Dead() {};
	void  HitReact() {};
};