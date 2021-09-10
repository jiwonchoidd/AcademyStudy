
/*---------------------------------------------------------------

��Ŷ����� ���� �Լ�.

---------------------------------------------------------------*/

#ifndef __PACKET_MAKE__
#define __PACKET_MAKE__



////////////////////////////////////////////////////////////////////
// dfPACKET_SC_CREATE_MY_CHARACTER
//
// Parameters: (CAyaPacket *)PacketClass. (DWORD)SessionID. (BYTE)Direction. (short)X. (short)Y. (char)HP.
// Return: ����.
////////////////////////////////////////////////////////////////////
void	mpCreateMyCharacter(CAyaPacket *clpPacket, DWORD dwSessionID, BYTE byDirection, short shX, short shY, char chHP);



////////////////////////////////////////////////////////////////////
// dfPACKET_SC_CREATE_OTHER_CHARACTER
//
// Parameters: (CAyaPacket *)PacketClass. (DWORD)SessionID. (BYTE)Direction. (short)X. (short)Y. (char)HP.
// Return: ����.
////////////////////////////////////////////////////////////////////
void	mpCreateOtherCharacter(CAyaPacket *clpPacket, DWORD dwSessionID, BYTE byDirection, short shX, short shY, char chHP);


////////////////////////////////////////////////////////////////////
// dfPACKET_SC_DELETE_CHARACTER
//
// Parameters: (CAyaPacket *)PacketClass. (DWORD)SessionID.
// Return: ����.
////////////////////////////////////////////////////////////////////
void	mpDeleteCharacter(CAyaPacket *clpPacket, DWORD dwSessionID);

////////////////////////////////////////////////////////////////////
// dfPACKET_SC_MOVE_START
//
// Parameters: (CAyaPacket *)PacketClass. (DWORD)SessionID. (BYTE)Direction. (short)X. (short)Y.
// Return: ����.
////////////////////////////////////////////////////////////////////
void	mpMoveStart(CAyaPacket *clpPacket, DWORD dwSessionID, BYTE byDir, short shX, short shY);

////////////////////////////////////////////////////////////////////
// dfPACKET_SC_MOVE_STOP
//
// Parameters: (CAyaPacket *)PacketClass. (DWORD)SessionID. (BYTE)Direction. (short)X. (short)Y.
// Return: ����.
////////////////////////////////////////////////////////////////////
void	mpMoveStop(CAyaPacket *clpPacket, DWORD dwSessionID, BYTE byDir, short shX, short shY);

////////////////////////////////////////////////////////////////////
// dfPACKET_SC_ATTACK1
//
// Parameters: (CAyaPacket *)PacketClass. (DWORD)SessionID. (BYTE)Direction. (short)X. (short)Y.
// Return: ����.
////////////////////////////////////////////////////////////////////
void	mpAttack1(CAyaPacket *clpPacket, DWORD dwSessionID, BYTE byDir, short shX, short shY);

////////////////////////////////////////////////////////////////////
// dfPACKET_SC_ATTACK2
//
// Parameters: (CAyaPacket *)PacketClass. (DWORD)SessionID. (BYTE)Direction. (short)X. (short)Y.
// Return: ����.
////////////////////////////////////////////////////////////////////
void	mpAttack2(CAyaPacket *clpPacket, DWORD dwSessionID, BYTE byDir, short shX, short shY);

////////////////////////////////////////////////////////////////////
// dfPACKET_SC_ATTACK3
//
// Parameters: (CAyaPacket *)PacketClass. (DWORD)SessionID. (BYTE)Direction. (short)X. (short)Y.
// Return: ����.
////////////////////////////////////////////////////////////////////
void	mpAttack3(CAyaPacket *clpPacket, DWORD dwSessionID, BYTE byDir, short shX, short shY);



////////////////////////////////////////////////////////////////////
// dfPACKET_SC_DAMAGE
//
// Parameters: (CAyaPacket *)PacketClass. (DWORD)AttackID. (DWORD)DamageID. (short)DamageHP.
// Return: ����.
////////////////////////////////////////////////////////////////////
void	mpDamage(CAyaPacket *clpPacket, DWORD dwAttackID, DWORD dwDamageID, short shHP);


#endif