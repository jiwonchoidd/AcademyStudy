#pragma once
#include "TStd.h"
#include <map>
#include <functional>
enum State
{
	STATE_STAND = 0,
	STATE_MOVE,
	STATE_ATTACK,
	STATE_RUNAWAY, // 도망가다
	// STATE_CHASE, // 추격하다.//PURSUIT
	STATE_COUNT
};

enum Event
{
	EVENT_FINDTARGET = 0,
	EVENT_LOSTTARGET,
	EVENT_BEATTACKED,
	EVENT_HEALTHDRYUP,
	EVENT_STOPWALK,
	EVENT_PATROL
};


class TFiniteStateMachine;

class TFiniteState
{
private:
	friend class TFiniteStateMachine;
private:
	DWORD		m_dwState;
	std::map	< DWORD, DWORD >		m_Map;
private:
	DWORD		GetState() { return m_dwState; }
	void		AddTransition(DWORD inputEvent, DWORD outputStateID);
	void		DeleteTransition(DWORD inputEvent);
	DWORD		Output(DWORD inputEvent);
	DWORD		GetCount();
private:
	TFiniteState(DWORD state);
	virtual ~TFiniteState();
};


