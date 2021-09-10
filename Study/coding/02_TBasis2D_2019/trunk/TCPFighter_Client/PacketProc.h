/////////////////////////////////////////////////////////////////////
//
/*---------------------------------------------------------------

패킷 처리 전용 함수.

---------------------------------------------------------------*/

#ifndef __PACKET_PROC__
#define __PACKET_PROC__


//////////////////////////////////////////////////////////////////////////
// 각 패킷 타입에 따른 패킷 처리 함수.
//
// Parameters: (CAyaPacket *)PacketClass.
// Return: (BOOL)TRUE, FALSE.
//////////////////////////////////////////////////////////////////////////
BOOL	netPacketProc_CreateMyCharacter(CAyaPacket *clpPacket);

BOOL	netPacketProc_CreateOtherCharacter(CAyaPacket *clpPacket);

BOOL	netPacketProc_DeleteCharacter(CAyaPacket *clpPacket);

BOOL	netPacketProc_MoveStart(CAyaPacket *clpPacket);

BOOL	netPacketProc_MoveStop(CAyaPacket *clpPacket);

BOOL	netPacketProc_Attack1(CAyaPacket *clpPacket);

BOOL	netPacketProc_Attack2(CAyaPacket *clpPacket);

BOOL	netPacketProc_Attack3(CAyaPacket *clpPacket);

BOOL	netPacketProc_Damage(CAyaPacket *clpPacket);

BOOL	netPacketProc_Sync(CAyaPacket *clpPacket);


#endif
