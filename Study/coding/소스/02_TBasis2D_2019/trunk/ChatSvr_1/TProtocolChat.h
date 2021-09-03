#include "TProtocol.h"

#define PACKET_CHAR_NAME_REQ 1000
#define PACKET_CHAR_NAME_ACK 1001
#define PACKET_CHAR_MSG      1002

#pragma  pack( push, 1 )
/// 필요한 프로토콜 정의
typedef struct {
	WORD  user_idx; // 케릭터의 고유한 인덱스
	WORD  posX;     // 좌표
	WORD  posY;
	WORD  direction; // 방향 ( 0~7 ) 8방향
}TPACKET_USER_POSITION;
#define PACKET_USER_POSITION   3000

#pragma pack(pop)