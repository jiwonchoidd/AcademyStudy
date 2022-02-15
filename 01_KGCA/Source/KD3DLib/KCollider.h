#pragma once
#include "KStd.h"
enum KCollisionType
{
	Block = 0,
	Overlap,
	Ignore,
};
struct Rt_Size
{
	float width;
	float height;
};
class KCollider
{
public:
	int					m_ID;
	DWORD				m_CollisonType;
	KRect				m_rtColl;
	KVector2			m_pos;
	KVector2			m_dir;
	Rt_Size				m_rtSize; //사이즈 구조체
public:
	virtual void	HitOverlap(KCollider* pObj, DWORD dwState);
public:

	KCollider()
	{
		m_rtSize = { 0,0 };
		m_ID = -1;
		m_pos = {0,0};
		m_CollisonType = Overlap;
	}
};

