#pragma once
#include "TStd.h"
#include <process.h>
#include "TProtocol.h"
#include "TStreamPacket.h"
#include "TGameUser.h"

class TClient 
{
public:
	TStreamPacket		m_StreamPacket;
	int					m_iSocket;
	unsigned long 		m_hSendThread;
	unsigned long 		m_hReceiveThread;
	CRITICAL_SECTION	m_CS;
	bool				m_bExit;
	TCharacterMap		m_PlayerCharacterList;
	TCharacterMap		m_GameNpcList;
public:
	char		m_strBuffer[128];
	bool		m_bSend;
	bool		m_bConnectOK;
	bool		m_bGameStart;
	bool		Init();
	bool		Frame();
	bool		Release();
	int			CreateConnectSocket( int iPort);
	void		DisConnected();
	int			SendMsg( char* pMsg, int iSize, WORD code );
	int			SendMsg(const stringstream& sendPacket);
	int			ProcessPacket();
public:
public:
	tGUID		m_tGuidUser;
	tGUID		m_tGuidLogin;
	tGUID		m_tGuidActiveCharacter;
	int			LoginAck(UPACKET& packet);
	bool		Verification();
	bool		IsVerificationLogin;
	bool		IsVerificationLobby;
	bool		IsVerificationZone;
	int			VerificationAck(UPACKET& packet);
	TCharacter  RecvCharacter(UPACKET& packet);
	TCharacterMap * RecvZonePlayersInfo(UPACKET& packet);
	bool		SendRespawnPlayer(TCharacter& info);
	
	void		SendZoneEntry();
	bool		SendZoneEntryReq(TCharacter& info);

	void		RecvZoneInfo(UPACKET& packet);
	void		RecvZoneEntry();
	void		SpawnPCList();
	void		SpawnNPCList();
	TCharacter  GetCharacterInfo();
	void		OnPossess();
public:
	TClient(void);
	~TClient(void);
};
