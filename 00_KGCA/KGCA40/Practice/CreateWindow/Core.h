#pragma once
#include "Window.h"
class Core : public Window
{
	//Virtual를 쓰면 override 재사용가능
	//하지만 코어에서는 재사용안함
public:
	bool	GameInit();
	bool	GameFrame();
	bool	GameRender();
	bool	GameRun();
	bool	GameRelease();
public:

};

