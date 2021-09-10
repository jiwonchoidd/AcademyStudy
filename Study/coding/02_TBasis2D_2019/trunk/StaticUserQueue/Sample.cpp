#include "TSessionMgr.h"

class TUser : public TBaseUser
{
public:
	RUNTIME_DECLARE(TUser)
	//DECLARE_DYNAMIC(TUser);
	//DECLARE_DYNCREATE(TUser);
	//DECLARE_DYNCLASS_CREATE;
	bool operator() (TUser* pData) const
	{
		return (m_Sock == pData->m_Sock);
	}
public:
	void  Create(){}
	void  Destory() {};
	void SetSocket(SOCKET sock, SOCKADDR_IN addr)
	{
		m_Sock = sock;
		m_addr = addr;
	};
protected:
	TUser() {}
};
RUNTIME_IMPLEMENT(TUser)
//IMPLEMENT_DYNCREATE(TUser);
//IMPLEMENT_DYNAMIC(TUser);
//IMPLEMENT_DYNCLASS_CREATE(TUser);

void main()
{
	ISessionMgr.Create(RUNTIME_CLASS(TUser));

	SOCKET sock=0;
	SOCKADDR_IN addr = { 0, };
	TBaseUser* pObj = ISessionMgr.CreateNewSession(sock, addr);
	ISessionMgr.RemoveSession(pObj);
}