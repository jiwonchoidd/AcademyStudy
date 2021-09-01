#include "TCore.h"

bool TCore::Init()
{
	return true;
};
// 계산
bool TCore::Frame()
{
	return true;
};
// 드로우, 랜더링
bool TCore::Render()
{
	return true;
};
// 삭제
bool TCore::Release()
{
	return true;
};

bool TCore::GameInit()
{	
	I_Input->Init();
	I_Timer.Init();
	return Init();
};
bool TCore::GameFrame()
{
	I_Timer.Frame();
	I_Input->Frame();
	return Frame();
};
bool TCore::GameRender()
{
	PreRender();
	Render();
	I_Timer.Render();
	I_Input->Render();
	return PostRender();
};
bool TCore::PreRender()
{
	return true;
}
bool TCore::PostRender()
{
	HDC hdc = GetDC( m_hWnd );
	//SetBkMode( hdc, TRANSPARENT );
	//SetTextColor( hdc, RGB(255,0,0));
	TextOut( hdc, 0, 0, I_Timer.m_strBuffer,
		_tcslen(I_Timer.m_strBuffer));
	ReleaseDC( m_hWnd, hdc );
	return true;
}
bool TCore::GameRelease()
{
	I_Timer.Release();
	I_Input->Release();
	return Release();
};
bool TCore::Run()
{
	GameInit();

	MSG msg;	
	ZeroMemory( &msg, sizeof( msg ));	
	while( msg.message != WM_QUIT )
	{
		if( PeekMessage(&msg, NULL, 0, 0, PM_REMOVE ))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			// 강제적 프레임 고정
			Sleep(1); 
			// 1 프레임 최소 1초당 30프레임
			if( GameFrame() == false ) break;
			if( GameRender() == false ) break;
		}
	}
	GameRelease();
	return true;
};
TCore::TCore(void)
{
}


TCore::~TCore(void)
{
}
