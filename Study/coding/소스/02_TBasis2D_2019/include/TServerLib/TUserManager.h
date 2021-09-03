#pragma once
#include "TUser.h"
class TUserManager: public TSingleton<TUserManager>
{
private:
	friend class TSingleton<TUserManager>;
public:
	std::vector<TUser*>   m_UserList;
	HANDLE          m_hMutex;	
public:
	void AddUser(TUser* pUser);
	void AddUser(SOCKET sock, SOCKADDR_IN clientaddr);
	void DelUser(TUser* pack);
	void DelUser(SOCKET sock);
	void Run();
public:
	TUserManager();
	virtual ~TUserManager();
};
#define I_UserMgr TUserManager::GetInstance()
