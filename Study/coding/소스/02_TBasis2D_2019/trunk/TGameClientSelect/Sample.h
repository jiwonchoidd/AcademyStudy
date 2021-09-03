#pragma once
//#define PACKET_TEST
#include "TNetworkClient.h"
#include "TCore.h"
#include "TSceneIntro.h"
#include "TSceneLobby.h"
#include "TSceneInGame.h"
#include "TFiniteStateMachine.h"
#include "TGamePlayer.h"

// npc->server
// login->db
class Sample : public TCore
{
public:
	std::vector<TScene*>	m_SceneList;
	int						m_iCurrentScene;
	TBitmap					m_EditImage;
	TBitmap					m_ButtonImage;
	TCHAR	m_szChatBuffer[MAX_PATH] = { 0, };
	std::map<SOCKET, TGameOtherPlayer*>    m_userlist;
	TNetworkClient			m_Network;
public:	
	HANDLE						m_hExecuteMutex;
	bool						m_bUseNet;
	std::string					m_szName;
public:
	HWND  g_hEdit;
	HWND  g_hButton;
	RECT  g_rtEdit;
	RECT  g_rtButton;
	bool  m_bEditFocus = false;
public:
	bool  Init();
	bool  PreFrame();
	bool  Frame();
	bool  Render();	
	bool  PostRender() override;
	bool  Release();
public:
	bool  NetInit();
	bool  NetFrame();
	bool  NetRender();
	bool  NetRelease();
	void  SendGamePlayerData();
	void  DisplayText(const char* fmt, ...);
public:
	void  SendTest(const char*pData);
	void  SendAttack();
	bool  AddUser(TPACKET_CHARACTER_INFO& info);
	bool  AddNpc(TPACKET_CHARACTER_INFO& info );
	LRESULT	 MsgProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
public:
	Sample();
	~Sample();
};

