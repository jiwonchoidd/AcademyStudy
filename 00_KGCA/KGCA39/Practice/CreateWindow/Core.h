#pragma once
#include "Window.h"
class Core : public Window
{
	//Virtual�� ���� override ���밡��
	//������ �ھ���� �������
public:
	bool	GameInit();
	bool	GameFrame();
	bool	GameRender();
	bool	GameRun();
	bool	GameRelease();
public:

};

