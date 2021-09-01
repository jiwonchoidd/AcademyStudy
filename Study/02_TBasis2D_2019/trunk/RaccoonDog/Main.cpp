#include "GameEngine.h"
#include "GameStateManager.h"


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,PSTR szCmdLine, int iCmdShow){
	MSG         msg;
	static int  iTickTrigger = 0;
	int         iTickCount;

	
	GameEngine *_pGame = new GameEngine(hInstance, TEXT("呈备府"), TEXT("呈备府"),IDI_RACCOON, IDI_RACCOON, 900, 670);
	
	GS_Intro Intro;
	GameStateManager::Instance()->ChangeGameState(GS_INTRO);
	_pGame->SetFrameRate(60);
	if (!GameEngine::GetEngine()->Initialize(iCmdShow))
		return FALSE;
	while (1){
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)){
			if (msg.message == WM_QUIT)
				break;
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else{
				iTickCount = GetTickCount();		  
				if (iTickCount > iTickTrigger){
					iTickTrigger = iTickCount +	GameEngine::GetEngine()->GetFrameDelay();
					GameStateManager::Instance()->Render();		
					GameStateManager::Instance()->GetGameState()->Update();	

					GameStateManager::Instance()->ChangeGameState(GameStateManager::Instance()->GetGameState()->ChangeGameState());
			}
		}
	}
	GameStateManager::Instance()->GameEnd();
	delete _pGame;
	return (int)msg.wParam;
}