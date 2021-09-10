#pragma once
#include <windows.h>
#define PACKET_HEADER_SIZE 4
#pragma pack( push, 1)
typedef struct {
	WORD  len;   // 패킷의 전체 길이
	WORD  type;  // 패킷의 타입(종류)
}PACKET_HEADER;
typedef struct {
	PACKET_HEADER ph;
	char		  msg[2048];
}UPACKET, *P_UPACKET;
///   케릭터 생성 패킷
typedef struct {
	byte    szName[13];
	int		iIndex;
	int		iData[50];
}CHARACTER_INFO;
///   아이템 생성 패킷
typedef struct {
	byte    szName[13];
	int		iIndex;
	int		iData[10];
}ITEM_INFO;
#pragma pack(pop) // 복원
//////////////  패킷의 타입들 /////////
#define PACKET_CHAT_MSG    1000
#define PACKET_GAME_START    2000
#define PACKET_GAME_END    3000
#define PACKET_CREATE_CHARACTER    4000
#define PACKET_MOVE_CHARACTER    5000

