#pragma once
#include <windows.h>
#define PACKET_HEADER_SIZE 4
#define PACKET_MAX_DATA_SIZE 2048

#pragma pack( push, 1)
typedef struct
{
	WORD len;   // 데이터 길이+헤더길이
	WORD type;  // 패킷 타입
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

typedef struct {
	int   iIndex;
	char  szName[20];
}USER_BASE;
#define USER_BASE_SIZE 24
#define USER_NAME_SIZE 20
typedef struct {
	USER_BASE base;
	char  msg[30];
}USER_NAME_REQ;
typedef struct {
	int   iIndex;
	char  szName[USER_NAME_SIZE];
}USER_NAME_ACK;
typedef struct {
	char  szName[USER_NAME_SIZE];
	char  msg[PACKET_MAX_DATA_SIZE - 20];// 메세지 실제 크기
}USER_CHAT_MSG;

#pragma pack(pop)
