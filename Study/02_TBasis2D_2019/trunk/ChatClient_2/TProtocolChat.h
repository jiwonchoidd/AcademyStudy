#ifndef SAMPLE_MUTEX
#define SAMPLE_MUTEX
#include "TProtocol.h"

#pragma  pack(push, 1 )
typedef struct {
	int		user_idx;
	BYTE	posX;
	BYTE	posY;
	BYTE	direction; //0 ~7 8방향
}TPACKET_USER_POSITION;
#define PACKET_USER_POSITION	3000

typedef struct {
	int		user_idx;
	char	uid[21];
	char	ups[15];
}TPACKET_USER_LOGIN;
#define PACKET_USER_LOGIN	2000

// 서버
#define PACKET_CHAT_NAME_REQ 1000 // 대화명 입력해라.
// 클라이언트
#define PACKET_CHAT_NAME_ACK 1001 // 대화명 입력 전송.
// 보통 채팅 메세지
#define PACKET_CHAT_MSG 1002 // 대화명 입력 전송.

#pragma pack(pop)

#endif