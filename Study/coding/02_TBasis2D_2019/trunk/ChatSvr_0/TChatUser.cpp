#include "TChatUser.h"
#include "TChatSvr.h"
void TChatUser::AddPacket(T_PACKET addPacket )
{
	I_Server.m_PacketPool.AddPacket(addPacket);
}
void TChatUser::DeleteUser( SOCKET socket )
{
	I_Server.DeleteUser( socket );
};
TChatUser::TChatUser(void)
{
}


TChatUser::~TChatUser(void)
{
}
