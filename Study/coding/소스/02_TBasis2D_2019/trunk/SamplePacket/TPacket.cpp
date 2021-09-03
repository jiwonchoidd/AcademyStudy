#include "TPacket.h"

int TPacket::PutData(const char* pData, int iSize)
{
	memcpy(m_pWritePos, pData, iSize);
	m_pWritePos += iSize;
	m_iDataSize += iSize;
	return iSize;
}
int TPacket::GetData(char* pData, int iSize)
{	
	memcpy(pData, m_pReadPos, iSize);
	m_pReadPos += iSize;
	m_iDataSize -= iSize;
	return iSize;
}
TPacket&  TPacket::operator << (int  iValue)
{
	PutData( reinterpret_cast<char*>(&iValue), sizeof(int));
	return *this;
}
TPacket&  TPacket::operator >> (int&  iValue)
{
	GetData(reinterpret_cast<char*>(&iValue), sizeof(int));
	return *this;
}
TPacket&  TPacket::operator << (float  fValue)
{
	PutData(reinterpret_cast<char*>(&fValue), sizeof(float));
	return *this;
}
TPacket&  TPacket::operator >> (float&  fValue)
{
	GetData(reinterpret_cast<char*>(&fValue), sizeof(float));
	return *this;
}
TPacket&  TPacket::operator << (char  cValue)
{
	PutData(reinterpret_cast<char*>(&cValue), sizeof(char));
	return *this;
}
TPacket&  TPacket::operator >> (char&  cValue)
{
	GetData(reinterpret_cast<char*>(&cValue), sizeof(char));
	return *this;
}
TPacket&  TPacket::operator << (WORD  iValue)
{
	PutData(reinterpret_cast<char*>(&iValue), sizeof(WORD));
	return *this;
}
TPacket&  TPacket::operator >> (WORD&  iValue)
{
	GetData(reinterpret_cast<char*>(&iValue), sizeof(WORD));
	return *this;
}
TPacket&  TPacket::operator << (C_STR  iValue)
{
	PutData(iValue.c_str(), iValue.size()+1);
	return *this;
}
TPacket&  TPacket::operator >> (C_STR&  iValue)
{
	int ilen = strlen(m_pReadPos)+1;
	char buffer[256] = { 0 };
	GetData(reinterpret_cast<char*>(buffer), ilen);
	iValue = buffer;
	return *this;
}
TPacket&  TPacket::operator << (CHAR*  iValue)
{
	PutData(reinterpret_cast<char*>(iValue), strlen(iValue)+1);
	return *this;
}
TPacket&  TPacket::operator >> (CHAR*  iValue)
{
	int ilen = strlen(m_pReadPos)+1;
	GetData(reinterpret_cast<char*>(iValue), ilen);
	return *this;
}
TPacket::TPacket()
{
	m_pLen = m_pWritePos = m_szBuffer;
	m_iDataSize = 0;
	m_pType = &m_pLen[2];

	PACKET_HEADER head;
	head.len = 4;
	head.type = 1;
	PutData(reinterpret_cast<char*>(&head.len), sizeof(WORD));
	PutData(reinterpret_cast<char*>(&head.type), sizeof(WORD));

	m_pReadPos = &m_pLen[4];
}


TPacket::~TPacket()
{
}
