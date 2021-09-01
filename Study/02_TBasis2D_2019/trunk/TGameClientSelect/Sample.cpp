#include "Sample.h"
#include "TObjectMgr.h"
// ftp://112.216.123.139 id:kgcauser ps:kgca!@34

Sample*  g_pSample = nullptr;

LRESULT	 Sample::MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{	
	int  iID = LOWORD(wParam);
	if (TScene::m_pCurrentScene != nullptr)
	{
		m_Network.MsgProc(hWnd, msg, wParam, lParam);
		int iRet = TScene::m_pCurrentScene->MsgProc(hWnd, msg, wParam, lParam);
		if (iRet >= 0)
		{
			return iRet;
		}
	}
	switch (msg)
	{		
		case WM_LBUTTONDOWN:
		{
			if (TCollision::RectInPt(g_rtClient, g_Input.GetPos()))
			{
				if (!TCollision::RectInPt(g_rtEdit, g_Input.GetPos()))
				{
					::SetFocus(g_hWnd);
					m_bEditFocus = false;
					g_Input.SetEnable();
				}
			}
			return 0;
		}break;
		case WM_COMMAND:
		{
			HWND hWndCtrl	= (HWND)lParam;
			int  iID		= LOWORD(wParam);
			int  iNotify	= HIWORD(wParam);
			switch (iID)
			{
				case 100:
				{
					switch (iNotify)
					{
					case EN_CHANGE:
					{
						//GetWindowText(g_hEdit, str, 128);
						SetWindowText(g_hWnd, L"EN_CHANGE");		
						GetWindowText(g_hEdit, m_szChatBuffer, MAX_PATH);						
					}break;
					case EN_SETFOCUS:
					{
						m_bEditFocus = true;
						g_Input.SetEnable(false);
						//GetWindowText(g_hEdit, str, 128);
						SetWindowText(g_hWnd, L"EN_SETFOCUS");						
					}break;
					case EN_KILLFOCUS:
					{
						m_bEditFocus = false;
						g_Input.SetEnable();
						SetWindowText(g_hWnd, L"EN_KILLFOCUS100");						
					}break;
					}
				}break;
				case 200:
				{
					switch (iNotify)
					{
					case BN_SETFOCUS:
					{
						m_bEditFocus = true;
						g_Input.SetEnable(false);
						SetWindowText(g_hWnd, L"BN_SETFOCUS200");
					}break;
					case BN_CLICKED:
					{
						m_bEditFocus = false;
						g_Input.SetEnable();
						std::wstring chat = g_pSample->m_szChatBuffer;
						if (!chat.empty())
						{
							g_pSample->SendTest(to_wm(chat).c_str());
						}
						SetWindowText(g_hWnd, L"BN_CLICKED200");
						SendMessage(g_hEdit, EM_SETSEL, 0, -1);
						SendMessage(g_hEdit, EM_REPLACESEL, FALSE, (LPARAM)L"");						
					}break;
					case BN_KILLFOCUS:
					{
						m_bEditFocus = false;
						g_Input.SetEnable();
						SetWindowText(g_hWnd, L"BN_KILLFOCUS200");
					}break;
					}
				}break;
				case 1100:
				{
					switch (iNotify)
					{
					case EN_CHANGE:
					{
						SetWindowText(hWnd, L"aaaaa");
					}break;
					}
				}
			}	
			//return 0;
		}break;
	}
	return -1;// m_Network.MsgProc(hWnd, msg, wParam, lParam);
}

Sample::Sample()
{
	g_pSample = this;
}
Sample::~Sample()
{	
}

bool  Sample::Init()
{
	OpenDebugConsole();

	WNDCLASS WndClass;
	GetClassInfo(NULL, L"edit", &WndClass);
	WndClass.style = CS_OWNDC;
	WndClass.lpszClassName = L"kgcaEdit";
	RegisterClass(&WndClass);
	GetClassInfo(NULL, L"button", &WndClass);
	WndClass.style = CS_OWNDC;
	WndClass.lpszClassName = L"kgcaButton";
	RegisterClass(&WndClass);

	g_ObjectMgr.Load(L"../../data/script/rtSprite.txt");
	g_ObjectMgr.Load(L"../../data/script/object.txt");

	g_rtEdit = {0, g_rtClient.bottom-30, g_rtClient.right-100, 30};
	g_rtButton = { g_rtEdit.right+10, g_rtEdit.top, 50, 30 };
	m_EditImage.Load(L"../../data/bitmap/LoginEditBoxBack.bmp");
	m_ButtonImage.Load(L"../../data/sand.bmp");	

	DWORD style = WS_CHILD | WS_VISIBLE;
	g_hEdit = CreateWindow(L"kgcaEdit",NULL, style, 
		g_rtEdit.left, g_rtEdit.top, g_rtEdit.right, g_rtEdit.bottom, 
		g_hWnd, (HMENU)100, g_hInstance, NULL);
	SendMessage(g_hEdit, WM_SETFONT, (WPARAM)g_Write.m_hFont, 0);

	style = WS_CHILD | WS_VISIBLE | BS_NOTIFY | BS_OWNERDRAW;
	g_hButton = CreateWindow(L"kgcaButton", L"Send", style,
		g_rtButton.left, g_rtButton.top, g_rtButton.right, g_rtButton.bottom,
		g_hWnd, (HMENU)200, g_hInstance, NULL);

	//g_Fsm.AddStateTrasition(STATE_STAND, EVENT_TIMEMOVE, STATE_MOVE);
	//g_Fsm.AddStateTrasition(STATE_STAND, EVENT_POINTMOVE, STATE_MOVE);
	//g_Fsm.AddStateTrasition(STATE_STAND, EVENT_FINDTARGET, STATE_ATTACK);
	//g_Fsm.AddStateTrasition(STATE_MOVE, EVENT_STOPMOVE, STATE_STAND);
	//g_Fsm.AddStateTrasition(STATE_MOVE, EVENT_FINDTARGET, STATE_ATTACK);
	//g_Fsm.AddStateTrasition(STATE_ATTACK, EVENT_LOSTTARGET, STATE_STAND);

	m_iCurrentScene = 0;
	m_SceneList.push_back(new TSceneIntro);
	m_SceneList.push_back(new TSceneLobby);
	m_SceneList.push_back(new TSceneInGame);
	m_SceneList[0]->Load(L"Loading.txt");
	m_SceneList[1]->Load(L"Lobby.txt");
	m_SceneList[2]->Load(L"InGame.txt");

	m_SceneList[0]->CreateScene();
	TScene::m_pCurrentScene = m_SceneList[0];
	return NetInit();
}
bool  Sample::PreFrame()
{
	if (NetFrame() == false) return false;
	return true;
}
bool  Sample::Frame()
{
	if (g_bActive)
	{
		if (!m_bEditFocus && g_Input.GetKey(VK_LBUTTON) == KEY_PUSH)
		{
			if (TScene::m_pCurrentScene->m_iSceneID == TScene::TSCENE_ZONE)
			{
				SendAttack();
			}
		}
	}	
	TScene::m_pCurrentScene->Frame();	
	return true;
}
bool  Sample::Release()
{
	if (NetRelease() == false) return false;
	for (TScene* scene : m_SceneList)
	{
		scene->Release();
	}
	for (auto user : m_userlist)
	{
		TGameOtherPlayer* pUser = user.second;
		pUser->Release();
		delete pUser;
	}
	m_userlist.clear();

	m_EditImage.Release();
	m_ButtonImage.Release();

	return true;
}
bool  Sample::Render()
{
	TScene::m_pCurrentScene->Render();
	for (auto user : m_userlist)
	{
		TGameOtherPlayer* pUser = user.second;
		pUser->Frame();
		pUser->Render();
	}

	RECT rtSrc = { 0, 0, m_EditImage.m_BitmapInfo.bmWidth, m_EditImage.m_BitmapInfo.bmHeight };
	m_EditImage.Draw(g_rtEdit, rtSrc, SRCCOPY);
	rtSrc = { 0, 0, m_ButtonImage.m_BitmapInfo.bmWidth, m_ButtonImage.m_BitmapInfo.bmHeight };
	m_ButtonImage.Draw(g_rtButton, rtSrc, SRCCOPY);

	/*if (m_bEditFocus)
	{
		SendMessage(g_hEdit, WM_GETTEXT, MAX_PATH, (LPARAM)m_szChatBuffer);		
	}*/
	g_Write.Draw({ g_rtEdit.left, g_rtEdit.top }, m_szChatBuffer);


	return true;
}
bool	Sample::PostRender()
{
	g_Timer.Render();
	g_Input.Render();
	g_SoundMgr.Render();
	g_Write.Render();
	g_Write.Draw(0, 0, g_Timer.m_szBuffer);
	m_Graphic.PostRender();

	if (TScene::m_pCurrentScene->m_bSceneChange == true)
	{
		//TScene::m_pCurrentScene->Reset();
		TScene::m_pCurrentScene = m_SceneList[TScene::m_pCurrentScene->m_iNextScene];
		TScene::m_pCurrentScene->CreateScene();
	}
	return NetRender();
}

bool Sample::AddUser(TPACKET_CHARACTER_INFO& info)
{
	TGameOtherPlayer* pObj = new TGameOtherPlayer;
	pObj->m_Info = info;
	pObj->Init();
	TObjAttribute att = m_SceneList[2]->m_ObjAttribute[1];

	bool bRet = false;
	if (att.bColorKey == false)
	{
		bRet = pObj->Load(att.colorbitmap.c_str(),att.maskbitmap.c_str());
	}
	else
	{
		bRet = pObj->Load(att.colorbitmap.c_str(),nullptr,	att.dwColor);
	}
	if (bRet == true)
	{
		TPoint p = { info.movement.p[0], info.movement.p[1] };
		pObj->Set(p, att.rtSrc);
		pObj->m_ptDirection = { info.movement.d[0], info.movement.d[1] };
		pObj->SetPos(p);
		pObj->m_bCollisionEnabled = true;
		m_userlist.insert(make_pair(info.movement.index, pObj));
	}
	else
	{
		delete pObj;
	}
	return true;
}
bool Sample::AddNpc(TPACKET_CHARACTER_INFO& info)
{
	TPoint p = { info.movement.p[0], info.movement.p[1] };	
	TPoint d = { info.movement.d[0], info.movement.d[1] };
	std::wstring szName = L"npc";
	szName += to_wstring(info.movement.index);
	TScene::m_pCurrentScene->AddNpc(szName, p, d, info.movement.index);
	return true;
}
//TCORE_RUN(L"TGameClientsSelect1", 800, 600);
TGAME_RUN;