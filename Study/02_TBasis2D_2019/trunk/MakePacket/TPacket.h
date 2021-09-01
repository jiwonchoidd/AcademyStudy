#pragma once
#include <windows.h>
#include <tchar.h>
#include <vector>
#include <map>
#include <list>
#include <string>
#include "TProtocol.h"
using namespace std;
typedef basic_string<TCHAR> T_STR;
typedef basic_string<wchar_t> W_STG;
typedef basic_string<char>  C_STR;


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
class TPacket
{
public:
	char*		m_pOffset;
	UPACKET		m_uPacket;
public:
	void PutData(const char* pData,	int iSize);
	void GetData(const char* pData,	int iSize);
public:
	TPacket & operator << (int data);
	TPacket & operator << (short data);
	TPacket & operator << (long data);
	TPacket & operator << (float data);
	//TPacket & operator << (byte data);
	TPacket & operator << (char* data);
	TPacket & operator << (DWORD data);
	TPacket & operator << (string data);

	TPacket & operator >> (int& data);
	TPacket & operator >> (short& data);
	TPacket & operator >> (long& data);
	TPacket & operator >> (float& data);
	//TPacket & operator >> (byte& data);
	TPacket & operator >> (char* data);
	TPacket & operator >> (DWORD& data);
	TPacket & operator >> (string& data);
public:
	TPacket();
	TPacket(int  dwType);
	~TPacket();
};

