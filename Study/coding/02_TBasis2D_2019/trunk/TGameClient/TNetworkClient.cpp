#include "TNetworkClient.h"
static int	g_iChatCount = 0;
bool		g_bConnect = false;

DWORD WINAPI SendThread(LPVOID arg)
{
	TNetwork* net = (TNetwork*)arg;
	TChatMsg chatmsg;
	//printf("\nmsg=");
	while (g_bConnect)
	{	
		memset(&chatmsg, 0, sizeof(chatmsg));
		//fgets(chatmsg.buffer, 128, stdin);
		strcpy_s(chatmsg.szName, "홍길동");
		strcpy_s(chatmsg.buffer, "kgca");
		chatmsg.buffer[strlen(chatmsg.buffer) - 1] = 0;
		chatmsg.iCnt = g_iChatCount++;
		EnterCriticalSection(&net->m_cs);
			if (net->SendMsg(net->m_Sock, (char*)&chatmsg,
				sizeof(TChatMsg),
				PACKET_CHAT_MSG) < 0)
			{
				LeaveCriticalSection(&net->m_cs);
				break;
			}		
		LeaveCriticalSection(&net->m_cs);	
		Sleep(1000);
	}	
	return 0;
}
bool  TNetworkClient::InitNetwork(std::string ip, int iPort)
{
	if (TNetwork::InitNetwork(ip.c_str(), iPort) == false)
	{
		return false;
	}
	g_bConnect = true;
	// id:ps
	SendLoginData(m_Sock, "kgca", "game");

	// Create Thread
	DWORD dwID;
	g_hSendThread = CreateThread(0, 0, SendThread,
		(LPVOID)this, 0,//CREATE_SUSPENDED,
		&dwID);
	return true;
}
bool TNetworkClient::InitSocket(std::string ip, int port)
{
	SOCKADDR_IN sa;
	// 바이트 정렬 구조 	
	USHORT jValue = 10000;
	sa.sin_family = AF_INET;
	sa.sin_addr.s_addr = inet_addr(ip.c_str());
	//error C4996 : 'inet_addr' : Use inet_pton() or InetPton() instead or define _WINSOCK_DEPRECATED_NO_WARNINGS
	sa.sin_port = htons(port);
	int iRet = connect(m_Sock, (SOCKADDR*)&sa,
		sizeof(sa));
	if (iRet == SOCKET_ERROR)
	{
		return false;
	}
	return true;
}

void TNetworkClient::SendLoginData(SOCKET sock, std::string id, std::string ps)
{
	TLogin login;
	strcpy_s(login.szID, id.c_str());
	strcpy_s(login.szPS, ps.c_str());
	SendMsg(sock, (char*)&login, TLoginSize, PACKET_LOGIN_REQ);
}

bool TNetworkClient::RecvData(TNetUser& user)
{	
	// header
	if (user.iRecvSize < PACKET_HEADER_SIZE)
	{
		user.iRecvSize += recv(m_Sock, &user.recvBuf[user.iRecvSize],
			PACKET_HEADER_SIZE - user.iRecvSize, 0);
		if (user.iRecvSize == 0 || user.iRecvSize == SOCKET_ERROR)
		{
			user.iRecvSize = 0;
			if (WSAGetLastError() != WSAEWOULDBLOCK)
			{				
				return false;
			}			
			return true;
		}
	}
	else
	{
		UPACKET* packet = (UPACKET*)&user.recvBuf;
		user.iRecvSize += recv(m_Sock, &user.recvBuf[user.iRecvSize],
			packet->ph.len - user.iRecvSize, 0);
		if (user.iRecvSize == 0 || user.iRecvSize == SOCKET_ERROR)
		{
			user.iRecvSize = 0;
			if (WSAGetLastError() != WSAEWOULDBLOCK)
			{				
				return false;
			}			
			return true;
		}
		if (user.iRecvSize == packet->ph.len)
		{
			AddPacket(user, packet);
			memset(user.recvBuf, 0, sizeof(char) * 10000);
			user.iRecvSize = 0;
		}
	}
	return true;
}
void TNetworkClient::PacketProcess()
{
	std::vector<TPacket>::iterator senditer;
	for (senditer = m_recvPacket.begin();
		senditer != m_recvPacket.end();
		senditer++)
	{
		UPACKET* packet = (UPACKET*)&senditer->packet;
		if (packet->ph.type == PACKET_CHAT_MSG)
		{
			TChatMsg* pMsg = (TChatMsg*)&packet->msg;
			printf("\n[%s]%s:%d", pMsg->szName,
				pMsg->buffer, pMsg->iCnt);
		}	
		if (packet->ph.type == PACKET_LOGIN_ACK)
		{
			TLoginResult* ret = (TLoginResult*)packet->msg;
			if (ret->iRet == 1)
			{
				ResumeThread(g_hSendThread);
			}
			else
			{
				SendLoginData(m_Sock, "kgca", "game");
			}
			//SuspendThread(g_hSendThread);				
		}
	}
	m_recvPacket.clear();
}
TNetworkClient::TNetworkClient()
{

}
TNetworkClient::~TNetworkClient()
{

}