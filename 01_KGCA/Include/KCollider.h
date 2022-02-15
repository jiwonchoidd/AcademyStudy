#pragma once
#include "KStd.h"
enum KCollisionType
{
	Block = 0,
	Overlap,
	Ignore,
};
class KCollider
{
public:
	int					m_ID;
	DWORD				m_CollisonType;
	KRect*				m_rtColl;
	KVector2*			m_pos;
public:
	virtual void	HitOverlap(KCollider* pObj, DWORD dwState);
public:

	KCollider()
	{
		m_ID = -1;
		m_pos = nullptr;
		m_rtColl = nullptr;
		m_CollisonType = Overlap;
	}
};

