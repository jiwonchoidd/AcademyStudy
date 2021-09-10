#pragma once
#include "igamestate.h"
#include "Resource.h"
#include <windows.h>
#include "GameEngine.h"


class GS_Intro :
	public IGameState
{
private:
	HBITMAP		_hLeft;
	HBITMAP		_hRight;
	HBITMAP		_hUpDown;
	HBITMAP		_hStand;
	HBITMAP		_hLeftJump;
	HBITMAP		_hRightJump;
	HBITMAP		_hDrop;
	HBITMAP		_hDie;
	HBITMAP		_hSurprise;

	HBITMAP		_hLets;
	int _iAni;

	GameState flag;
public:
	GS_Intro(void);
	~GS_Intro(void);
	void Init();
	GameState ChangeGameState();
	void Update();
	void Render(HDC hdc);
};
