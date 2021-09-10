#pragma once
#include "TStd.h"
enum TFSMState
{
	STATE_STAND = 0,
	STATE_MOVE,
	STATE_ATTACK,
	STATE_COUNT,
};
enum TFSMEvent
{
	EVENT_STOPMOVE = 0,
	EVENT_TIMEMOVE,
	EVENT_POINTMOVE,
	EVENT_FINDTARGET,
	EVENT_LOSTTARGET,
	EVENT_COUNT,
};
// 현재상태         이벤트				전이상태
// STAND         EVENT_TIMEMOVE           MOVE
// STAND         EVENT_POINTMOVE          MOVE
// STAND         EVENT_FINDTARGET         ATTACK
// MOVE          EVENT_STOPMOVE           STAND
// ATTACK        EVENT_LOSTTARGET         STAND
class TFiniteState
{
public:
	//         EVENT_TIMEMOVE           MOVE
	//         EVENT_POINTMOVE          MOVE
	//         EVENT_FINDTARGET         ATTACK
	//        이벤트, 전이상태
	std::map< DWORD, DWORD> m_mapList;
	DWORD					m_dwState;
public:
	void			AddTransition(DWORD dwEvent, 
								 DWORD dwOutputStatee);
public:
	TFiniteState(DWORD dwState)
	{
		m_dwState = dwState;
	}
};

