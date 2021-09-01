#pragma once
#include <windows.h>

#define PACKET_HEADER_SIZE 4

#pragma pack( push, 1)
typedef struct {
	WORD  len;   // 패킷의 전체 길이
	WORD  type;  // 패킷의 타입(종류)
				 //WORD  Index; // 패킷의 고유한 인덱스
				 //WORD  time;  // 패킷의 전달 시간
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

// [0] ~ [12][13][14] [14][15][16]
//////////////  패킷의 타입들 /////////
#define PACKET_CHAT_MSG    1000
#define PACKET_GAME_START    2000
#define PACKET_GAME_END    3000
#define PACKET_CREATE_CHARACTER    4000
#define PACKET_MOVE_CHARACTER    5000
/*
MSG = "안녕하세여!"

UPACKET  sendPacket;
sendPacket.ph.len = strlen(MSG) + PACKET_HEADER_SIZE;
sendPacket.ph.type = PACKET_CHAT_MSG;
sendPacket.msg = MSG;

UPACKET  sendPacket;
sendPacket.ph.len = strlen(CHARACTER_INFO) + PACKET_HEADER_SIZE;
sendPacket.ph.type = PACKET_CREATE_CHARACTER;
sendPacket.msg = CHARACTER_INFO;
// 4 바이트 패킹
struct TData
{
int   iA; // 4
int   iD; // 4
short sB; // 2 덤 2byte
short sC; // 2 덤 2byte
short sC; // 2 덤 2byte
byte  bC; // 3 byte
byte  bC; // 3 byte
};
// 200
UPACKET a[10]; for( send(a[i] )
// a[0]a[1]a[2]a[3] == > 스트림(연속적인 물의 흐름 )
char buffer[256];
UPACKET a[10];
while(1)
{
iRecvByte=recv(buffer, 256)
a[0]생성, a[1]= 56 + 144byte;일부분만 받음
200-56 = 144 ~ 256  256-144= 12
a[2] = 144~ 244;
a[3] = 244~256=12
}
recvbuffer = recv();
if( strlen(recvbuffer) > PACKET_HEADER_SIZE)
{
// if( strlen(recvbuffer) > packet.len )
{

}
}
*/

