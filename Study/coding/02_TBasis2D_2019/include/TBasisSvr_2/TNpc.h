#pragma once
#include "TCharacter.h"

class TNpc : public TCharacter
{
	float	m_fTraceRange;		// 추격 범위
	float	m_fHitRange;		// 타격 범위
	float	m_fMovePoint;		// 이동 포인트
	float	m_fHitPoint;		// 타격 포인트	
public:	
	void SendMove(TCharacter& pDesk);
	void MoveTo( TCharacter& target);
	void HitPlayer(TCharacter& target);
	void Damaged(float damage);
	bool IsAlive();
	bool IsAttacking();
	bool MoveChecker(const TCharacter& target);
	bool AttartChecker(const TCharacter& target);
	void SetLocation(float x, float y, float z);	
	
public:
	TNpc();
	virtual ~TNpc();
};

class TNpcManager : public TThread
{
public:
	map<tGUID, TNpc, GUIDComparer> m_NpcList;
	TCharacter* m_pTargetChar;
public:
	bool Init();
	bool Run();
	bool Set();
	void Spawn();
	void SetTargetPoint();
	void SendAttack(TNpc* pNpc, TCharacter* pDesk);
	void SendMove(TNpc* pNpc, TCharacter* pDesk);
public:
	friend ostream& operator<<(ostream &stream, TNpcManager& npcList)
	{
		stream << npcList.m_NpcList.size();
		for (auto& npc : npcList.m_NpcList)
		{
			stream << npc.first;
			stream << npc.second;
		}
		return stream;
	}
	friend istream& operator>>(istream& stream, TNpcManager& npcList)
	{
		npcList.m_NpcList.clear();		
		
		tGUID tGuid;
		TNpc tNpc;
		size_t iNumNpc = 0;
		stream >> iNumNpc;
		
		for (int i = 0; i < iNumNpc; i++)
		{
			stream >> tGuid;
			stream >> tNpc;
			npcList.m_NpcList[tGuid] = tNpc;
		}
		return stream;
	}
public:
	TNpcManager();
	virtual ~TNpcManager();
};
