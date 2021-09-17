#include "GCore.h"
bool GCore::GameRun()
{
	if (!GameFrame())return false;
	if (!GameRender())return false;
	return true;
}
bool GCore::GameInit()
{
	Init();
	return true;
}
bool GCore::GameFrame()
{
	Frame();
	return true;
}
bool GCore::GameRender()
{
	Render();
	return true;
}
bool GCore::GameRelease()
{
	Release();
	return true;
}

bool GCore::Init()
{
	return true;
}
bool GCore::Frame()
{
	return true;
}bool GCore::Render()
{
	return true;
}bool GCore::Release()
{
	return true;
} 