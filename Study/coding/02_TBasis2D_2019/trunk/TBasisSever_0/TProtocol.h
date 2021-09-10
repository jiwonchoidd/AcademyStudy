#ifndef TPROTOCAL_HEADER
#define TPROTOCAL_HEADER
#include <windows.h>

#pragma  pack( push, 1 )
typedef struct {
	WORD len;
	WORD type;
}PACKET_HEADER;
typedef struct{
	PACKET_HEADER ph;
	char     msg[2048];
}UPACKET, *P_UPACKET;

#pragma pack(pop)

#endif
