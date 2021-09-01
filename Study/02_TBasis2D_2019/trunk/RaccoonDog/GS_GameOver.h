#pragma once
#include "igamestate.h"

class GS_GameOver :
	public IGameState
{
public:
	GS_GameOver(void);
	~GS_GameOver(void);
	void Init();
	GameState ChangeGameState();
	void Update();
	void Render(HDC hdc);
};
