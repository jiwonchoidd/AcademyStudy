#include "TServer.h"
bool TServer::SendMsg( SOCKET socket, 
					   char* msg,
					   WORD code )
{
	UPACKET sendmsg;
	ZeroMemory( &sendmsg, sizeof(sendmsg));
	//sprintf( sendmsg.msg, "%s", msg );
	sprintf_s( sendmsg.msg, "%s", msg );
	sendmsg.ph.type = code;
	sendmsg.ph.len = sizeof(PACKET_HEADER) +
		             strlen(sendmsg.msg );
	int iRet = send( socket, (char*)&sendmsg,sendmsg.ph.len, 0 );
	if( iRet <= 0 )
	{		
		return false;
	}	
	return true;
}
bool TServer::AddUser(SOCKET socket, SOCKADDR_IN address)
{
	return true;
}
bool TServer::Init()
{	
	// 쓰레드 생성 
	CreateThread();
	return true;
}

bool TServer::Run()
{
	return true;
}
bool TServer::Release()
{
	return true;
}
TServer::TServer(void)
{
}


TServer::~TServer(void)
{
}
