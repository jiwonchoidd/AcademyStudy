//-----------------------------------------------------------------
// Game Engine Object
// C++ Source - GameEngine.cpp
//-----------------------------------------------------------------

//-----------------------------------------------------------------
// Include Files
//-----------------------------------------------------------------
#include "GameEngine.h"

//-----------------------------------------------------------------
// Static Variable Initialization
//-----------------------------------------------------------------
GameEngine *GameEngine::m_pGameEngine = NULL;

//-----------------------------------------------------------------
// Windows Functions
//-----------------------------------------------------------------


LRESULT CALLBACK WndProc(HWND hWindow, UINT msg, WPARAM wParam, LPARAM lParam)
{
  // Route all Windows messages to the game engine
  return GameEngine::GetEngine()->HandleEvent(hWindow, msg, wParam, lParam);
}

//-----------------------------------------------------------------
// GameEngine Constructor(s)/Destructor
//-----------------------------------------------------------------
GameEngine::GameEngine(HINSTANCE hInstance, LPTSTR szWindowClass,
  LPTSTR szTitle, WORD wIcon, WORD wSmallIcon, int iWidth, int iHeight)
{
  // Set the member variables for the game engine
  m_pGameEngine = this;
  m_hInstance = hInstance;
  m_hWindow = NULL;
  if (lstrlen(szWindowClass) > 0)
    lstrcpy(m_szWindowClass, szWindowClass);
  if (lstrlen(szTitle) > 0)
    lstrcpy(m_szTitle, szTitle);
  m_wIcon = wIcon;
  m_wSmallIcon = wSmallIcon;
  m_iWidth = iWidth;
  m_iHeight = iHeight;
  m_iFrameDelay = 50;   // 20 FPS default
  m_bSleep = TRUE;
}

GameEngine::~GameEngine()
{
}

//-----------------------------------------------------------------
// Game Engine General Methods
//-----------------------------------------------------------------
BOOL GameEngine::Initialize(int iCmdShow)
{
  WNDCLASSEX    wndclass;

  // Create the window class for the main window
  wndclass.cbSize         = sizeof(wndclass);
  wndclass.style          = CS_HREDRAW | CS_VREDRAW;
  wndclass.lpfnWndProc    = WndProc;
  wndclass.cbClsExtra     = 0;
  wndclass.cbWndExtra     = 0;
  wndclass.hInstance      = m_hInstance;
  wndclass.hIcon          = LoadIcon(m_hInstance,
    MAKEINTRESOURCE(GetIcon()));
  wndclass.hIconSm        = LoadIcon(m_hInstance,
    MAKEINTRESOURCE(GetSmallIcon()));
  wndclass.hCursor        = LoadCursor(NULL, IDC_ARROW);
  wndclass.hbrBackground  = (HBRUSH)(COLOR_WINDOW + 1);
  wndclass.lpszMenuName   = NULL;
  wndclass.lpszClassName  = m_szWindowClass;

  if (!RegisterClassEx(&wndclass))
    return FALSE;

  int iWindowWidth = m_iWidth + GetSystemMetrics(SM_CXFIXEDFRAME) * 2,
      iWindowHeight = m_iHeight + GetSystemMetrics(SM_CYFIXEDFRAME) * 2 +
        GetSystemMetrics(SM_CYCAPTION);
  if (wndclass.lpszMenuName != NULL)
    iWindowHeight += GetSystemMetrics(SM_CYMENU);
  int iXWindowPos = (GetSystemMetrics(SM_CXSCREEN) - iWindowWidth) / 2,
      iYWindowPos = (GetSystemMetrics(SM_CYSCREEN) - iWindowHeight) / 2;
  
  m_hWindow = CreateWindow(m_szWindowClass, m_szTitle, WS_POPUPWINDOW |
    WS_CAPTION | WS_MINIMIZEBOX , iXWindowPos, iYWindowPos, iWindowWidth,
    iWindowHeight, NULL, NULL, m_hInstance, NULL);  


  if (!m_hWindow)
    return FALSE;
  ShowWindow(m_hWindow, iCmdShow);
  UpdateWindow(m_hWindow);

  return TRUE;
}

LRESULT GameEngine::HandleEvent(HWND hWindow, UINT msg, WPARAM wParam, LPARAM lParam)
{
  switch (msg)
  {
    case WM_CREATE:
      SetWindow(hWindow);
      return 0;
    case WM_DESTROY:
      PostQuitMessage(0);
      return 0;
  }
  return DefWindowProc(hWindow, msg, wParam, lParam);
}

void GameEngine::ErrorQuit(LPTSTR szErrorMsg)
{
  MessageBox(GetWindow(), szErrorMsg, TEXT("Critical Error"), MB_OK | MB_ICONERROR);
  PostQuitMessage(0);
}
