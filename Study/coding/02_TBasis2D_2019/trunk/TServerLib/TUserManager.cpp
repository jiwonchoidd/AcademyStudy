#include "TUserManager.h"
#include "TServer.h"

template<typename T> class TCheck
{
public:
	bool operator() (T val) const
	{
		return (val->sock == m_iSock);
	}
	SOCKET m_iSock;
};

/////////////////////////////////

void TUserManager::AddUser(SOCKET		sock,SOCKADDR_IN clientaddr)
{
	WaitForSingleObject(m_hMutex, INFINITE);
	{
		TUser* client = new TUser;
		client->sock = sock;
		client->clientaddr = clientaddr;

		m_UserList.push_back(client);

		printf("\n접속[%s][%d]=%I64u",
			inet_ntoa(clientaddr.sin_addr),
			ntohs(clientaddr.sin_port),
			m_UserList.size());
	}
	ReleaseMutex(m_hMutex);
}
void TUserManager::AddUser(TUser* user)
{
	WaitForSingleObject(m_hMutex, INFINITE);
	{
		user->bConnect = true;
		user->userEvent = WSACreateEvent();
		m_UserList.push_back(user);

		USER_NAME_REQ nameReq;
		ZeroMemory(&nameReq, sizeof(USER_NAME_REQ));
		USER_BASE username;
		username.iIndex = 9;
		memcpy(username.szName, "SERVER", sizeof(char) * 7);
		memcpy(nameReq.msg, "환영합니다.", sizeof(char) * 12);
		memcpy(&nameReq.base, &username, sizeof(USER_BASE));

		UPACKET packet;
		packet.ph.len = sizeof(USER_NAME_REQ) + PACKET_HEADER_SIZE;
		packet.ph.type = PACKET_CHAR_NAME_SC_REQ;
		memcpy(packet.msg, &nameReq, sizeof(USER_NAME_REQ));
		I_Server.SendPool(user, packet);

		user->WaitForReceive();

		printf("\n접속[%s][%d]=%I64u",
			inet_ntoa(user->clientaddr.sin_addr),
			ntohs(user->clientaddr.sin_port),
			m_UserList.size());
	}
	ReleaseMutex(m_hMutex);
}
void TUserManager::DelUser(SOCKET sock)
{
	WaitForSingleObject(m_hMutex, INFINITE);
	{
		TCheck<TUser*> check;
		check.m_iSock = sock;
		std::vector<TUser*>::iterator iter;
		iter = find_if(m_UserList.begin(),
			m_UserList.end(),
			check);
		if (iter != m_UserList.end())
		{
			TUser* pUser = (TUser*)*iter;
			/*TUser::findSock = sock;
			iter = find_if( m_UserList.begin(),
							m_UserList.end(),
							TUser());*/
			closesocket(pUser->sock);
			printf("\n종료[%s][%d]=%I64u",
				inet_ntoa(pUser->clientaddr.sin_addr),
				ntohs(pUser->clientaddr.sin_port),
				m_UserList.size());

			delete *iter;
			m_UserList.erase(iter);
		}
	}
	ReleaseMutex(m_hMutex);	
}
void TUserManager::DelUser(TUser* pUser)
{
	WaitForSingleObject(m_hMutex, INFINITE);
	{
		std::vector<TUser*>::iterator iter;
		iter = find(m_UserList.begin(), m_UserList.end(), pUser);
		if (iter != m_UserList.end())
		{
			closesocket(pUser->sock);
			printf("\n종료[%s][%d]=%I64u",
				inet_ntoa(pUser->clientaddr.sin_addr),
				ntohs(pUser->clientaddr.sin_port),
				m_UserList.size());
			delete *iter;
			m_UserList.erase(iter);
		}
	}
	ReleaseMutex(m_hMutex);	
}
void TUserManager::Run()
{

}
TUserManager::TUserManager()
{
	m_hMutex = CreateMutex(NULL, FALSE, NULL);
}
TUserManager::~TUserManager()
{
	CloseHandle(m_hMutex);
}
