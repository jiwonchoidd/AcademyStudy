#pragma once
#include "igamestate.h"

class GS_Clear :
	public IGameState
{
public:
	GS_Clear(void);
	~GS_Clear(void);
	void Init();
	GameState ChangeGameState();
	void Update();
	void Render(HDC hdc);
};
