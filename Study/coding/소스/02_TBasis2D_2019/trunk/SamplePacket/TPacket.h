#pragma once
#include <TStd.h>
// struct TUserData { header, name[9], age };
// �����(1)
	// �غ� : char buffer[2048] = {0};
	// TPacket data;
	// data << header;
	// data << "ȫ�浿";
	// data << age;
	// �����(2)
// data << header << "ȫ�浿" << 20;
//char buffer[2048] = { 0 };
// TPacket data;
// data >> header;
// data >> "ȫ�浿";
// data >> age;
// �����(2)
// data >> header >> "ȫ�浿" << 20;
#define MAX_BUFFER_SIZE 4096
//typedef basic_string<TCHAR> T_STR;
//typedef basic_string<wchar_t> W_STG;
//typedef basic_string<char>  C_STR;

class TPacket
{
	typedef struct {
		WORD  len;   // ��Ŷ�� ��ü ����
		WORD  type;  // ��Ŷ�� Ÿ��(����)
					 //WORD  Index; // ��Ŷ�� ������ �ε���
					 //WORD  time;  // ��Ŷ�� ���� �ð�
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
	// �Է�
	TPacket&   operator << (int  iValue);
	TPacket&   operator << (float fValue);
	TPacket&   operator << (char cValue);
	TPacket&   operator << (WORD Value); 
	TPacket&   operator << (C_STR Value);
	TPacket&   operator << (CHAR* Value);	
	// ���
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

