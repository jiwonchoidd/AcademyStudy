#include "Sample.h"


Sample::Sample(void)
{
}


Sample::~Sample(void)
{
}
int WINAPI WinMain( HINSTANCE hInst,
				    HINSTANCE hPrev,
					LPSTR lpszCmdParam,
					int nCmdShow )
{
	Sample g_Sample;
	if( g_Sample.SetWindow( hInst, 
							nCmdShow,
							L"SampleLib_0") )
	{
		g_Sample.Run();
	}
	return 1;
}