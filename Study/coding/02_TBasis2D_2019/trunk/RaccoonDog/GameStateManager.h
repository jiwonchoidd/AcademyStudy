#pragma once
#include "GameEngine.h"
#include "GS_Intro.h"
#include "GS_Play.h"

class GameStateManager
{
private:
	HWND			m_hWnd;
	HDC				m_hMemDC;
	HBITMAP			m_hBit;
	HBITMAP			m_hOldBit;
	RECT			m_rClient;

	IGameState * GS;

	GameStateManager(void);
	~GameStateManager(void);
public:
	void ChangeGameState(GameState _GS)
	{
		if(_GS==GS_WAIT)
			return ;
		if(GS){
			delete GS;
			GS=NULL;
		}
		switch(_GS)
		{
		case GS_INTRO:
			GS=new GS_Intro();
			break;
		case GS_PLAY:
			GS=new GS_Play();
			break;
		};
		GS->Init();
	}
	void GameEnd(){
		if(GS)
			delete GS;
	}
	IGameState * GetGameState(){
		return GS;
	}
	void Render()
	{
		m_hWnd=GameEngine::GetEngine()->GetWindow();
		HDC hDC;
		HDC	hMemDC;
		GetClientRect(m_hWnd, &m_rClient);
		hDC = GetDC(m_hWnd);
		m_hBit = CreateCompatibleBitmap(hDC, m_rClient.right, m_rClient.bottom);
		m_hMemDC = CreateCompatibleDC(hDC);
		m_hOldBit = (HBITMAP)SelectObject(m_hMemDC, m_hBit);
		ReleaseDC(m_hWnd, hDC);
		hDC = GetDC(m_hWnd);
		hMemDC = CreateCompatibleDC(hDC);


		GS->Render(m_hMemDC);

		
		DeleteDC(hMemDC);
		ReleaseDC(m_hWnd, hDC);
		hDC = GetDC(m_hWnd);
		BitBlt(hDC, 0, 0, m_rClient.right, m_rClient.bottom, m_hMemDC, 0, 0, SRCCOPY);

		SelectObject(m_hMemDC, m_hOldBit);
		DeleteObject(m_hBit);
		DeleteDC(m_hMemDC);
		ReleaseDC(m_hWnd, hDC);
		InvalidateRect(m_hWnd, NULL, FALSE);
		
	}
	static GameStateManager * Instance(){
		static GameStateManager _Instance;
		return &_Instance;
	}
};
