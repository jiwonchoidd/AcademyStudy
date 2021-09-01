#pragma once
#include "TNpc.h"
#define MAX_BUFFER 2048
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



class TActiveCharacter
{
public:
	TActiveCharacter() {};
	virtual ~TActiveCharacter() {};

	map<tGUID, TCharacter, GUIDComparer> players;

	friend ostream& operator<<(ostream &stream, TActiveCharacter& info)
	{
		stream << info.players.size();
		for (auto& data : info.players)
		{
			stream << data.first;
			stream << data.second;
		}
		return stream;
	}

	friend istream &operator>>(istream &stream, TActiveCharacter& info)
	{
		info.players.clear();		

		size_t nPlayers = 0;
		tGUID tGuid;
		TCharacter tChar;
		stream >> nPlayers;
		for (int i = 0; i < nPlayers; i++)
		{
			stream >> tGuid;
			stream >> tChar;
			info.players.insert(make_pair(tGuid,tChar));
		}
		return stream;
	}
};

class TUser
{
public:
	OVERLAPPED2		m_ov;
	int				m_iEvent; // 이벤트 배열의 인덱스
	CONNECT_TYPE    m_ConnectType;
	string			m_Name;
	tGUID		    m_LoginGuid;
	tGUID		    m_Guid;
	tGUID			m_GuidActiveCharacter;

	SOCKET			m_Socket;
	WSABUF			m_wsaRecvBuffer;
	WSABUF			m_wsaSendBuffer;
	char			m_strBuffer[MAX_BUFFER];
	char			m_sendBuffer[MAX_BUFFER];
	TStreamPacket	m_StreamPacket;
	bool			m_bUsed;	
	TCharacter*     m_pCharacter;
	std::map<tGUID, TCharacter, GUIDComparer> m_CharacterList;

public:
	void	Dispatch(DWORD deByteSize, LPOVERLAPPED ov);
	void	WaitForpacketReceive();
public:
	friend ostream& operator<<(ostream &stream, TUser& info)
	{		
		stream << info.m_Guid;		
		stream << info.m_GuidActiveCharacter;
		stream << info.m_Name << endl;
		stream << info.m_CharacterList.size();

		for (auto& tChar : info.m_CharacterList)
		{
			stream << tChar.first;
			stream << tChar.second;
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
		stream >> info.m_GuidActiveCharacter;
		stream >> info.m_Name;
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
