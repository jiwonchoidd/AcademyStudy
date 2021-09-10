#include "TFiniteStateMachine.h"

void TFiniteStateMachine::AddStateTrasition(
	DWORD dwState, DWORD dwEvent, DWORD dwOutputState)
{
	TFiniteState* pState = nullptr;
	auto ret = m_mapState.find(dwState);
	if (ret != m_mapState.end())
	{
		pState = ret->second;
	}
	else
	{
		pState = new TFiniteState(dwState);
		m_mapState[dwState] = pState;
	}	
	pState->AddTransition(dwEvent, dwOutputState);
}
DWORD TFiniteStateMachine::GetTransition(DWORD dwState, 
	DWORD dwEvent)
{
	return m_mapState[dwState]->m_mapList[dwEvent];
}
TFiniteStateMachine::~TFiniteStateMachine()
{
	for (auto list : m_mapState)
	{
		delete list.second;
	}
	m_mapState.clear();
}