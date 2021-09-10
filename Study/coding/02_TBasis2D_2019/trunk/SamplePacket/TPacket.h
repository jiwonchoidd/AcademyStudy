#pragma once
#include <TStd.h>
// struct TUserData { header, name[9], age };
// 사용방법(1)
	// 준비 : char buffer[2048] = {0};
	// TPacket data;
	// data << header;
	// data << "홍길동";
	// data << age;
	// 사용방법(2)
// data << header << "홍길동" << 20;
//char buffer[2048] = { 0 };
// TPacket data;
// data >> header;
// data >> "홍길동";
// data >> age;
// 사용방법(2)
// data >> header >> "홍길동" << 20;
#define MAX_BUFFER_SIZE 4096
//typedef basic_string<TCHAR> T_STR;
//typedef basic_string<wchar_t> W_STG;
//typedef basic_string<char>  C_STR;

class TPacket
{
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
public:
	char	m_szBuffer[MAX_BUFFER_SIZE];
	char*   m_pReadPos;
	char*   m_pWritePos;
	int		m_iDataSize;
	char*	m_pLen;
	char*	m_pType;
	UPACKET		  m_UPacket;
public:
	int			PutData(const char* pData, int iSize);
	int			GetData(char* pData, int iSize);
	// 입력
	TPacket&   operator << (int  iValue);
	TPacket&   operator << (float fValue);
	TPacket&   operator << (char cValue);
	TPacket&   operator << (WORD Value); 
	TPacket&   operator << (C_STR Value);
	TPacket&   operator << (CHAR* Value);	
	// 출력
	TPacket&   operator >> (int& iValue);
	TPacket&   operator >> (float& fValue);
	TPacket&   operator >> (char& cValue);	
	TPacket&   operator >> (WORD& Value);
	TPacket&   operator >> (C_STR& Value);
	TPacket&   operator >> (CHAR* Value);
public:
	TPacket();
	virtual ~TPacket();
};

