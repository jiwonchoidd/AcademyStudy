#pragma once
#include "xObject.h"
#include "TFiniteStateMachine.h"
class TEnemy;

class TEnemyState
{
public:
	TEnemyState(TEnemy* parent) : m_pOwner(parent)
	{	
	};
	virtual ~TEnemyState() {};
public:
	virtual	void		Process(xObject* ch) = 0;
protected:
	TEnemy*		m_pOwner;
};

class TEnemy : public xObject
{
private:
	TEnemyState*			m_pAction;
	TEnemyState*			m_pActionList[STATE_COUNT];
	TFiniteStateMachine*	m_pFsm;
	DWORD					m_dwCurrentState;
public:
	virtual	void		Process(xObject* target);
	DWORD	GetState();
	void	SetTransition(DWORD event);
	void	Move(int x, int y);
	void    FSM(int iIndex);
	bool	Frame();
public:
	TEnemy();
	virtual ~TEnemy();
};

