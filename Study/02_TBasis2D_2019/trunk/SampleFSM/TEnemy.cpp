#include "TEnemy.h"
#include "TStandAction.h"
#include "TMoveAction.h"
#include "TAttackAction.h"
#include "tRunawayAction.h"
void TEnemy::FSM(int iIndex)
{
	m_pFsm = I_fsm.GetPtr(iIndex);
	m_pActionList[STATE_STAND] = (TEnemyState*)new TStandAction(this);
	m_pActionList[STATE_MOVE] = (TEnemyState*)new TMoveAction(this);
	m_pActionList[STATE_ATTACK] = (TEnemyState*)new TAttackAction(this);
	m_pActionList[STATE_RUNAWAY] = (TEnemyState*)new TRunawayAction(this);
	m_pAction = m_pActionList[STATE_MOVE];
	m_dwCurrentState = STATE_MOVE;
}
bool TEnemy::Frame()
{
	//if (m_ObjPos.x < g_rtClient.left + m_rtObj.right/2)
	//{
	//	m_iDirection[0] *= -1;
	//	m_ObjPos.x = g_rtClient.left + m_rtObj.right / 2;
	//}
	//if (m_ObjPos.x > g_rtClient.right - m_rtObj.right / 2)
	//{
	//	m_iDirection[0] *= -1;
	//	m_ObjPos.x = g_rtClient.right - m_rtObj.right / 2;
	//}
	//if (m_ObjPos.y < g_rtClient.top + m_rtObj.bottom / 2)
	//{
	//	m_iDirection[1] *= -1;
	//	m_ObjPos.y = g_rtClient.top + m_rtObj.bottom / 2;
	//}
	//if (m_ObjPos.y > g_rtClient.bottom - m_rtObj.bottom / 2)
	//{
	//	m_iDirection[1] *= -1;
	//	m_ObjPos.y = g_rtClient.bottom - m_rtObj.bottom / 2;
	//}
	//float fTime = g_fSecPerFrame;
	//m_ObjPos.x += m_iDirection[0] * m_fSpeed * fTime;
	//m_ObjPos.y += m_iDirection[1] * m_fSpeed * fTime;
	//SetPos(m_ObjPos);
	return true;
}
void TEnemy::Process(xObject* target)
{
	m_pAction->Process(target);
}

DWORD TEnemy::GetState()
{
	if (m_pFsm == 0)
		return 0;
	return m_dwCurrentState;
}

void TEnemy::SetTransition(DWORD event)
{
	m_dwCurrentState = m_pFsm->StateTransition(m_dwCurrentState, event);
	m_pAction = m_pActionList[m_dwCurrentState];
}

void TEnemy::Move(int x, int y)
{
	if (m_ObjPos.x < x) m_ObjPos.x += m_fSpeed * g_fSecPerFrame;
	if (m_ObjPos.x > x) m_ObjPos.x -= m_fSpeed * g_fSecPerFrame;
	if (m_ObjPos.y < y) m_ObjPos.y += m_fSpeed * g_fSecPerFrame;
	if (m_ObjPos.y > y) m_ObjPos.y -= m_fSpeed * g_fSecPerFrame;
	SetPos(m_ObjPos);
}
TEnemy::TEnemy()
{
}


TEnemy::~TEnemy()
{
	for (int i = 0; i < STATE_COUNT; i++)
	{
		delete m_pActionList[i];
	}
}
