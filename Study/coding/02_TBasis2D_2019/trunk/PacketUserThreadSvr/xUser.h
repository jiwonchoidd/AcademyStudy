#pragma once
#include "TServerStd.h"
#include "Protocol.h"
#include <iostream>
#include <list>
#include <vector>
#include <string>
#include <algorithm>
using namespace std;

#define PACKET_MAX_SIZE 2048

struct TOV : OVERLAPPED
{
	int iType;
};

struct xUser 
{
	TOV		m_ovRecv;

	SOCKET sock;
	SOCKADDR_IN address;
	int			m_iStartPos;
	int			m_iWritePos;
	int			m_iReadPos;
	char		m_RecvBuffer[PACKET_MAX_SIZE];
	char*       m_pWritePos;
	UPACKET*    m_pRecvPacket;
	WSABUF      m_wsaWritePos;
	
	OVERLAPPED  m_ovSend;
	DWORD       Recv(DWORD dwSize);
	xUser(const xUser& user);
	xUser();
	int  AddBuffer(int iRecvData);
};


