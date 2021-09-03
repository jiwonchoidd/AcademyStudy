#ifndef TBASIS_TPROTOCOL
#define TBASIS_TPROTOCOL
#include <windows.h>
#pragma  pack(push, 1 )

struct tGUID : public GUID
{
	friend ostream& operator<<(ostream &stream, const tGUID& tGuid)
	{
		stream << tGuid.Data1;
		stream << tGuid.Data2;
		stream << tGuid.Data3;
		stream << tGuid.Data4[0];
		stream << tGuid.Data4[1];
		stream << tGuid.Data4[2];
		stream << tGuid.Data4[3];
		stream << tGuid.Data4[4];
		stream << tGuid.Data4[5];
		stream << tGuid.Data4[6];
		stream << tGuid.Data4[7];
		return stream;
	}

	friend istream& operator>>(istream& stream, const tGUID& tGuid)
	{
		stream >> DWORD(tGuid.Data1);
		stream >> USHORT(tGuid.Data2);
		stream >> USHORT(tGuid.Data3);
		stream >> UCHAR(tGuid.Data4[0]);
		stream >> UCHAR(tGuid.Data4[1]);
		stream >> UCHAR(tGuid.Data4[2]);
		stream >> UCHAR(tGuid.Data4[3]);
		stream >> UCHAR(tGuid.Data4[4]);
		stream >> UCHAR(tGuid.Data4[5]);
		stream >> UCHAR(tGuid.Data4[6]);
		stream >> UCHAR(tGuid.Data4[7]);
		return stream;
	}
};

typedef struct {
	WORD	len;	// msg 바이트 크기
	WORD	type;	// 어떤 패킷이다.
	WORD    iotype;	// 패킷 입출력 종류
	DWORD   time;   // 패킷 전송 시간 
}PACKET_HEADER;

typedef struct {
	PACKET_HEADER	ph;
	char			msg[1024];
}UPACKET, *P_UPACKET;
#define PACKET_HEADER_SIZE 10

// 서버
#define PACKET_CHAT_NAME_REQ	1000 // 대화명 입력해라.
// 클라이언트
#define PACKET_CHAT_NAME_ACK	1001 // 대화명 입력 전송.
// 보통 채팅 메세지
#define PACKET_CHAT_MSG			1002 // 대화명 입력 전송.
#define PACKET_LOGIN			1003 // 로그인
#define PACKET_LOGIOUT			1004 // 로그아웃

#define PACKET_CREATE_ACCOUNT	2000 // 계정생성
#define PACKET_DELETE_ACCOUNT	2001 // 계정삭제

typedef struct {
	WORD	user_idx;
	WORD	posX;
	WORD	posY;
	WORD	direction; //0 ~7 8방향
}TPACKET_USER_POSITION;
#define PACKET_USER_POSITION	3000 // 케릭터 위치 이동 시작

typedef struct {
	int m_iID;
	float m_fPos[3];
	float m_fAngle[3];
	float m_fVelocity[3];
	float m_fHealth;
	bool  m_bAttacking;
	bool  m_bAlive;
}TPACKET_CHARACTER_INFO;

#define PACKET_CREATE_CHARACTER 4000 // 케릭터 생성
#define PACKET_DELETE_CHARACTER 4001 // 케릭터 삭제
#define PACKET_HIT_CHARACTER	4002 // 케릭터 피격 받음
#define PACKET_HIT_MONSTER		4003 // 몬스터 피격 받음
#define PACKET_ATTACK_CHARACTER	4004 // 케릭터 공격 시작
#define PACKET_ATTACK_MONSTER	4005 // 몬스터 공격 시작
#define PACKET_DEAD_CHARACTER	4006 // 케릭터 죽음
#define PACKET_DEAD_MONSTER		4007 // 몬스터 죽음
#define PACKET_DAMAGE_CHARACTER	4008 // 케릭터 데미지 받음
#define PACKET_DAMAGE_MONSTER	4009 // 몬스터 데미지 받음
#define PACKET_SPAWN_CHARACTER	4010 // 케릭터 스폰
#define PACKET_SPAWN_MONSTER	4011 // 몬스터 스폰
#define PACKET_SYNC_CHARACTER	4012 // 케릭터 동기화
#define PACKET_SYNC_MONSTER		4013 // 몬스터 동기화

#pragma pack(pop)

#endif