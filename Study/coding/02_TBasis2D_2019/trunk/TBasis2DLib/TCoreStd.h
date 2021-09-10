#pragma once
#include "TStd.h"
#define TGAME_STRAT int APIENTRY wWinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance,	LPWSTR    lpCmdLine,int       nCmdShow){Sample demo;
#define TGAME_LOOP	if (demo.SetWindow(hInstance)){demo.Run();}return 1;}
#define TGAME_RUN TGAME_STRAT;TGAME_LOOP;

#define TCORE_START int WINAPI wWinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance,LPWSTR    lpCmdLine,int       nCmdShow){Sample sample;
#define TCORE_WIN(s,x,y) if (sample.SetWindow(hInstance, L#s, x, y))	{	sample.Run();	} }
#define TCORE_RUN(s,x,y) TCORE_START;TCORE_WIN(s,x,y);
#define TCORE_END }