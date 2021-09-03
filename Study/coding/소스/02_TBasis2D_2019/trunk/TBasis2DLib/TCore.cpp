#include "TCore.h"
#include "TObjectMgr.h"

#include <fcntl.h> 
#include <io.h>

bool TCore::GameInit() 
{
	m_bGameRun = true;
	g_Timer.Init();
	g_Input.Init();
	m_Graphic.Init();
	g_Write.Init();
	g_SoundMgr.Init();
	Init();
	return true;
}
bool TCore::GameRelease()
{	
	Release();
	g_Timer.Release();
	g_Input.Release();
	g_Write.Release();
	g_SoundMgr.Release();
	g_ObjectMgr.Release();
	m_Graphic.Release();	
	return true;
}
bool	TCore::GameFrame()
{
	PreFrame();
	g_Timer.Frame();	
	g_Input.Frame();
	m_Graphic.Frame();
	g_SoundMgr.Frame();
		Frame();
	g_Write.Frame();
	g_ObjectMgr.Frame();
	PostFrame();
	return true;
}
bool	TCore::PreRender()
{
	m_Graphic.PreRender();
	return true;
}
bool	TCore::PostRender()
{
	g_Timer.Render();
	g_Input.Render();
	g_SoundMgr.Render();
	g_Write.Render();	
	g_Write.Draw(0, 0, g_Timer.m_szBuffer);
	m_Graphic.PostRender();
	return true;
}
bool	TCore::GameRender()
{
	if (PreRender() == false) return false;
	if (Render() == false) return false;
	if (PostRender() == false) return false;
	return true;
}
bool	TCore::GameRun()
{
	if (GameFrame()==false) return false;
	if (GameRender() == false) return false;
	return true;
}
bool TCore::Run()
{
	//HeapEnableTerminateOnCorruption 옵션을 설정하면 손상된 힙을 이용하는 보안 악용에 대한 애플리케이션의 노출을 줄일 수 있으므로 강력하게 권장.
	//HeapSetInformation(NULL, HeapEnableTerminationOnCorruption, NULL, 0);

	HRESULT hr = S_OK;
	//CoInitializeEx 는 COM 라이브러리를 사용하는 각 스레드에 대해 한 번 이상 호출해야  한다.
	hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);
	GameInit();
	while (m_bGameRun)
	{
		if (MsgProcess() == false)
		{
			m_bGameRun = false;
		}
		else
		{
			if (GameRun()==false)
			{
				break;
			}
		}
	}
	GameRelease();
	CoUninitialize();
	return true;
}
TCore::TCore()
{

}
TCore::~TCore()
{
	::FreeConsole();
}
bool TCore::OpenDebugConsole()
{	
	_tsetlocale(LC_ALL, _T("Korean"));  
	ios::sync_with_stdio();

	if (AllocConsole() == TRUE)
	{
		FILE*  pConsoleStream = nullptr;
		if (freopen_s(&pConsoleStream, "conin$", "rt", stdin))
		{
			return false;
		}
		if (freopen_s(&pConsoleStream, "conout$", "wt", stdout))
		{
			return false;
		}
		if (freopen_s(&pConsoleStream, "conout$", "wt", stderr))
		{
			return false;
		}
	}
	return true;
}
