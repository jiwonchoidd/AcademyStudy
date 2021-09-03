#include "TUser.h"
void TUser::AddPacket(T_PACKET& addPacket )
{
}
bool TUser::Create()
{
	TThread::CreateThread();
	return true;
}
bool TUser::Run()
{	
	char szMsgBuffer[2048]= {0,};
	int strlen = 0;
	while(1)
	{
		strlen = recv( m_Socket,
			szMsgBuffer,sizeof(szMsgBuffer), 0 );
		if( strlen <= 0) break;
		szMsgBuffer[strlen] = 0;	
		m_StreamPacket.Put(szMsgBuffer, strlen, this);
		Sleep(10);
	}
	DeleteUser(m_Socket);	
	return true;
}
void TUser::DeleteUser( SOCKET socket )
{
	closesocket(m_Socket);
}
TUser::TUser(void)
{
	m_Socket = 0;
}


TUser::~TUser(void)
{
}
