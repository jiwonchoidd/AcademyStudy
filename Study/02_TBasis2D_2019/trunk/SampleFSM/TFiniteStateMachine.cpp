#include "TFiniteStateMachine.h"
InvalidCurrentStateException::InvalidCurrentStateException(int curStateID)
	: currentStateID(curStateID)
{
}

InvalidCurrentStateException::~InvalidCurrentStateException()
{
}


InvalidInputEventException::InvalidInputEventException(int event)
	: inputEvent(event)
{
}

InvalidInputEventException::~InvalidInputEventException()
{
}




//------------------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------- Finite State Machine ----------------------------------------------------

TFiniteStateMachine::TFiniteStateMachine()
{
}

TFiniteStateMachine::~TFiniteStateMachine()
{
	Release();
}
bool TFiniteStateMachine::Load(const TCHAR* szFileName)
{
	return true;
}
bool TFiniteStateMachine::Release()
{
	std::map < DWORD, TFiniteState* >::iterator iter, iterPrev;

	iter = mapState.begin();
	while (iter != mapState.end())
	{
		iterPrev = iter++;
		delete iterPrev->second;
		mapState.erase(iterPrev);
	}
	return false;
}
void TFiniteStateMachine::AddStateTransition(DWORD m_dwState, DWORD inputEvent, DWORD outputStateID)
{
	std::map < DWORD, TFiniteState* >::iterator iter;
	TFiniteState* state= NULL;

	//  동일한 State가 이미 존재하는지를 찾아낸다.
	iter = mapState.begin();
	while (iter != mapState.end())
	{
		state = iter->second;
		if (state->GetState() == m_dwState)
			break;
		iter++;
	}

	//  만일 동일한 State가 존재하지 않는다면 새로 생성한다.
	if (iter == mapState.end())
	{
		state = new TFiniteState(m_dwState);
		mapState[state->GetState()] = state;
	}

	//  상태 전이 정보를 추가한다.
	if (state != NULL)
	{
		state->AddTransition(inputEvent, outputStateID);
	}
}

void TFiniteStateMachine::DeleteTransition(DWORD m_dwState, DWORD inputEvent)
{
	std::map < DWORD, TFiniteState* >::iterator iter, iterPrev;
	TFiniteState* state=NULL;

	//  해당 State를 찾는다.
	iter = mapState.begin();
	while (iter != mapState.end())
	{
		iterPrev = iter;
		state = iter->second;

		if (state->GetState() == m_dwState)
			break;

		iter++;
	}

	if (iter == mapState.end())
		return;

	//  State의 전이 정보를 지운다.
	if (state != NULL)
	{
		state->DeleteTransition(inputEvent);
	}
	if (state->GetCount() == 0)
	{
		delete state;
		mapState.erase(iterPrev);
	}
}

DWORD TFiniteStateMachine::GetOutputState(DWORD inputEvent) 
{
	std::map < DWORD, TFiniteState* >::iterator iter;
	

	//  만일 존재하지 않는 State가 입력되었다면, Exception을 발생시킨다.
	iter = mapState.find(inputEvent);
	if (iter == mapState.end())
		throw (new InvalidCurrentStateException(inputEvent));

	TFiniteState* state = mapState[inputEvent];
	if(state == NULL)
	{
		throw (new InvalidCurrentStateException(inputEvent));
	}
	return state->Output(inputEvent);
}

DWORD TFiniteStateMachine::GetCurrentStateID(DWORD Output)
{
	TFiniteState*  currentState = mapState[Output];
	return currentState->GetState();
}

DWORD TFiniteStateMachine::StateTransition(DWORD dwCur, int event) 
{
	DWORD Output;
	try
	{
		Output = mapState[dwCur]->Output(event);
	}
	catch (InvalidInputEventException* ex)
	{
		delete ex;
		return -1;
	}
	return Output;
}
/////////////////////////////////////////////////////////////////////////////
// 
/////////////////////////////////////////////////////////////////////////////
INT TFSMMgr::Add( const TCHAR* szLoadFileName)
{
	TCHAR szFileName[MAX_PATH];
	TCHAR Drive[MAX_PATH];
	TCHAR Dir[MAX_PATH];
	TCHAR FName[MAX_PATH];
	TCHAR Ext[MAX_PATH];

	// 기본 유한상태기기를 사용한다.
	if (szLoadFileName == NULL)
	{
		szLoadFileName = _T("DefaultEnemy");
	}

	if (szLoadFileName)
	{
		_tsplitpath_s(szLoadFileName, Drive, Dir, FName, Ext);
		Ext[4] = 0;
		memset(szFileName, 0, sizeof(TCHAR) * MAX_PATH);
		_stprintf_s(szFileName, _T("%s%s"), FName, Ext);

		for (TemplateMapItor itor = TMap.begin(); itor != TMap.end(); itor++)
		{
			TFiniteStateMachine *pPoint = (TFiniteStateMachine *)(*itor).second;
			if (!_tcsicmp(pPoint->m_szName.c_str(), szFileName))
			{
				return (*itor).first;
			}
		}
	}	

	TFiniteStateMachine* pPoint = new TFiniteStateMachine;
	// 파일에 저장되어 있는 유한상태기기를 사용한다.
	if (!pPoint->Load(szLoadFileName))
	{
		SAFE_DEL(pPoint);
		return 1; // 기본값 반환한다. 
	}
	pPoint->m_szName = szFileName;
	TMap.insert(make_pair(++m_iCurIndex, pPoint));
	pPoint->m_iIndex = m_iCurIndex;
	return m_iCurIndex;
}
bool TFSMMgr::Release()
{
	TFiniteStateMachine *pPoint;
	for (TemplateMapItor itor = TMap.begin(); itor != TMap.end(); itor++)
	{
		pPoint = (TFiniteStateMachine *)(*itor).second;
		pPoint->Release();
		SAFE_DEL(pPoint);
	}
	TMap.clear();
	m_iCurIndex = 0;
	return true;
}
TFiniteStateMachine* TFSMMgr::GetPtr(INT iIndex)
{
	TemplateMapItor itor = TMap.find(iIndex);
	if (itor == TMap.end()) return NULL;
	TFiniteStateMachine *pPoint = (*itor).second;
	return pPoint;
}
bool	TFSMMgr::Delete(INT iDelete)
{
	return true;
}
bool TFSMMgr::Init()
{
	TFiniteStateMachine* pPoint = new TFiniteStateMachine();
	pPoint->AddStateTransition(STATE_STAND, EVENT_FINDTARGET, STATE_ATTACK);
	pPoint->AddStateTransition(STATE_STAND, EVENT_BEATTACKED, STATE_ATTACK);
	pPoint->AddStateTransition(STATE_STAND, EVENT_PATROL, STATE_MOVE);		// 정찰

	pPoint->AddStateTransition(STATE_MOVE, EVENT_FINDTARGET, STATE_ATTACK);
	pPoint->AddStateTransition(STATE_MOVE, EVENT_STOPWALK, STATE_STAND);

	pPoint->AddStateTransition(STATE_ATTACK, EVENT_LOSTTARGET, STATE_STAND);
	pPoint->AddStateTransition(STATE_ATTACK, EVENT_HEALTHDRYUP, STATE_RUNAWAY);

	pPoint->AddStateTransition(STATE_RUNAWAY, EVENT_LOSTTARGET, STATE_STAND); // 도망/회피(AVOID)
	//pPoint->AddStateTransition(STATE_CHASE, EVENT_FINDTARGET, STATE_ATTACK);  // 추격

	pPoint->m_szName = _T("DefaultEnemy");
	TMap.insert(make_pair(++m_iCurIndex, pPoint));
	pPoint->m_iIndex = m_iCurIndex;
	return true;
}
TFSMMgr::TFSMMgr()
{

}
TFSMMgr::~TFSMMgr()
{
}
