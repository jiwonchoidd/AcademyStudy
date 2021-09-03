#pragma once
#include "TFiniteState.h"

class InvalidCurrentStateException
{
public:
	InvalidCurrentStateException(int curStateID);
	virtual	~InvalidCurrentStateException();

	DWORD	currentStateID;
};

class InvalidInputEventException
{
public:
	InvalidInputEventException(int event);
	virtual	~InvalidInputEventException();

	DWORD	inputEvent;
};

class TFiniteStateMachine 
{
private:
	std::map	< DWORD, TFiniteState* >		mapState;
public:
	T_STR			m_szName;
	T_STR			m_szPath;
	int				m_iIndex;
public:
	void		AddStateTransition(DWORD m_dwState, DWORD inputEvent, DWORD outputStateID);
	void		DeleteTransition(DWORD m_dwState, DWORD inputEvent);
	DWORD		GetOutputState(DWORD inputEvent);
	DWORD		GetCurrentStateID(DWORD Output);
	DWORD		StateTransition(DWORD dwCur, int event);
	bool		Load(const TCHAR* szFileName);
	bool		Release();
public:
	TFiniteStateMachine();
	virtual ~TFiniteStateMachine();
};


class TFSMMgr : public TSingleton < TFSMMgr >
{
private:
	friend class TSingleton<TFSMMgr>;
public:
	typedef map <INT, TFiniteStateMachine*>		TemplateMap;
	typedef TemplateMap::iterator				TemplateMapItor;
	TemplateMapItor								TItor;
	TemplateMap									TMap;
	INT											m_iCurIndex;
public:	
	bool			Init();
	INT				Add( const TCHAR* szFileName );
	TFiniteStateMachine*		GetPtr(INT iIndex);
	bool			Release();
	bool			Delete(INT iDelete);
public:
	TFSMMgr();
	~TFSMMgr();
};
#define I_fsm TFSMMgr::GetInstance()

