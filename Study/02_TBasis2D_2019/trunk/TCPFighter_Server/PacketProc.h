
/*---------------------------------------------------------------

패킷 처리 전용 함수.

---------------------------------------------------------------*/

#ifndef __PACKET_PROC__
#define __PACKET_PROC__


//////////////////////////////////////////////////////////////////////////
// 사용자 접속/해지 알림.
//
// Parameters: (DWORD)SessionID.
// Return: (BOOL)TRUE, FALSE.
//////////////////////////////////////////////////////////////////////////
BOOL	netPacketProc_Connect(DWORD dwSessionID);
BOOL	netPacketProc_Disconnect(DWORD dwSessionID);


//////////////////////////////////////////////////////////////////////////
// 각 패킷 타입에 따른 패킷 처리 함수.
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
