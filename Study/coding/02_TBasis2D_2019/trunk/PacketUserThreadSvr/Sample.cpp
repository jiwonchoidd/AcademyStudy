#include "xUser.h"

CRITICAL_SECTION cs;
CRITICAL_SECTION packetCS;

struct TUser
{
	static SOCKET	findSock;
	bool			isChatting;
	std::string		name;
	SOCKET			sock;
	SOCKADDR_IN		clientaddr;
	char			recvbuf[256];
	bool operator() (TUser& val) const 
	{
		return (findSock == val.sock);
	}
};
struct T_PACKET
{
	UPACKET packet;
	TUser*  pUser;
};
std::list<TUser*>   g_Userlist;
std::list<T_PACKET>  g_PacketPool;


template<typename T> class TCheck
{
public:
	bool operator() (T val) const
	{
		return (val->sock == m_iSock);
	}
	SOCKET m_iSock;
};
bool   SendMsg(TUser* pUser, int type, char* data, int iSize)
{ 
	UPACKET packet;
	ZeroMemory(&packet, sizeof(UPACKET));
	packet.ph.type = type;
	packet.ph.len = PACKET_HEADER_SIZE + iSize;
	if (data != nullptr)
	{
		memcpy(packet.msg, data, iSize);
	}

	int iSendByte = 0;
	do {
		int iPacketByte = send(pUser->sock,
			(char*)&packet, packet.ph.len - iSendByte, 0);
		if (iSendByte == SOCKET_ERROR)
		{
			return false;
		}
		iSendByte += iPacketByte;
	} while (packet.ph.len > iSendByte);
	return true;
}
bool   SendMsg(TUser* pUser, UPACKET& packet)
{
	int iSendByte = 0;
	do {
		int iPacketByte = send(pUser->sock,
			(char*)&packet, packet.ph.len - iSendByte, 0);
		if (iSendByte == SOCKET_ERROR)
		{
			return false;
		}
		iSendByte += iPacketByte;
	} while (packet.ph.len > iSendByte);
	return true;
}
void AddUser(	SOCKET		sock,
				SOCKADDR_IN clientaddr)
{
	TUser* client = new TUser;
	client->sock = sock;
	client->clientaddr = clientaddr;

	EnterCriticalSection(&cs);
		g_Userlist.push_back(client);
	LeaveCriticalSection(&cs);

	printf("\n접속[%s][%d]=%d",
		inet_ntoa(clientaddr.sin_addr),
		ntohs(clientaddr.sin_port),
		g_Userlist.size());
}
void AddUser(TUser* user)
{
	EnterCriticalSection(&cs);
		g_Userlist.push_back(user);
	LeaveCriticalSection(&cs);

	USER_NAME_REQ nameReq;
	ZeroMemory(&nameReq, sizeof(USER_NAME_REQ));
	USER_BASE username;
	username.iIndex = 9;
	memcpy(username.szName,"SERVER", sizeof(char)*7);
	memcpy(nameReq.msg, "환영합니다.", sizeof(char) * 12);
	memcpy(&nameReq.base, &username, sizeof(USER_BASE));

	UPACKET packet;
	packet.ph.len = sizeof(USER_NAME_REQ)+ PACKET_HEADER_SIZE;
	packet.ph.type = PACKET_CHAR_NAME_SC_REQ;
	memcpy(packet.msg, &nameReq, sizeof(USER_NAME_REQ));
	SendMsg(user, packet);

	printf("\n접속[%s][%d]=%d",
		inet_ntoa(user->clientaddr.sin_addr),
		ntohs(user->clientaddr.sin_port),
		g_Userlist.size());
}
void DelUser(SOCKET		sock)
{
	EnterCriticalSection(&cs);
		TCheck<TUser*> check;
		check.m_iSock = sock;
		std::list<TUser*>::iterator iter;
		iter = find_if(	g_Userlist.begin(),
						g_Userlist.end(), 
						check);
		TUser* pUser = (TUser*)*iter;
		/*TUser::findSock = sock;
		iter = find_if( g_Userlist.begin(),
						g_Userlist.end(),
						TUser());*/
		closesocket(pUser->sock);
		printf("\n종료[%s][%d]=%d",
			inet_ntoa(pUser->clientaddr.sin_addr),
			ntohs(pUser->clientaddr.sin_port),
			g_Userlist.size());

		delete *iter;
		g_Userlist.erase(iter);	
	LeaveCriticalSection(&cs);
}
void DelUser(TUser* pUser)
{
	EnterCriticalSection(&cs);
		std::list<TUser*>::iterator iter;
		iter = find(g_Userlist.begin(),g_Userlist.end(),pUser);
		closesocket(pUser->sock);
		printf("\n종료[%s][%d]=%d",
			inet_ntoa(pUser->clientaddr.sin_addr),
			ntohs(pUser->clientaddr.sin_port),
			g_Userlist.size());
		delete *iter;
		g_Userlist.erase(iter);
	LeaveCriticalSection(&cs);
}
void   Broadcast(const char* msg, int ibyte)
{
	//printf("\n%s", msg);
	EnterCriticalSection(&cs);
		for (TUser* user : g_Userlist)
		{
			int iSendByte = send(user->sock, msg, ibyte, 0);
			if (iSendByte == SOCKET_ERROR)
			{
				closesocket(user->sock);
			}
		}
	LeaveCriticalSection(&cs);
}


void   Broadcast(T_PACKET& tPacket)
{
	USER_CHAT_MSG msg;
	ZeroMemory(&msg, sizeof(USER_CHAT_MSG));
	memcpy(&msg, tPacket.packet.msg, tPacket.packet.ph.len-PACKET_HEADER_SIZE);
	printf("%s:%s", msg.szName, msg.msg);

	EnterCriticalSection(&cs);
	for (TUser* user : g_Userlist)
	{
		if (tPacket.pUser==user || user->isChatting == false) continue;
		if (SendMsg(user, tPacket.packet)==false)
		{
			closesocket(user->sock);
		}
	}
	LeaveCriticalSection(&cs);
}
bool RecvMsg(TUser* pUser, char* pBuffer, int iNumDataByte)
{
	if (iNumDataByte == 0) return true;
	int iRecvByte = 0;
	do {
		int iByte = recv(pUser->sock, &pBuffer[iRecvByte], iNumDataByte - iRecvByte, 0);
		if (iByte == 0)
		{
			return false;
		}
		if (iByte == SOCKET_ERROR)
		{
			return false;
		}
		iRecvByte += iByte;
	} while (iNumDataByte > iRecvByte);
	return true;
}
DWORD WINAPI UserThread(LPVOID arg)
{
	TUser* user = (TUser*)arg;
	int iRecvByte = 0;
	while (1)
	{
		iRecvByte = recv(	user->sock,	&user->recvbuf[iRecvByte],PACKET_HEADER_SIZE-iRecvByte, 0);
		if (iRecvByte == 0)
		{
			break;
		}
		if (iRecvByte == SOCKET_ERROR)
		{
			break;
		}
		if (iRecvByte == PACKET_HEADER_SIZE)
		{
			T_PACKET tPacket;
			ZeroMemory(&tPacket, sizeof(tPacket));
			memcpy(&tPacket.packet.ph, user->recvbuf, PACKET_HEADER_SIZE);
			int iNumDataByte = tPacket.packet.ph.len - PACKET_HEADER_SIZE;
			
			if (false == RecvMsg(user, tPacket.packet.msg, iNumDataByte))
			{
				break;
			}

			EnterCriticalSection(&packetCS);				
				tPacket.pUser = user;
				g_PacketPool.push_back(tPacket);
			LeaveCriticalSection(&packetCS);
			iRecvByte = 0;
		}		
		Sleep(1);
	}
	//DelUser(user->sock);
	DelUser(user);
	return 1;
}
DWORD WINAPI PacketThread(LPVOID arg)
{
	while (1)
	{
		EnterCriticalSection(&packetCS);
		for (T_PACKET& tPacket : g_PacketPool)
		{
			switch (tPacket.packet.ph.type)
			{
				case  PACKET_DRUP_CS_REQ: 
				{								
					tPacket.pUser->isChatting = false;
					EnterCriticalSection(&cs);
						USER_BASE info;
						info.iIndex = 9;
						memcpy(info.szName, tPacket.pUser->name.c_str(), tPacket.pUser->name.size());
						for (TUser* user : g_Userlist)
						{
							if (user->isChatting != true)continue;
							SendMsg(user, PACKET_DRUP_USERS_SC, (char*)&info, sizeof(USER_BASE));
						}				
						
						SendMsg(tPacket.pUser, PACKET_DRUP_SC_ACK, nullptr, 0);
					LeaveCriticalSection(&cs);
				}break;
				case  PACKET_CHAR_MSG:
				{
					Broadcast(tPacket);
				}break;
				case  PACKET_CHAR_NAME_CS_ACK:
				{
					EnterCriticalSection(&cs);
						std::list<TUser*>::iterator iter;
						iter = find(g_Userlist.begin(), g_Userlist.end(), tPacket.pUser);
						TUser* pUser = (*iter);

						USER_NAME_ACK userName;
						memcpy( &userName,tPacket.packet.msg, sizeof(USER_NAME_ACK));						
						pUser->name = userName.szName;
						printf("\n%s", userName.szName);
						// 클라이언트 등록 허가
						SendMsg(pUser, PACKET_JOIN_SIGNIN_SC,nullptr, 0);
						// 다른 클라이언트 유저 접속 통지
						//PACKET_JOIN_USER_SC+data
						USER_BASE info;
						ZeroMemory(&info, sizeof(USER_BASE));
						info.iIndex = 9;
						memcpy(info.szName, userName.szName, USER_NAME_SIZE);
						for (TUser* user : g_Userlist)
						{
							if (user->isChatting != true)continue;
							SendMsg(user, PACKET_JOIN_USER_SC, (char*)&info, sizeof(USER_BASE));
						}
						pUser->isChatting = true;
					LeaveCriticalSection(&cs);
				}break;
			}
		}
		g_PacketPool.clear();
		LeaveCriticalSection(&packetCS);
		Sleep(1);
	}
	return 1;
}
void main()
{
	InitializeCriticalSection(&cs);
	InitializeCriticalSection(&packetCS);

	WSADATA wsa;
	WSAStartup(MAKEWORD(2, 2), &wsa);
	SOCKET listensock = socket(AF_INET, SOCK_STREAM, 0);
	SOCKADDR_IN sa;
	ZeroMemory(&sa, sizeof(sa));
	sa.sin_family = AF_INET;
	sa.sin_port = htons(10000);
	sa.sin_addr.s_addr = htonl(INADDR_ANY);
	int ret = bind(listensock, (SOCKADDR*)&sa, sizeof(sa));
	if (ret == SOCKET_ERROR)
	{
		E_MSG("bind");
		return;
	}
	ret = listen(listensock, SOMAXCONN);
	if (ret == SOCKET_ERROR)
	{
		E_MSG("listen");
		return;
	}
	printf("\n서버 시작!!!\n");
	DWORD iPacketID;
	HANDLE hThreadPacket = CreateThread(0, 0,
		PacketThread,
		0,
		0,
		&iPacketID);
	CloseHandle(hThreadPacket);

	int addlen = sizeof(SOCKADDR_IN);
	while (1)
	{
		TUser* user = new TUser;
		//ZeroMemory(user, sizeof(TUser));
		user->sock = accept(	listensock,
							(SOCKADDR*)&user->clientaddr,
							&addlen);
		if (user->sock == INVALID_SOCKET)
		{
			E_MSG("accept");
		}		
		AddUser(user);	

		DWORD iThreadID;
		HANDLE hThreadSend = CreateThread(0, 0,
			UserThread,
			(void*)user,
			0,
			&iThreadID);
	}	
	closesocket(listensock);
	WSACleanup();

	DeleteCriticalSection(&cs);
	DeleteCriticalSection(&packetCS);
}