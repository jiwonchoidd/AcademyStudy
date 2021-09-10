#pragma once
#include <windows.h>
#define PACKET_HEADER_SIZE 4
#define PACKET_MAX_DATA_SIZE 2048

#pragma pack( push, 1)
typedef struct
{
	WORD len;   // ������ ����+�������
	WORD type;  // ��Ŷ Ÿ��
}PACKET_HEADER;
typedef struct {
	PACKET_HEADER ph;
	char          msg[PACKET_MAX_DATA_SIZE];
}UPACKET;

#define PACKET_CHAR_MSG			 1000 // DATA
#define PACKET_CHAR_NAME_SC_REQ  1001 // DATA
#define PACKET_CHAR_NAME_CS_ACK  1002 // DATA
#define PACKET_JOIN_SIGNIN_SC	 1003 // x
#define PACKET_JOIN_USER_SC		 1004 // DATA
#define PACKET_DRUP_CS_REQ		 1005 // x
#define PACKET_DRUP_SC_ACK		 1006 // x
#define PACKET_DRUP_USERS_SC	 1007 // DATA


// SIZE
#define USER_BASE_SIZE 24
#define USER_NAME_SIZE 20
#define USER_NAME_REQ_MSG_SIZE 30

typedef struct {
	int   iIndex;
	char  szName[USER_NAME_SIZE];
}USER_BASE;
typedef struct {
	int   iIndex;
	char  szName[USER_NAME_SIZE];
}USER_NAME_ACK;

typedef struct {
	USER_BASE base;
	char  msg[USER_NAME_REQ_MSG_SIZE];
}USER_NAME_REQ;
typedef struct {
	char  szName[USER_NAME_SIZE];
	char  msg[PACKET_MAX_DATA_SIZE - USER_NAME_SIZE];// �޼��� ���� ũ��
}USER_CHAT_MSG;

#pragma pack(pop)