#include "TFiniteState.h"

void TFiniteState::AddTransition(DWORD dwEvent, DWORD dwOutputState)
{
	m_mapList[dwEvent] = dwOutputState;
}
