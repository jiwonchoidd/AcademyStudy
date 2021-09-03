#include "Sample.h"
#include "TBasisSys.h"
#include "TDebugString.h"

bool Sample::Init()
{	
	I_Server.Init();	
	I_DebugStr.Init();
	return true;
}
bool Sample::Frame()
{
	I_DebugStr.Frame();
	return true;
}
bool Sample::Release()
{
	I_Server.Release();
	I_DebugStr.Release();
	return true;
}
Sample::Sample(void)
{
}

Sample::~Sample(void)
{
}
INT WINAPI wWinMain(   HINSTANCE hInst, 
								HINSTANCE hPreInst, 
								LPWSTR strCmdLine, 
								INT nShow )
{
	Sample sWin;
	sWin.SetWindow(hInst, 800, 600, L"SampleOverlapped_0");
	return 0;
}