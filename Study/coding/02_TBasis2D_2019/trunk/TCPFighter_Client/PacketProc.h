/////////////////////////////////////////////////////////////////////
//
/*---------------------------------------------------------------

��Ŷ ó�� ���� �Լ�.

---------------------------------------------------------------*/

#ifndef __PACKET_PROC__
#define __PACKET_PROC__


//////////////////////////////////////////////////////////////////////////
// �� ��Ŷ Ÿ�Կ� ���� ��Ŷ ó�� �Լ�.
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
