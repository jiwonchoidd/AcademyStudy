#include "TChatAccept.h"
#include "TChatSvr.h"

void TChatAccept::AddUser(SOCKET socket, SOCKADDR_IN address )
{
	I_Server.AddUser( socket, address );
}
TChatAccept::TChatAccept(void)
{
}


TChatAccept::~TChatAccept(void)
{
}
