#pragma once
#include "TCharacter.h"

class TNpc : public TCharacter
{
	float	m_fTraceRange;		// 추격 범위
	float	m_fHitRange;		// 타격 범위
	float	m_fMovePoint;		// 이동 포인트
	float	m_fHitPoint;		// 타격 포인트	
public:	
	void MoveTo(const TCharacter& target);
	void HitPlayer(TCharacter& target);
	void Damaged(float damage);
	bool IsAlive();
	bool IsAttacking();
	bool IsPlayerInTraceRange(const TCharacter& target);
	bool IsPlayerInHitRange(const TCharacter& target);
	void SetLocation(float x, float y, float z);	
public:
	TNpc();
	virtual ~TNpc();
};

class TNpcManager : public TThread
{
public:
	map<int, TNpc> m_NpcList;
public:
	bool Init();
	bool Run();
	bool Set();
public:
	friend ostream& operator<<(ostream &stream, TNpcManager& npcList)
	{
		stream << npcList.m_NpcList.size() << endl;
		for (auto& npc : npcList.m_NpcList)
		{
			stream << npc.first << endl;
			stream << npc.second << endl;
		}
		return stream;
	}
	friend istream& operator>>(istream& stream, TNpcManager& npcList)
	{
		int iNumNpc = 0;
		int iID = 0;

		npcList.m_NpcList.clear();
		stream >> iNumNpc;

		TNpc tNpc;
		for (int i = 0; i < iNumNpc; i++)
		{
			stream >> iID;
			stream >> tNpc;
			npcList.m_NpcList[iID] = tNpc;
		}
		return stream;
	}
public:
	TNpcManager();
	virtual ~TNpcManager();
};
