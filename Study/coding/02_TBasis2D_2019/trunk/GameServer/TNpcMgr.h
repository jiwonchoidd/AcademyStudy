#pragma once
#include "TThread.h"
#include "TNetObject.h"
#include "TObjectPool.h"
class TServer;
struct TNpc : TObjectPool<TNpc>
{
	int   m_iIndex;
	int   m_iNpcObjectID; // TNetObject
	TPoint  m_ptPos;
	TPoint  m_ptDir;
};
struct TBullet
{
	int		m_iIndex;
	TPoint  m_ptPos;
	TPoint  m_ptDir;
};
class TKeyGen
{
public:
	TKeyGen(int x) : m_iKey(x) {}
	int operator()(int x = 1) 
	{
		return (m_iKey += x);
	}
private:
	int m_iKey;
};
class TNpcMgr : public TSingleton<TNpcMgr>, public TThread
{	
private:
	friend class TSingleton<TNpcMgr>;
	TKeyGen  m_KeyGen;
public:
	std::map<int, TNpc*>		m_List;
	std::vector<int>			m_DelList;	
	std::vector<TBullet*>		m_BulletList;
	TServer*		m_pServer;
public:
	void	Init(TServer* pServer);
	void	AddNew(TNpc* pUser);
	void	AddDel(int id);
	bool	Run() override;
	bool	Del(TNpc* user);
	bool	Close(TNpc* user);
	std::vector<int> copy();
private:
	TNpcMgr();
public:
	virtual ~TNpcMgr();
};
#define I_NpcMgr TNpcMgr::GetInstance()
