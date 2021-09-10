// TBasis2D.lib + TCoreClient.lib
#include "Sample.h"
#include "TBasisSys.h"
#include "TInput.h"
#include "TSound.h"
#include "TObjectMgr.h"
#include "TDebugString.h"
bool Sample::Init()
{	
	I_ObjectMgr.m_iWidth = m_iWidth;
	I_ObjectMgr.m_iHeight = m_iHeight;
	I_ObjectMgr.m_rtClient = m_rtClient;

	I_DebugStr.Init();
	m_bLogin = true;
	m_Client.Init();
	I_ObjectMgr.Init();

	I_GameUser.Init();

	m_Udp.Init();

	m_iValueW =0;
	m_iValueS = 0;
	m_iValueD =0;
	m_iValueA= 0;
	return true;
}
bool Sample::PreRender()
{	
	RECT rt;
	SetRect(&rt, 0, 0, m_iWidth, m_iHeight );
	FillRect(	I_ObjectMgr.m_hBackScreen.hDC, &rt, (HBRUSH)GetStockObject( WHITE_BRUSH ));
	return true;
}
bool Sample::Render()
{
	if( m_bLogin == false ) return true;
	if( PreRender() )
	{
		I_ObjectMgr.m_ObjMap_A.RenderMap();
		I_GameUser.Render();
		for( int iObj=0; iObj < m_UserList.size(); iObj++ )
		{
			m_UserList[iObj].Render();
		}
	}
	return PostRender();
}
bool Sample::PostRender()
{	
	HFONT font, oldfont;
	SetBkMode( I_ObjectMgr.m_hBackScreen.hDC, TRANSPARENT );
	font = CreateFont( 12,0,0,0,0,0,0,0,HANGEUL_CHARSET, 0,0,0,0,L"±Ã¼­");
	oldfont = (HFONT)SelectObject(I_ObjectMgr.m_hBackScreen.hDC, font);
		
	TextOut( I_ObjectMgr.m_hBackScreen.hDC, 0, 0, m_Time.m_strBuffer, lstrlen( m_Time.m_strBuffer ));

	TCHAR  strBuffer[256] = {0,};	
	_stprintf( strBuffer, _T(" PosX:%d, PosY:%d : %d [%d] [%d] [%d] [%d]"), 
						 m_CursorPos.x, m_CursorPos.y , m_iPlayState, m_iValueW, m_iValueS, m_iValueA, m_iValueD
	);

	TextOut( I_ObjectMgr.m_hBackScreen.hDC, 0, 50, strBuffer, lstrlen( strBuffer ));

	// Msg
	EnterCriticalSection(&I_DebugStr.m_CS);	
	std::list<TCHAR_STRING>::iterator	iter;
	int iHeightStep = 0;
	for( iter =  I_DebugStr.m_Msg.begin();
			iter != I_DebugStr.m_Msg.end();
			iter++ )
		{				
			ZeroMemory( strBuffer, sizeof(strBuffer));
			TCHAR_STRING pMsg = (TCHAR_STRING)(*iter);
			_stprintf( strBuffer, _T("%s"), pMsg.c_str() );
			TextOut( I_ObjectMgr.m_hBackScreen.hDC, 0, 500+iHeightStep, strBuffer, pMsg.size() );
			iHeightStep+=15;			
		}
	LeaveCriticalSection(&I_DebugStr.m_CS);


	SelectObject(I_ObjectMgr.m_hBackScreen.hDC, oldfont);
	DeleteObject(font);

	HDC hScreenDC = GetDC( g_hWnd );
		BitBlt( hScreenDC, 0,0, 800, 600,
			I_ObjectMgr.m_hBackScreen.hDC, 0,0, SRCCOPY );
	ReleaseDC( g_hWnd, hScreenDC );
	return true;
}
bool Sample::Release()
{
	I_DebugStr.Release();
	m_Client.m_bExit = true;
	I_GameUser.Release();
	I_ObjectMgr.Release();
	m_Client.Release();
	m_Udp.Release();
	return true;
}
bool Sample::Frame()
{		
	if( m_bLogin == false ) return true;
	m_Client.Frame();
	GetCursorPos(&m_CursorPos);
	ScreenToClient(g_hWnd, &m_CursorPos);

	if(m_iPlayState != 0 && (I_Input.KeyCheck(VK_LEFT) == KEY_PUSH))
	{
		TPACKET_USER_POSITION userdata;
		userdata.direction = VK_LEFT;
		userdata.posX = I_GameUser.m_fPosX;
		userdata.posY = I_GameUser.m_fPosY;
		userdata.user_idx = m_iSerIndex;
		char buffer[256] = {0,};
		int iSize = sizeof(userdata);
		memcpy( buffer, &userdata, iSize);
		m_Client.SendMsg( buffer, iSize,PACKET_USER_POSITION);
		m_iValueW++;
		m_iPlayState = 0;
	}
	if(m_iPlayState != 1 && (I_Input.KeyCheck(VK_RIGHT) == KEY_PUSH))
	{
		TPACKET_USER_POSITION userdata;
		userdata.direction = VK_RIGHT;
		userdata.posX = I_GameUser.m_fPosX;
		userdata.posY = I_GameUser.m_fPosY;
		userdata.user_idx = m_iSerIndex;
		char buffer[256] = {0,};
		int iSize = sizeof(userdata);
		memcpy( buffer, &userdata, iSize);
		m_Client.SendMsg( buffer, iSize,	PACKET_USER_POSITION);
		m_iValueS++;
		m_iPlayState = 1;
	}
	if(m_iPlayState != 2 && (I_Input.KeyCheck(VK_UP) == KEY_PUSH))
	{
		TPACKET_USER_POSITION userdata;
		userdata.direction = VK_UP;
		userdata.posX = I_GameUser.m_fPosX;
		userdata.posY = I_GameUser.m_fPosY;
		userdata.user_idx = m_iSerIndex;
		char buffer[256] = {0,};
		int iSize = sizeof(userdata);
		memcpy( buffer, &userdata, iSize);
		m_Client.SendMsg( buffer, iSize,PACKET_USER_POSITION);
		m_iValueA++;
		m_iPlayState = 2;
	}
	if(m_iPlayState != 3 && (I_Input.KeyCheck(VK_DOWN) == KEY_PUSH))
	{
		TPACKET_USER_POSITION userdata;
		userdata.direction = VK_DOWN;
		userdata.posX = I_GameUser.m_fPosX;
		userdata.posY = I_GameUser.m_fPosY;
		userdata.user_idx = m_iSerIndex;
		char buffer[256] = {0,};
		int iSize = sizeof(userdata);
		memcpy( buffer, &userdata, iSize);
		m_Client.SendMsg( buffer, iSize,	PACKET_USER_POSITION);
		m_iValueD++;
		m_iPlayState = 3;
	}

	I_GameUser.Frame();
	for( int iObj=0; iObj < m_UserList.size(); iObj++ )
	{
		m_UserList[iObj].Frame();
	}	

	Sleep(30);
	return true;
}

Sample::Sample(void)
{
	m_iPlayState = -1;
	m_bLogin		= false;
	m_iSerIndex		= 0;
}

Sample::~Sample(void)
{
}

INT WINAPI wWinMain(   HINSTANCE hInst, 
								HINSTANCE hPreInst, 
								LPWSTR strCmdLine, 
								INT nShow )
{
	Sample sWin;
	sWin.SetWindow(hInst, 800, 600, L"SampleObject_0");
	return 0;
}