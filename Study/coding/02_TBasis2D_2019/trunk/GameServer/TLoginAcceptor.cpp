#include "TLoginAcceptor.h"
#include "TIocp.h"
#include "TSessionMgr.h"
#include "TServer.h"
bool TLoginAcceptor::AddSession(SOCKET sock, SOCKADDR_IN addr)
{
	TChatUser* user = new TChatUser(m_pServer);
	user->m_Sock = sock;
	user->m_SockAddr = addr;
	m_pServer->m_pSessionMgr->AddUser(user);
	user->WaitReceive();
	printf("\nÁ¢¼Ó->%s:%d",	inet_ntoa(addr.sin_addr),	ntohs(addr.sin_port));	
	return true;
}
TLoginAcceptor::TLoginAcceptor(TServer* pServer) : TAcceptor(pServer)
{

}
TLoginAcceptor::~TLoginAcceptor()
{
}