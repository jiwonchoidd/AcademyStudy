#pragma once
#include "TFiniteState.h"
class TFiniteStateMachine : public TSingleton<TFiniteStateMachine>
{
	friend class TSingleton<TFiniteStateMachine>;
private:
	//        STAND   STAND-CLASS
	//        인덱스  상태클래스
	std::map< DWORD, TFiniteState*> m_mapState;
public:
	void    AddStateTrasition(  DWORD dwState, 
								DWORD dwEvent, 
								DWORD dwOutputState);
	DWORD	GetTransition(DWORD dwState, DWORD dwEvent);
	TFiniteStateMachine() = default;
	~TFiniteStateMachine();
};

#define g_Fsm TSingleton<TFiniteStateMachine>::GetInstance()