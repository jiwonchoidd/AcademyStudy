#include "TUdpSocket.h"
#include "TDebugString.h"
bool TUdpSocket::Init()
{
	m_SendSocket = socket( AF_INET, SOCK_DGRAM, 0 );
	m_Socket = socket( AF_INET, SOCK_DGRAM, 0 );

	int optval = 1;
	setsockopt( m_SendSocket,  SOL_SOCKET,
				SO_REUSEADDR, (char*)&optval, 
				sizeof(optval));
	setsockopt( m_Socket,  SOL_SOCKET,
				SO_REUSEADDR, (char*)&optval, 
				sizeof(optval));

	m_RecvAddr.sin_family = AF_INET;
	m_RecvAddr.sin_port = htons(10001);
	m_RecvAddr.sin_addr.s_addr= htonl(INADDR_ANY);
	int retval = ::bind( m_Socket, (SOCKADDR*)&m_RecvAddr, sizeof(m_RecvAddr));
	if( retval == SOCKET_ERROR ) return false;
	
	BOOL bEnable = TRUE;
	retval = setsockopt( m_SendSocket, SOL_SOCKET, 
		SO_BROADCAST, (char*)&bEnable, sizeof(bEnable));
	if( retval == SOCKET_ERROR ) return false;

	m_SendAddr.sin_family = AF_INET;
	m_SendAddr.sin_port = htons(9001);
	m_SendAddr.sin_addr.s_addr= htonl(INADDR_BROADCAST);

	CreateThread();
	return true;
}
bool TUdpSocket::Run()
{
	int retval, addrlen;
	SOCKADDR_IN PeerAddr;
	char buf[2048]= {0,};
	while(1)
	{
		addrlen = sizeof(PeerAddr);
		retval = recvfrom(m_Socket, buf, 2048, 0,
						(SOCKADDR*)&PeerAddr, &addrlen);
		if( retval == SOCKET_ERROR ) 
		{
			I_Debug.Print("[%s]", "Error!!!!!");
			break;
		}

		I_Debug.Print("[RECV]IP=%s, PORT=%d, %s",
			inet_ntoa(PeerAddr.sin_addr),
			ntohs(PeerAddr.sin_port), buf);

		//I_Debug.Print("%s ¹Þ°í", buf);
		buf[retval] = 0;
		retval = sendto(m_SendSocket, buf, strlen(buf), 0, 
			(SOCKADDR*)&m_SendAddr, sizeof(m_SendAddr));
		if( retval == SOCKET_ERROR ) 
		{
			I_Debug.T_ERROR(true);
			I_Debug.Print("[%s]", "Error!!!!!");
			break;
		}
		I_Debug.Print("[SEND]IP=%s, PORT=%d, %s",
			inet_ntoa(PeerAddr.sin_addr),
			ntohs(PeerAddr.sin_port), buf);			
	}
	closesocket(m_Socket);
	m_Socket = 0;
	return true;
}
bool TUdpSocket::Release()
{
	if(m_Socket != 0 ) 
	{
		closesocket(m_Socket);
		m_Socket = 0;
	}
	return true;
}
TUdpSocket::TUdpSocket(void)
{
	m_Socket = 0;
}


TUdpSocket::~TUdpSocket(void)
{
}
