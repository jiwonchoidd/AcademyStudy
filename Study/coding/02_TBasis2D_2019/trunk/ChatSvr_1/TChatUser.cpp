#include "TChatUser.h"
#include "TChatSvr.h"

//typedef struct {
//	WORD  user_idx; // 케릭터의 고유한 인덱스
//	WORD  posX;     // 좌표
//	WORD  posY;
//	WORD  direction; // 방향 ( 0~7 ) 8방향
//}TPACKET_USER_POSITION;

void TChatUser::Movement(UPACKET* pPacket)
{
	TPACKET_USER_POSITION user;
	memcpy( &user, pPacket->msg, sizeof(char)*
		pPacket->ph.len-4 );
	m_iIndex = user.user_idx;
	m_PositionX = user.posX;
	m_PositionY = user.posY;
	m_iDirection = user.direction;
};
void TChatUser::AddPacket(T_PACKET& addPacket )
{
	I_Server.m_PacketPool.AddPacket(addPacket);
}
void TChatUser::DeleteUser( SOCKET socket )
{
	I_Server.DeleteUser( socket );
};
void TChatUser::DeleteUser()
{	
	I_Server.DeleteUser( this );
}
void TChatUser::Dispatch( DWORD dwByteSize, 
					 LPOVERLAPPED ov)
{
	OVERLAPPEDEX* pOV = (OVERLAPPEDEX*)ov;
	if( dwByteSize == 0 ) return;
	if( pOV->iFlags == OVERLAPPEDEX::MODE_RECV)
	{
		m_StreamPacket.Put(
			m_szRecvBuffer.buf,
			// this->m_strBuffer,
			dwByteSize,
			this);
	}
	if( pOV->iFlags == OVERLAPPEDEX::MODE_SEND)
	{
	}
	WaitForPacketReceive();
};
TChatUser::TChatUser(void)
{
}


TChatUser::~TChatUser(void)
{
}
