
/*---------------------------------------------------------------

패킷만들기 관련 함수.

---------------------------------------------------------------*/

#ifndef __PACKET_MAKE__
#define __PACKET_MAKE__



////////////////////////////////////////////////////////////////////
// dfPACKET_CS_MOVE_START
//
// Parameters: (CAyaPacket *)PacketClass. (BYTE)Direction. (short)X. (short)Y.
// Return: 없음.
////////////////////////////////////////////////////////////////////
void	mpMoveStart(CAyaPacket *clpPacket, BYTE byDirection, short shX, short shY);



////////////////////////////////////////////////////////////////////
// dfPACKET_CS_MOVE_STOP
//
// Parameters: (CAyaPacket *)PacketClass. (BYTE)Direction. (short)X. (short)Y.
// Return: 없음.
////////////////////////////////////////////////////////////////////
void	mpMoveStop(CAyaPacket *clpPacket, BYTE byDirection, short shX, short shY);


////////////////////////////////////////////////////////////////////
// dfPACKET_CS_ATTACK1
//
// Parameters: (CAyaPacket *)PacketClass. (BYTE)Direction. (short)X. (short)Y.
// Return: 없음.
////////////////////////////////////////////////////////////////////
void	mpAttack1(CAyaPacket *clpPacket, BYTE byDir, short shX, short shY);

////////////////////////////////////////////////////////////////////
// dfPACKET_CS_ATTACK2
//
// Parameters: (CAyaPacket *)PacketClass. (BYTE)Direction. (short)X. (short)Y.
// Return: 없음.
////////////////////////////////////////////////////////////////////
void	mpAttack2(CAyaPacket *clpPacket, BYTE byDir, short shX, short shY);

////////////////////////////////////////////////////////////////////
// dfPACKET_CS_ATTACK3
//
// Parameters: (CAyaPacket *)PacketClass. (BYTE)Direction. (short)X. (short)Y.
// Return: 없음.
////////////////////////////////////////////////////////////////////
void	mpAttack3(CAyaPacket *clpPacket, BYTE byDir, short shX, short shY);



#endif