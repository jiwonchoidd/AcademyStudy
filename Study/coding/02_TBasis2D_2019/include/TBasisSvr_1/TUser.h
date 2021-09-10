#pragma once
#include "TNpc.h"

// 어떤 메세지인지 구분해야 한다. ( 필요성 )
// 프로토콜 생성
typedef struct _OVERLAPPED2 : OVERLAPPED
{
	enum { MODE_RECV = 0, MODE_SEND = 1 };
	int		m_iFlags;
	_OVERLAPPED2()
	{
		m_iFlags = MODE_RECV;
	}

}OVERLAPPED2;

struct GUIDComparer
{
	bool operator()(const tGUID & Left, const tGUID & Right) const
	{
		return memcmp(&Left, &Right, sizeof(Right)) < 0;
	}
};

class TUser
{
public:
	OVERLAPPED2		m_ov;
	int				m_iEvent; // 이벤트 배열의 인덱스
	string			m_Name;
	tGUID		    m_Guid;
	SOCKET			m_Socket;
	WSABUF			m_wsaBuffer;
	char			m_strBuffer[2048];
	char			m_sendBuffer[2048];
	TStreamPacket	m_StreamPacket;
	bool			m_bUsed;
	tGUID			m_GuidActiveCharacter;
	TCharacter*     m_pCharacter;
	std::map<tGUID, TCharacter, GUIDComparer> m_CharacterList;

	void			Dispatch(DWORD deByteSize,
		LPOVERLAPPED ov)
	{
		OVERLAPPED2* pOV = (OVERLAPPED2*)ov;
		if (deByteSize == 0) return;
		// send
		// recv
		// 패킷처리
		if (pOV->m_iFlags == OVERLAPPED2::MODE_RECV)
		{
			m_StreamPacket.Put(
				m_wsaBuffer.buf,
				deByteSize,
				this);
		}
		WaitForpacketReceive();
	}
	void WaitForpacketReceive()
	{
		int iRet = 0;
		DWORD cbTransferred, flags = 0;
		m_ov.m_iFlags = OVERLAPPED2::MODE_RECV;
		m_wsaBuffer.buf = m_strBuffer;
		m_wsaBuffer.len = 2048;
		iRet = WSARecv(m_Socket,
			&(m_wsaBuffer), 1,
			&cbTransferred, &flags,
			(LPOVERLAPPED)&m_ov, NULL);
		if (iRet == SOCKET_ERROR)
		{
			if (WSAGetLastError() != WSA_IO_PENDING)
			{
				I_DebugStr.T_ERROR();
			}
		}
	}
	friend ostream& operator<<(ostream &stream, TUser& info)
	{
		stream << info.m_Guid << endl;
		stream << info.m_Name << endl;
		stream << info.m_GuidActiveCharacter << endl;
		stream << info.m_CharacterList.size() << endl;

		for (auto& tChar : info.m_CharacterList)
		{
			stream << tChar.first << endl;
			stream << tChar.second << endl;
		}
		return stream;
	}

	friend istream& operator>>(istream& stream, TUser& info)
	{
		int iNumCharacter = 0;
		tGUID tGuid;
		TCharacter tChar;
		info.m_CharacterList.clear();

		stream >> info.m_Guid;
		stream >> info.m_Name;
		stream >> info.m_GuidActiveCharacter;
		stream >> iNumCharacter;

		for (int i = 0; i < iNumCharacter; i++)
		{
			stream >> tGuid;
			stream >> tChar;
			info.m_CharacterList[tGuid] = tChar;
		}
		return stream;
	}
public:
	TUser();
	virtual ~TUser();
};
