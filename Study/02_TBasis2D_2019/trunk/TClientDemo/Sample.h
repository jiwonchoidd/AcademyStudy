//http://msdn.microsoft.com/ko-kr/library/35fhdx10.aspx
#pragma once
#include "TCore.h"
#include "TDemoObjMgr.h"
#include "TClient.h"
#include "TGameUser.h"
#include "TUdpSocket.h"

class Sample  : public TCore
{
public:	
	TObject				m_Map;
	HFONT				m_hLogFont;
	TUdpSocket			m_Udp;
	TClient				m_Client;
	int					m_iSerIndex;
	POINT				m_CursorPos;
	int					m_iPlayState;
public:
	typedef bool (Sample::*CallFuction)();
	std::map<int, CallFuction>		m_fnExecuteScene;
	typedef std::map<int, CallFuction>::iterator FunctionIterator;
	bool				FrameLogin();
	bool				FrameLobby();
	bool				FrameIngame();
	bool				RenderLogin();
	bool				RenderLobby();
	bool				RenderIngame();
public:
	bool				Init();
	bool				Render();
	bool				PostRender();
	bool				Frame();
	bool				Release();	
public:
	Sample(void);
	~Sample(void);
};
