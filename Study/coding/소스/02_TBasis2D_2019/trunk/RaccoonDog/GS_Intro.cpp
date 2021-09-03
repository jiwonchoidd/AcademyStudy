#include "GS_Intro.h"

GS_Intro::GS_Intro(void)
{
}
GS_Intro::~GS_Intro(void)
{
	DeleteObject(_hLeft);
	DeleteObject(_hRight);
	DeleteObject(_hUpDown);
	DeleteObject(_hStand);
	DeleteObject(_hLeftJump);
	DeleteObject(_hRightJump);
	DeleteObject(_hDrop);
	DeleteObject(_hDie);
	DeleteObject(_hSurprise);

	DeleteObject(_hLets);

}
void GS_Intro::Init()
{
	HINSTANCE hInstance=GameEngine::GetEngine()->GetInstance();
	_iAni=0;
	flag=GS_WAIT;
	_hLeft = LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_LEFT));
	_hRight = LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_RIGHT));
	_hUpDown = LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_UPDOWN));
	_hStand = LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_STAND));	
	_hLeftJump = LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_LEFT_JUMP));
	_hRightJump = LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_RIGHT_JUMP));
	_hDrop = LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_DROP));
	_hDie = LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_DIE));
	_hSurprise = LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_SURPRISE));

	_hLets = LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_LETS));
}
void GS_Intro::Update()
{
	_iAni++;
	if (_iAni == 171) 	{
		PlaySound(MAKEINTRESOURCE(IDR_INTRO), GameEngine::GetEngine()->GetInstance(), SND_RESOURCE | SND_ASYNC);
	}
	// 인트로음악종료
	if (GetAsyncKeyState(VK_SPACE) < 0) {
		PlaySound(NULL,GameEngine::GetEngine()->GetInstance(), 0);
		flag=GS_PLAY;
		Sleep(100);
	}
}
GameState GS_Intro::ChangeGameState()
{
	return flag;
}
void GS_Intro::Render(HDC hdc)
{
	RECT crt;
	GetClientRect(GameEngine::GetEngine()->GetWindow(), &crt);
	HBRUSH BackBrush = CreateSolidBrush(RGB(0, 0, 0));
	HBRUSH OldBrush = (HBRUSH)SelectObject(hdc, BackBrush);
	if (_iAni == 1 ) 
		FillRect(hdc,&crt,BackBrush);	
	else {
		crt.left = 120;
		crt.top = 430;
		crt.right = 900;
		crt.bottom = 500;
		FillRect(hdc,&crt,BackBrush);	
	}
	static BOOL b;
	if (_iAni <= 171) {
		b = !b;		
		if (_iAni <= 50)		
			DrawBitmap(hdc, 970-_iAni*5, 450, _hLeft, TRUE, b*50, 0, (b+1)*50, 50);	
		else 
			DrawBitmap(hdc, 720, 450, _hLeft, TRUE, b*50, 0, (b+1)*50, 50);	
		if ( _iAni >= 50 && _iAni <= 70) 		
			DrawBitmap(hdc, 970-_iAni*5, 450, _hLeft, TRUE, b*50, 0, (b+1)*50, 50);	
		else if ( _iAni >= 70)
			DrawBitmap(hdc, 620, 450, _hLeft, TRUE, b*50, 0, (b+1)*50, 50);
		if ( _iAni >= 70 && _iAni <= 90)		
			DrawBitmap(hdc, 970-_iAni*5, 450, _hLeft, TRUE, b*50, 0, (b+1)*50, 50);	
		else if ( _iAni >= 90)
			DrawBitmap(hdc, 520, 450, _hLeft, TRUE, b*50, 0, (b+1)*50, 50);	
		if ( _iAni >= 90 && _iAni <= 110) 		
			DrawBitmap(hdc, 970-_iAni*5, 450, _hLeft, TRUE, b*50, 0, (b+1)*50, 50);	
		else if ( _iAni >= 110)
			DrawBitmap(hdc, 420, 450, _hLeft, TRUE, b*50, 0, (b+1)*50, 50);	
		if ( _iAni >= 110 && _iAni <= 130)		
			DrawBitmap(hdc, 970-_iAni*5, 450, _hLeft, TRUE, b*50, 0, (b+1)*50, 50);	
		else if ( _iAni >= 130)
			DrawBitmap(hdc, 320, 450, _hLeft, TRUE, b*50, 0, (b+1)*50, 50);	
		if ( _iAni >= 130 && _iAni <= 150) 		
			DrawBitmap(hdc, 970-_iAni*5, 450, _hLeft, TRUE, b*50, 0, (b+1)*50, 50);	
		else if ( _iAni >= 150)
			DrawBitmap(hdc, 220, 450, _hLeft, TRUE, b*50, 0, (b+1)*50, 50);	
		if ( _iAni >= 150 && _iAni <= 170) 		
			DrawBitmap(hdc, 970-_iAni*5, 450, _hLeft, TRUE, b*50, 0, (b+1)*50, 50);	
		else if ( _iAni >= 170)
			DrawBitmap(hdc, 120, 450, _hLeft, TRUE, b*50, 0, (b+1)*50, 50);	
	} else if (_iAni < 437 ) {
		for (int i = 120; i <= 520; i += 100)
			DrawBitmap(hdc, i, 450, _hStand, TRUE, (_iAni/19%2)*50, 0, (_iAni/19%2+1)*50, 50);
		if (_iAni < 250)
			DrawBitmap(hdc, 620, 450, _hStand, TRUE, (1-_iAni/19%2)*50, 0, (1-_iAni/19%2+1)*50, 50);		
		else 
			DrawBitmap(hdc, 620, 450, _hStand, TRUE, (_iAni/19%2)*50, 0, (_iAni/19%2+1)*50, 50);		
		if (_iAni > 220 && _iAni < 255)
			DrawBitmap(hdc, 640, 435, _hSurprise, TRUE);
		DrawBitmap(hdc, 720, 450, _hStand, TRUE, (_iAni/19%2)*50, 0, (_iAni/19%2+1)*50, 50);

	} else {
		for (int i = 120; i <= 520; i += 100)
			DrawBitmap(hdc, i, 450, _hRight, TRUE, 0, 0, 50, 50);
		DrawBitmap(hdc, 620, 450, _hStand, TRUE, 0, 0, 50, 50);
		DrawBitmap(hdc, 640, 435, _hSurprise, TRUE);
		DrawBitmap(hdc, 720, 450, _hLeft, TRUE, 0, 0, 50, 50);
	}
	if (_iAni >= 171) 	{
		DrawBitmap(hdc, 220, 120, _hLets, TRUE);
	}
	SelectObject(hdc, OldBrush);		
	DeleteObject(BackBrush);
}