#include "Sample.h"
#include "TInput.h"
#include "TSoundMgr.h"
#include "TDebugString.h"

bool Sample::FrameLogin()
{
	return true;
}
bool Sample::FrameLobby()
{
	return true;
}
bool Sample::RenderLogin()
{
	return true;
}
bool Sample::RenderLobby()
{
	return true;
}
bool Sample::FrameIngame()
{
	m_Client.Frame();
	I_ScriptManager.Frame();

	if (m_Client.IsVerificationZone == false) return true;
	GetCursorPos(&m_CursorPos);
	ScreenToClient(g_hWnd, &m_CursorPos);

	TPACKET_USER_POSITION userdata;
	if (m_isActive)
	{
		DWORD dwLeftKey = I_Input.KeyCheck(VK_LEFT);
		DWORD dwRightKey = I_Input.KeyCheck(VK_RIGHT);
		DWORD dwUpKey = I_Input.KeyCheck(VK_UP);
		DWORD dwDownKey = I_Input.KeyCheck(VK_DOWN);

		// 해당 방향을 제외한다.
		if (dwLeftKey == KEY_UP)
		{
			I_GameUser.m_iDirection &= ~8;
		}
		if (dwRightKey == KEY_UP)
		{
			I_GameUser.m_iDirection &= ~2;
		}
		if (dwUpKey == KEY_UP)
		{
			I_GameUser.m_iDirection &= ~1;
		}
		if (dwDownKey == KEY_UP)
		{
			I_GameUser.m_iDirection &= ~4;
		}
		userdata.direction = I_GameUser.m_iDirection;
		userdata.posX = I_GameUser.m_fPosX;
		userdata.posY = I_GameUser.m_fPosY;
		userdata.user_idx = m_iSerIndex;

		char buffer[256] = { 0, };
		int iSize = sizeof(userdata);

		// 플레이어는 변경된 방향으로 1번만 전달하고 지속한다.
		// 정지 상태 및 방향전환 일 때 상태 변경 된다.
		if (dwLeftKey == KEY_PUSH)
		{
			userdata.direction |= 8;			
			memcpy(buffer, &userdata, iSize);
			m_Client.SendMsg(buffer, iSize, PACKET_USER_POSITION);
			m_iPlayState = userdata.direction;
		}
		if (dwRightKey == KEY_PUSH)
		{
			userdata.direction |= 2;			
			memcpy(buffer, &userdata, iSize);
			m_Client.SendMsg(buffer, iSize, PACKET_USER_POSITION);
			m_iPlayState = userdata.direction;
		}
		if (dwUpKey == KEY_PUSH)
		{
			userdata.direction |= 1;			
			memcpy(buffer, &userdata, iSize);
			m_Client.SendMsg(buffer, iSize, PACKET_USER_POSITION);
			m_iPlayState = userdata.direction;
		}
		if (dwDownKey == KEY_PUSH)
		{
			userdata.direction |= 4;			
			memcpy(buffer, &userdata, iSize);
			m_Client.SendMsg(buffer, iSize, PACKET_USER_POSITION);
			m_iPlayState = userdata.direction;
		}

		//PACKET_STOP_POSITION
		if (m_iPlayState != 99 && userdata.direction <= 0)
		{
			TPACKET_USER_POSITION userdata;
			memcpy(buffer, &userdata, iSize);
			m_Client.SendMsg(buffer, iSize, PACKET_STOP_POSITION);
			m_iPlayState = 99;
		}
		I_GameUser.Frame();
	}
	for (int iObj = 0; iObj < I_GameUser.m_UserList.size(); iObj++)
	{
		I_GameUser.m_UserList[iObj].Frame();
	}
	return true;
}
bool Sample::RenderIngame()
{
	m_Map.Render();
	I_ScriptManager.Render();

	if (m_Client.IsVerificationZone == false) return true;

	I_GameUser.Render();
	for (int iObj = 0; iObj < I_GameUser.m_UserList.size(); iObj++)
	{
		I_GameUser.m_UserList[iObj].Render();
	}
	return true;
}
bool Sample::Init()
{	
	m_hLogFont = CreateFont(15, 0, 0, 0, 0, 0, 0, 0, HANGEUL_CHARSET, 0, 0, 0, 0, L"궁서");
	RECT rt;
	rt.left = 0;rt.top = 0;	rt.right = m_rtClient.right;	rt.bottom = m_rtClient.bottom;
	m_Map.Load(_T("../../data/ga.bmp"));
	m_Map.SetSourceRect(400, 300, rt);
	I_DemoObjMgr.Init();

	I_DebugStr.Init();
	m_Client.Init();
	I_GameUser.Init();
	m_Udp.Init();


	m_fnExecuteScene[0] = &Sample::FrameLogin;
	m_fnExecuteScene[1] = &Sample::FrameLobby;
	m_fnExecuteScene[2] = &Sample::FrameIngame;
	m_fnExecuteScene[3] = &Sample::RenderLogin;
	m_fnExecuteScene[4] = &Sample::RenderLobby;
	m_fnExecuteScene[5] = &Sample::RenderIngame;
	return true;
}
bool Sample::Render()
{
	FunctionIterator calliter = m_fnExecuteScene.find(5);
	if (calliter != m_fnExecuteScene.end())
	{
		CallFuction call = calliter->second;
		if ((this->*call)() == false)
		{
			return false;
		}
	}
	return true;
}
bool Sample::PostRender()
{	
	SetBkMode(g_hOffScreenDC, TRANSPARENT );	
	HFONT oldfont = (HFONT)SelectObject(g_hOffScreenDC, m_hLogFont);
	TCHAR  strBuffer[256] = {0,};	
	_stprintf( strBuffer, _T(" PosX:%d, PosY:%d : %d [%d]"), m_CursorPos.x,m_CursorPos.y,m_iPlayState,I_GameUser.m_iDirection);
	TextOut(g_hOffScreenDC, 30, 50, strBuffer, lstrlen( strBuffer ));
	// Msg
	EnterCriticalSection(&I_DebugStr.m_CS);	
	std::list<T_STR>::iterator	iter;
	int iHeightStep = 0;
	for( iter =  I_DebugStr.m_Msg.begin();iter != I_DebugStr.m_Msg.end();iter++ )
	{				
		ZeroMemory( strBuffer, sizeof(strBuffer));
		T_STR pMsg = (T_STR)(*iter);
		_stprintf( strBuffer, _T("%s"), pMsg.c_str() );
		TextOut(g_hOffScreenDC, 0, 500+iHeightStep, strBuffer, pMsg.size() );
		iHeightStep+=15;			
	}
	LeaveCriticalSection(&I_DebugStr.m_CS);
	SelectObject(g_hOffScreenDC, oldfont);	

	TCore::PostRender();
	return true;
}
bool Sample::Release()
{
	m_Map.Release();
	DeleteObject(m_hLogFont);
	I_DebugStr.Release();
	m_Client.m_bExit = true;
	I_GameUser.Release();
	I_DemoObjMgr.Release();
	m_Client.Release();
	m_Udp.Release();
	return true;
}
bool Sample::Frame()
{		
	FunctionIterator calliter = m_fnExecuteScene.find(2);
	if (calliter != m_fnExecuteScene.end())
	{
		CallFuction call = calliter->second;
		if ((this->*call)() == false)
		{
			return false;
		}
	}	
	return true;
}

Sample::Sample(void)
{
	m_iPlayState = -1;
	m_iSerIndex		= 0;
}

Sample::~Sample(void)
{
}

TCORE_RUN(TClientDemo, 800, 600);