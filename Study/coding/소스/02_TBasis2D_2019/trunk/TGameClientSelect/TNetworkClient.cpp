#include "TNetworkClient.h"
#include "TStd.h"

#define NETWORK_MSG WM_USER+80
static void DisplayText(const char* fmt, ...)
{
	va_list arg;
	va_start(arg, fmt);
	char buf[1024 + 256] = { 0, };
	vsprintf_s(buf, fmt, arg);
	OutputDebugStringA((char*)buf);
	va_end(arg);
}
static int	g_iChatCount = 0;
LRESULT	 TNetworkClient::MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if(msg == NETWORK_MSG)
	{
		if (WSAGETSELECTERROR(lParam) != 0)
		{
			TNetwork::g_bConnect = false;
			return -1;
		}
		WORD dwSelect = WSAGETSELECTEVENT(lParam);
		switch (dwSelect)
		{
			case FD_CONNECT:
			{
				TNetwork::g_bConnect = true;
			}break;
			case FD_CLOSE:
			{
				TNetwork::g_bConnect = false;
			}break;
			case FD_READ:
			{
				RecvData(m_User);
				//PostMessage(g_hWnd, NETWORK_MSG, m_Sock, FD_READ);				
			}break;
			case FD_WRITE:
			{
				DisplayText("%s\n", "FD_WRITE");
			}break;
		}		
	}
	return -1;
}
void TNetworkClient::AddSendPacket( UPACKET& packet)
{
	m_sendPacket.push_back(packet);
}
bool TNetworkClient::SendData(TNetUser& user, UPACKET& msg)
{
	while (user.iSendSize < msg.ph.len)
	{
		user.iSendSize += send(m_Sock,
			(char*)&msg,
			msg.ph.len - user.iSendSize, 0);
		if (user.iSendSize == 0)
		{
			return false;
		}
		if (user.iSendSize == SOCKET_ERROR)
		{
			if (WSAGetLastError() != WSAEWOULDBLOCK)
			{
				return false;
			}
			user.iSendSize = 0;
			break;
		}
	}
	if (user.iSendSize == msg.ph.len)
	{
		user.iSendSize = 0;
	}
	return true;
}
bool TNetworkClient::SendPackets(TNetUser& user)
{
	std::vector<UPACKET>::iterator senditer;
	for (senditer = m_sendPacket.begin();
		senditer != m_sendPacket.end();
		senditer++)
	{
		if (SendData(user, *senditer) == false)
		{
			TNetwork::g_bConnect = false;
			return false;
		}
	}
	m_sendPacket.clear();
	return true;
}

bool  TNetworkClient::InitNetwork(std::string ip, int iPort)
{
	if (TNetwork::InitNetwork(ip.c_str(), iPort) == false)
	{
		return false;
	}
	return true;
}
bool TNetworkClient::InitSocket(std::string ip, int port)
{
	// 넌블로킹소켓 : 네트워크 이벤트
	int iRet = WSAAsyncSelect(m_Sock, 
		g_hWnd,	NETWORK_MSG,
		 FD_CONNECT | FD_READ | FD_WRITE | FD_CLOSE	);
	if (iRet == SOCKET_ERROR) return false;

	SOCKADDR_IN sa;
	// 바이트 정렬 구조 	
	USHORT jValue = 10000;
	sa.sin_family = AF_INET;
	sa.sin_addr.s_addr = inet_addr(ip.c_str());
	//error C4996 : 'inet_addr' : Use inet_pton() or InetPton() instead or define _WINSOCK_DEPRECATED_NO_WARNINGS
	sa.sin_port = htons(port);
	iRet = connect(m_Sock, (SOCKADDR*)&sa, sizeof(sa));
	//iRet = WSAConnect(m_Sock, (SOCKADDR*)&sa, sizeof(sa),
	//					NULL,NULL,NULL,NULL);
	if (iRet == SOCKET_ERROR)
	{
		if (WSAGetLastError() != WSAEWOULDBLOCK)
		{
			return false;
		}
	}
	return true;
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
		return true;
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
		return true;
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
			TNetResult* ret = (TNetResult*)packet->msg;
			if (ret->iRet == 1)
			{
				m_bLogin = true;
				//ResumeThread(g_hSendThread);
			}
			//SuspendThread(g_hSendThread);				
		}
	}
	m_recvPacket.clear();
}
TNetworkClient::TNetworkClient()
{
	m_bLogin = false;
	m_bLobbyEntry = false;
	m_bZoneEntry = false;
}
TNetworkClient::~TNetworkClient()
{
	m_sendPacket.clear();
}


