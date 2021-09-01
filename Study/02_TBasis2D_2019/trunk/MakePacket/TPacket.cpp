#include "TPacket.h"
void TPacket::PutData(const char* pData,
	int iSize)
{
	m_uPacket.ph.len += iSize;
	memcpy(m_pOffset, pData, iSize);
	m_pOffset += iSize;
}
void TPacket::GetData(const char* pData,
	int iSize)
{
	memcpy(const_cast<char*>(pData),
		m_pOffset, iSize);
	m_pOffset += iSize;
}
TPacket & TPacket::operator << (int data)
{
	PutData((char*)&data, sizeof(int));
	return *this;
}
TPacket & TPacket::operator << (short data)
{
	PutData((char*)&data, sizeof(short));
	return *this;
}
TPacket & TPacket::operator << (long data)
{
	PutData((char*)&data, sizeof(long));
	return *this;
}
TPacket & TPacket::operator << (DWORD data)
{
	PutData((char*)&data, sizeof(DWORD));
	return *this;
}
TPacket & TPacket::operator << (float data)
{
	PutData((char*)&data, sizeof(float));
	return *this;
}
TPacket & TPacket::operator << (char* data)
{
	int iSize = strlen(data) + 1;
	PutData(data, iSize);
	return *this;
}
TPacket & TPacket::operator << (string data)
{
	PutData(data.c_str(), data.size());
	return *this;
}
/////////////////////////////////
TPacket & TPacket::operator >> (int& data)
{
	GetData((char*)&data, sizeof(int));
	return *this;
}
TPacket & TPacket::operator >> (short& data)
{
	GetData((char*)&data, sizeof(short));
	return *this;
}
TPacket & TPacket::operator >> (long& data)
{
	GetData((char*)&data, sizeof(long));
	return *this;
}
TPacket & TPacket::operator >> (DWORD& data)
{
	GetData((char*)&data, sizeof(DWORD));
	return *this;
}
TPacket & TPacket::operator >> (float& data)
{
	GetData((char*)&data, sizeof(float));
	return *this;
}
TPacket & TPacket::operator >> (char* data)
{
	int iSize = strlen(m_pOffset) + 1;
	GetData(data, iSize);
	return *this;
}
TPacket & TPacket::operator >> (string& data)
{
	/*int ilen = strlen(m_pOffset) + 1;
	char buffer[256] = { 0, };
	GetData(buffer, ilen);
	data = buffer;*/

	int iSize = strlen(m_pOffset) + 1;
	data.resize(iSize);
	GetData(const_cast<char*>(data.c_str()),iSize);
	return *this;
}
TPacket::TPacket()
{
	ZeroMemory(&m_uPacket, sizeof(UPACKET));
	m_pOffset = m_uPacket.msg;
}
TPacket::TPacket(int  dwType)
{
	ZeroMemory(&m_uPacket, sizeof(UPACKET));
	m_uPacket.ph.len = 0;
	m_uPacket.ph.type = dwType;
	m_pOffset = m_uPacket.msg;
}

TPacket::~TPacket()
{
}
