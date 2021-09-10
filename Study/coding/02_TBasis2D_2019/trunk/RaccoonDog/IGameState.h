#pragma once
#include <windows.h>
#include "DrawBitmap.h"

typedef enum{GS_WAIT,GS_INTRO,GS_PLAY,GS_CLEAR,GS_GAMEOVER}GameState; 

class IGameState
{
public:

	IGameState(void)
	{
	}

	~IGameState(void)
	{
	}
	virtual void Init()
	{

	}
	virtual void Update()
	{

	}
	virtual GameState ChangeGameState()
	{
		return GS_WAIT;
	}
	virtual void Render(HDC hdc)
	{

	}
	
};






































































#define LEFT		VK_LEFT
#define RIGHT		VK_RIGHT
#define UP			VK_UP
#define DOWN		VK_DOWN
#define	JUMP		VK_SPACE