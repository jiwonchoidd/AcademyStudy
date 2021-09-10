#pragma once
#include "TPacket.h"
#include "TProtocol.h"
class TMakePacket
{
public:
	static void MoveCharacter(TPacket* packet,
								int iID,
								int iDir,
								int iX, int iY);
public:
	TMakePacket();
	virtual ~TMakePacket();
};

