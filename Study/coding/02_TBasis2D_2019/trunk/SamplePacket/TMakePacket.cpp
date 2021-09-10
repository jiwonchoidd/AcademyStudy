#include "TMakePacket.h"

void TMakePacket::MoveCharacter(TPacket* packet,
						int iID, 
						int iDir, 
						int iX, int iY)
{
	PACKET_HEADER header;
	header.type = PACKET_MOVE_CHARACTER;
	*packet << iID << iDir << iX << iY;
	header.len = packet->m_iDataSize + PACKET_HEADER_SIZE;
}

TMakePacket::TMakePacket()
{
}


TMakePacket::~TMakePacket()
{
}
