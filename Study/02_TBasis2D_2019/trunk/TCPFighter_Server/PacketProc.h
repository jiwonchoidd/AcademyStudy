
/*---------------------------------------------------------------

��Ŷ ó�� ���� �Լ�.

---------------------------------------------------------------*/

#ifndef __PACKET_PROC__
#define __PACKET_PROC__


//////////////////////////////////////////////////////////////////////////
// ����� ����/���� �˸�.
//
// Parameters: (DWORD)SessionID.
// Return: (BOOL)TRUE, FALSE.
//////////////////////////////////////////////////////////////////////////
BOOL	netPacketProc_Connect(DWORD dwSessionID);
BOOL	netPacketProc_Disconnect(DWORD dwSessionID);


//////////////////////////////////////////////////////////////////////////
// �� ��Ŷ Ÿ�Կ� ���� ��Ŷ ó�� �Լ�.
//
// Parameters: (DWORD)SessionID. (CAyaPacket *)PacketClass.
// Return: (BOOL)TRUE, FALSE.
//////////////////////////////////////////////////////////////////////////
BOOL	netPacketProc_MoveStart(DWORD dwSessionID, CAyaPacket *clpPacket);

BOOL	netPacketProc_MoveStop(DWORD dwSessionID, CAyaPacket *clpPacket);

BOOL	netPacketProc_Attack1(DWORD dwSessionID, CAyaPacket *clpPacket);

BOOL	netPacketProc_Attack2(DWORD dwSessionID, CAyaPacket *clpPacket);

BOOL	netPacketProc_Attack3(DWORD dwSessionID, CAyaPacket *clpPacket);

BOOL	netPacketProc_Sync(DWORD dwSessionID, CAyaPacket *clpPacket);


#endif
