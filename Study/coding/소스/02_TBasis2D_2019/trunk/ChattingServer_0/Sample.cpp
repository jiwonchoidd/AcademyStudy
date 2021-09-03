#include "Sample.h"
#include "TStd.h"

bool Sample::Init()
{	
	WSADATA wsadata;
	WSAStartup(MAKEWORD(2,2),&wsadata);
   
	m_Acceptor.Set( 10000, "127.0.0.1" );
	return true;
}
bool Sample::PreRender()
{	
	return true;
}
bool Sample::Render()
{
	return PostRender();
}
bool Sample::PostRender()
{	
	return true;
}
bool Sample::Release()
{
	m_Acceptor.Release();
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
	sWin.SetWindow(hInst, L"Chatting Server",800, 600);
	sWin.Run();
	return 0;
}