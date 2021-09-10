#include "TNpcObject.h"
#include "TFiniteStateMachine.h"
#include "TScene.h"
#include "TObjectMgr.h"
TObject* TNpcObject::Clone()
{
	return new TNpcObject(*this);
}
void TNpcObject::FSM()
{
	m_pActionList.push_back(new TStandState(this));
	m_pActionList.push_back(new TMoveState(this));
	m_pActionList.push_back(new TAttackState(this));
	m_pAction = m_pActionList[0];
	
	m_fAttackTimer = 1.0f;
	m_pProjectile = (TEffect*)g_ObjectMgr.GetPtr(L"rtProjectile");

}
void TNpcObject::SetTransition(DWORD dwEvent)
{
	DWORD dwOutput = g_Fsm.GetTransition(m_pAction->m_dwState, 
									dwEvent);
	m_pAction = m_pActionList[dwOutput];
}
bool TNpcObject::Frame()
{
	if (m_bDead == true) return true;
	m_pAction->Process(TScene::m_pGamePlayer);	
	return true;
}
bool TNpcObject::Render()
{
	TObject::Render();	
	return true;
}

bool TNpcObject::Damage()
{
	return false;
}
bool TNpcObject::Dead()
{
	return true;
}
bool TNpcObject::Release()
{
	TObject::Release();
	for (int iAction = 0; iAction < m_pActionList.size(); iAction++)
	{
		delete m_pActionList[iAction];
	}
	m_pActionList.clear();
	return true;
}
