#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include "TUdpSocket.h"
#include "TDebugString.h"

// 멀티캐스트(udp)는 라우터로 멀티캐스트 정보 전달하면 
//                   받은 정보 복사하여 멀티 캐스트로 전송
// 받는 컴퓨터는 자신의 iP를 멀티캐스트로 등록하는 과정이 필요
bool TUdpSocket::Init()
{
	int retval;
	m_Socket = socket( AF_INET, SOCK_DGRAM, 0);
	
	// 멀티캐스트 TTL 설정
	int ttl = 64;// Time To Live
	retval = setsockopt(m_Socket, IPPROTO_IP, IP_MULTICAST_TTL,(char *)&ttl, sizeof(ttl));
	if(retval == SOCKET_ERROR){
		I_DebugStr.T_ERROR(false);
	}
	BOOL optval = TRUE;
	retval = setsockopt( m_Socket, SOL_SOCKET, SO_REUSEADDR,(char*)&optval, sizeof(optval));
	if( retval == SOCKET_ERROR )
	{
		I_DebugStr.T_ERROR(false);
	}
	// Recv	: Port번호를 달리 그룹화하면 Channel이 만들어 진다.
	ZeroMemory(&m_RecvAddr, sizeof(m_RecvAddr));
	m_RecvAddr.sin_family = AF_INET;
	m_RecvAddr.sin_port = htons(10000);
	m_RecvAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	retval = ::bind(m_Socket, (SOCKADDR *)&m_RecvAddr, sizeof(m_RecvAddr));	
	if( retval == SOCKET_ERROR )
	{
		I_DebugStr.T_ERROR(false);
	}
	// Send
	ZeroMemory( &m_SenderAddr, sizeof(m_SenderAddr));
	m_SenderAddr.sin_family = AF_INET;
	m_SenderAddr.sin_port = htons(9001);
	m_SenderAddr.sin_addr.s_addr = inet_addr("235.7.8.9");//inet_addr("127.0.0.1");//htonl(INADDR_BROADCAST);
	
	// 가입
	// 그룹에 가입할 호스트 주소.
	m_MemberReq.imr_interface.s_addr = htonl(INADDR_ANY);
	// 멀티캐스트 그룹의 주소
	m_MemberReq.imr_multiaddr.s_addr = inet_addr("235.7.8.9");
	retval= setsockopt( m_Socket, IPPROTO_IP, IP_ADD_MEMBERSHIP,(char*)&m_MemberReq, sizeof( m_MemberReq ));
	if( retval == SOCKET_ERROR )
	{
		I_DebugStr.T_ERROR();
	}

	CreateThread();
	return true;
}
bool TUdpSocket::Run()
{
	int retval;
	int addrlen;
	SOCKADDR_IN peeraddr;
	static int iCnt = 0;
	char buf[2048]={0,};
	while(1)
	{
		ZeroMemory(buf, sizeof(buf) );
		sprintf( buf, "[UDP][%d]ChattingMsg[1Sec Per Msg]----------------", iCnt++);
		retval = sendto( m_Socket, buf, strlen(buf), 0,(SOCKADDR*)&m_SenderAddr, sizeof(m_SenderAddr));				
		if( retval == SOCKET_ERROR )
		{
			I_DebugStr.T_ERROR(false);
		}
		addrlen = sizeof( peeraddr );
		retval = recvfrom(	m_Socket, buf, 2048, 0,(SOCKADDR*)&peeraddr, &addrlen);
		if( retval == SOCKET_ERROR )
		{
			I_DebugStr.T_ERROR(false);
		}
		else
		{
			buf[retval] = 0;
			I_DebugStr.DisplayText("[MSG]:%s", buf);
		}
		Sleep( 1000 );
	}
	return true;
}
bool TUdpSocket::Release()
{
	// 멀티캐스트 그룹 탈퇴
	int retval = setsockopt(m_Socket, IPPROTO_IP, IP_DROP_MEMBERSHIP,(char *)&m_MemberReq, sizeof(m_MemberReq));
	if(retval == SOCKET_ERROR) {
		I_DebugStr.T_ERROR(false);
	}
	closesocket( m_Socket);
	return true;
}
TUdpSocket::TUdpSocket(void)
{
}

TUdpSocket::~TUdpSocket(void)
{
}
