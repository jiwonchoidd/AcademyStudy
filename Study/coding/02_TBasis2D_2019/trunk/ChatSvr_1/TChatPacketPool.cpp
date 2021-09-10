#include "TChatPacketPool.h"
#include "TChatUser.h"
#include "TDebugString.h"
#include "TChatSvr.h"
#include "TChatUser.h"

void TChatPacketPool::ProcessWork(T_PACKET* pUserData)
{
	UPACKET* pPacket = &(pUserData->packet);
	TChatUser*   pUser = (TChatUser*)pUserData->pUser;	
	switch(pPacket->ph.type)
	{
	case PACKET_USER_POSITION:
		{
			pUser->Movement(pPacket);
			I_Server.Broadcastting(pPacket, pUser->m_Socket );	
		}break;
	case PACKET_CHAR_NAME_ACK:
		{
			pUser->m_szName=pPacket->msg;
			char buffer[256] = {0,};
			sprintf_s( buffer, "[%s]%s", pUser->m_szName.c_str(),"님이 입장하였습니다.");
			ZeroMemory(pPacket->msg, 2048);
			sprintf_s( pPacket->msg,"%s", buffer ); 
			pPacket->ph.len = sizeof( PACKET_HEADER) + strlen(buffer);
			pPacket->ph.type = PACKET_CHAR_MSG;

			I_Server.Broadcastting(pPacket, pUser->m_Socket );
			
		}break;
	case PACKET_CHAR_MSG:
		{
			I_Server.Broadcastting(pPacket,pUser->m_Socket);
		}break;
	}
}
TChatPacketPool::TChatPacketPool(void)
{
}


TChatPacketPool::~TChatPacketPool(void)
{
}
