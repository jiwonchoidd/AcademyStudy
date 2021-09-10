#ifndef SAMPLE_MUTEX
#define SAMPLE_MUTEX
#include <windows.h>

#pragma  pack(push, 1 )
#define PACKET_HEADER_SIZE 4

typedef struct {
	WORD	len;	// msg ����Ʈ ũ��
	WORD	type;	// � ��Ŷ�̴�.
}PACKET_HEADER;

typedef struct {
	PACKET_HEADER	ph;
	char			msg[1024];
}UPACKET, *P_UPACKET;

typedef struct {
	int		user_idx;
	BYTE	posX;
	BYTE	posY;
	BYTE	direction; //0 ~7 8����
}TPACKET_USER_POSITION;
#define PACKET_USER_POSITION	3000

typedef struct {
	int		user_idx;
	char	uid[21];
	char	ups[15];
}TPACKET_USER_LOGIN;
#define PACKET_USER_LOGIN	2000

// ����
#define PACKET_CHAT_NAME_REQ 1000 // ��ȭ�� �Է��ض�.
// Ŭ���̾�Ʈ
#define PACKET_CHAT_NAME_ACK 1001 // ��ȭ�� �Է� ����.
// ���� ä�� �޼���
#define PACKET_CHAT_MSG 1002 // ��ȭ�� �Է� ����.

#pragma pack(pop)

#endif