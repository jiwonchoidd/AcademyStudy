
/*---------------------------------------------------------------

패킷만들기 관련 함수.

---------------------------------------------------------------*/

#include <windows.h>
#include "AyaPacket.h"
#include "AyaStreamSQ.h"

#include "Network.h"
#include "PacketDefine.h"
#include "MakePacket.h"


////////////////////////////////////////////////////////////////////
// dfPACKET_SC_CREATE_MY_CHARACTER
//
// Parameters: (CAyaPacket *)PacketClass. (DWORD)SessionID. (BYTE)Direction. (short)X. (short)Y. (char)HP.
// Return: 없음.
////////////////////////////////////////////////////////////////////
void	mpCreateMyCharacter(CAyaPacket *clpPacket, DWORD dwSessionID, BYTE byDirection, short shX, short shY, char chHP)
{
	st_NETWORK_PACKET_HEADER	stPacketHeader;

	stPacketHeader.byCode = dfNETWORK_PACKET_CODE;
	stPacketHeader.bySize = 10;
	stPacketHeader.byType = dfPACKET_SC_CREATE_MY_CHARACTER;

	clpPacket->PutData((char *)&stPacketHeader, dfNETWORK_PACKET_HEADER_SIZE);

	*clpPacket << dwSessionID;
	*clpPacket << byDirection;
	*clpPacket << shX;
	*clpPacket << shY;
	*clpPacket << chHP;

	*clpPacket << (BYTE)dfNETWORK_PACKET_END;
}



////////////////////////////////////////////////////////////////////
// dfPACKET_SC_CREATE_OTHER_CHARACTER
//
// Parameters: (CAyaPacket *)PacketClass. (DWORD)SessionID. (BYTE)Direction. (short)X. (short)Y. (char)HP.
// Return: 없음.
////////////////////////////////////////////////////////////////////
void	mpCreateOtherCharacter(CAyaPacket *clpPacket, DWORD dwSessionID, BYTE byDirection, short shX, short shY, char chHP)
{
	st_NETWORK_PACKET_HEADER	stPacketHeader;

	stPacketHeader.byCode = dfNETWORK_PACKET_CODE;
	stPacketHeader.bySize = 10;
	stPacketHeader.byType = dfPACKET_SC_CREATE_OTHER_CHARACTER;

	clpPacket->PutData((char *)&stPacketHeader, dfNETWORK_PACKET_HEADER_SIZE);

	*clpPacket << dwSessionID;
	*clpPacket << byDirection;
	*clpPacket << shX;
	*clpPacket << shY;
	*clpPacket << chHP;


	*clpPacket << (BYTE)dfNETWORK_PACKET_END;
}


////////////////////////////////////////////////////////////////////
// dfPACKET_SC_DELETE_CHARACTER
//
// Parameters: (CAyaPacket *)PacketClass. (DWORD)SessionID.
// Return: 없음.
////////////////////////////////////////////////////////////////////
void	mpDeleteCharacter(CAyaPacket *clpPacket, DWORD dwSessionID)
{
	st_NETWORK_PACKET_HEADER	stPacketHeader;

	stPacketHeader.byCode = dfNETWORK_PACKET_CODE;
	stPacketHeader.bySize = 4;
	stPacketHeader.byType = dfPACKET_SC_DELETE_CHARACTER;

	clpPacket->PutData((char *)&stPacketHeader, dfNETWORK_PACKET_HEADER_SIZE);

	*clpPacket << dwSessionID;

	*clpPacket << (BYTE)dfNETWORK_PACKET_END;
}


////////////////////////////////////////////////////////////////////
// dfPACKET_SC_MOVE_START
//
// Parameters: (CAyaPacket *)PacketClass. (DWORD)SessionID. (BYTE)Direction. (short)X. (short)Y.
// Return: 없음.
////////////////////////////////////////////////////////////////////
void	mpMoveStart(CAyaPacket *clpPacket, DWORD dwSessionID, BYTE byDir, short shX, short shY)
{
	st_NETWORK_PACKET_HEADER	stPacketHeader;

	stPacketHeader.byCode = dfNETWORK_PACKET_CODE;
	stPacketHeader.bySize = 9;
	stPacketHeader.byType = dfPACKET_SC_MOVE_START;

	clpPacket->PutData((char *)&stPacketHeader, dfNETWORK_PACKET_HEADER_SIZE);

	*clpPacket << dwSessionID;
	*clpPacket << byDir;
	*clpPacket << shX;
	*clpPacket << shY;

	*clpPacket << (BYTE)dfNETWORK_PACKET_END;
}


////////////////////////////////////////////////////////////////////
// dfPACKET_SC_MOVE_STOP
//
// Parameters: (CAyaPacket *)PacketClass. (DWORD)SessionID. (BYTE)Direction. (short)X. (short)Y.
// Return: 없음.
////////////////////////////////////////////////////////////////////
void	mpMoveStop(CAyaPacket *clpPacket, DWORD dwSessionID, BYTE byDir, short shX, short shY)
{
	st_NETWORK_PACKET_HEADER	stPacketHeader;

	stPacketHeader.byCode = dfNETWORK_PACKET_CODE;
	stPacketHeader.bySize = 9;
	stPacketHeader.byType = dfPACKET_SC_MOVE_STOP;

	clpPacket->PutData((char *)&stPacketHeader, dfNETWORK_PACKET_HEADER_SIZE);

	*clpPacket << dwSessionID;
	*clpPacket << byDir;
	*clpPacket << shX;
	*clpPacket << shY;

	*clpPacket << (BYTE)dfNETWORK_PACKET_END;
}




////////////////////////////////////////////////////////////////////
// dfPACKET_SC_ATTACK1
//
// Parameters: (CAyaPacket *)PacketClass. (DWORD)SessionID. (BYTE)Direction. (short)X. (short)Y.
// Return: 없음.
////////////////////////////////////////////////////////////////////
void	mpAttack1(CAyaPacket *clpPacket, DWORD dwSessionID, BYTE byDir, short shX, short shY)
{
	st_NETWORK_PACKET_HEADER	stPacketHeader;

	stPacketHeader.byCode = dfNETWORK_PACKET_CODE;
	stPacketHeader.bySize = 9;
	stPacketHeader.byType = dfPACKET_SC_ATTACK1;

	clpPacket->PutData((char *)&stPacketHeader, dfNETWORK_PACKET_HEADER_SIZE);

	*clpPacket << dwSessionID;
	*clpPacket << byDir;
	*clpPacket << shX;
	*clpPacket << shY;

	*clpPacket << (BYTE)dfNETWORK_PACKET_END;
}


////////////////////////////////////////////////////////////////////
// dfPACKET_SC_ATTACK2
//
// Parameters: (CAyaPacket *)PacketClass. (DWORD)SessionID. (BYTE)Direction. (short)X. (short)Y.
// Return: 없음.
////////////////////////////////////////////////////////////////////
void	mpAttack2(CAyaPacket *clpPacket, DWORD dwSessionID, BYTE byDir, short shX, short shY)
{
	st_NETWORK_PACKET_HEADER	stPacketHeader;

	stPacketHeader.byCode = dfNETWORK_PACKET_CODE;
	stPacketHeader.bySize = 9;
	stPacketHeader.byType = dfPACKET_SC_ATTACK2;

	clpPacket->PutData((char *)&stPacketHeader, dfNETWORK_PACKET_HEADER_SIZE);

	*clpPacket << dwSessionID;
	*clpPacket << byDir;
	*clpPacket << shX;
	*clpPacket << shY;

	*clpPacket << (BYTE)dfNETWORK_PACKET_END;
}


////////////////////////////////////////////////////////////////////
// dfPACKET_SC_ATTACK3
//
// Parameters: (CAyaPacket *)PacketClass. (DWORD)SessionID. (BYTE)Direction. (short)X. (short)Y.
// Return: 없음.
////////////////////////////////////////////////////////////////////
void	mpAttack3(CAyaPacket *clpPacket, DWORD dwSessionID, BYTE byDir, short shX, short shY)
{
	st_NETWORK_PACKET_HEADER	stPacketHeader;

	stPacketHeader.byCode = dfNETWORK_PACKET_CODE;
	stPacketHeader.bySize = 9;
	stPacketHeader.byType = dfPACKET_SC_ATTACK3;

	clpPacket->PutData((char *)&stPacketHeader, dfNETWORK_PACKET_HEADER_SIZE);

	*clpPacket << dwSessionID;
	*clpPacket << byDir;
	*clpPacket << shX;
	*clpPacket << shY;

	*clpPacket << (BYTE)dfNETWORK_PACKET_END;
}



////////////////////////////////////////////////////////////////////
// dfPACKET_SC_DAMAGE
//
// Parameters: (CAyaPacket *)PacketClass. (DWORD)AttackID. (DWORD)DamageID. (short)DamageHP.
// Return: 없음.
////////////////////////////////////////////////////////////////////
void	mpDamage(CAyaPacket *clpPacket, DWORD dwAttackID, DWORD dwDamageID, short shHP)
{
	st_NETWORK_PACKET_HEADER	stPacketHeader;

	stPacketHeader.byCode = dfNETWORK_PACKET_CODE;
	stPacketHeader.bySize = 9;
	stPacketHeader.byType = dfPACKET_SC_DAMAGE;

	clpPacket->PutData((char *)&stPacketHeader, dfNETWORK_PACKET_HEADER_SIZE);

	*clpPacket << dwAttackID;
	*clpPacket << dwDamageID;
	*clpPacket << (char)shHP;

	*clpPacket << (BYTE)dfNETWORK_PACKET_END;

}