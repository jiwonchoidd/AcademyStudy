#include "GS_Play.h"

GS_Play::GS_Play(void)
{
	flag=GS_WAIT;
	// 초기화 
	_iLevel = 1;
	_iLive = LIVE;
	_iScore = 0;
	Init();
	GameInit();
	LoadMap();
	_GameState = 1;
}
GS_Play::~GS_Play(void)
{

}
void GS_Play::Init(){
	static HINSTANCE _hInstance=GameEngine::GetEngine()->GetInstance();
	_hMapEle[0] = LoadBitmap(_hInstance, MAKEINTRESOURCE(IDB_MAP_A));
	_hMapEle[1] = LoadBitmap(_hInstance, MAKEINTRESOURCE(IDB_MAP_B));
	_hMapEle[2] = LoadBitmap(_hInstance, MAKEINTRESOURCE(IDB_MAP_C));
	_hMapEle[3] = LoadBitmap(_hInstance, MAKEINTRESOURCE(IDB_MAP_D));
	_hMapEle[4] = LoadBitmap(_hInstance, MAKEINTRESOURCE(IDB_MAP_E));
	_hMapEle[5] = LoadBitmap(_hInstance, MAKEINTRESOURCE(IDB_MAP_F));			

	// 'LETS..' 
	_hLets = LoadBitmap(_hInstance, MAKEINTRESOURCE(IDB_LETS));
	// 'SCORE'
	_hScore = LoadBitmap(_hInstance, MAKEINTRESOURCE(IDB_SCORE));
	// 'GAME OVER'
	_hGameOver = LoadBitmap(_hInstance, MAKEINTRESOURCE(IDB_GAMEOVER));
	// 'BONUS'
	_hBonus = LoadBitmap(_hInstance, MAKEINTRESOURCE(IDB_BONUS));
	// "NEXT'
	_hNext = LoadBitmap(_hInstance, MAKEINTRESOURCE(IDB_NEXT));
	// ending
	_hEnding = LoadBitmap(_hInstance, MAKEINTRESOURCE(IDB_ENDING));

	// 숫자 
	_hDigit = LoadBitmap(_hInstance, MAKEINTRESOURCE(IDB_DIGIT));
	_hDigit_sm = LoadBitmap(_hInstance, MAKEINTRESOURCE(IDB_DIGIT_SM));

	// 항아리 
	_hPot = LoadBitmap(_hInstance, MAKEINTRESOURCE(IDB_MAP_N));

	// 과일 
	_hFruit[0] = LoadBitmap(_hInstance, MAKEINTRESOURCE(IDB_MAP_Q));
	_hFruit[1] = LoadBitmap(_hInstance, MAKEINTRESOURCE(IDB_MAP_R));
	_hFruit[2] = LoadBitmap(_hInstance, MAKEINTRESOURCE(IDB_MAP_S));
	_hFruit[3] = LoadBitmap(_hInstance, MAKEINTRESOURCE(IDB_MAP_T));

	// 너구리 
	_hLeft = LoadBitmap(_hInstance, MAKEINTRESOURCE(IDB_LEFT));
	_hRight = LoadBitmap(_hInstance, MAKEINTRESOURCE(IDB_RIGHT));
	_hUpDown = LoadBitmap(_hInstance, MAKEINTRESOURCE(IDB_UPDOWN));
	_hStand = LoadBitmap(_hInstance, MAKEINTRESOURCE(IDB_STAND));	
	_hLeftJump = LoadBitmap(_hInstance, MAKEINTRESOURCE(IDB_LEFT_JUMP));
	_hRightJump = LoadBitmap(_hInstance, MAKEINTRESOURCE(IDB_RIGHT_JUMP));
	_hDrop = LoadBitmap(_hInstance, MAKEINTRESOURCE(IDB_DROP));
	_hDie = LoadBitmap(_hInstance, MAKEINTRESOURCE(IDB_DIE));

	// 깜짝 놀란 무늬
	_hSurprise = LoadBitmap(_hInstance, MAKEINTRESOURCE(IDB_SURPRISE));

	// 적 
	_hEnemyLeft = LoadBitmap(_hInstance, MAKEINTRESOURCE(IDB_ENEMY_LEFT));
	_hEnemyRight = LoadBitmap(_hInstance, MAKEINTRESOURCE(IDB_ENEMY_RIGHT));
	_hEnemyLeftRed = LoadBitmap(_hInstance, MAKEINTRESOURCE(IDB_ENEMY_LEFT_RED));
	_hEnemyRightRed = LoadBitmap(_hInstance, MAKEINTRESOURCE(IDB_ENEMY_RIGHT_RED));

	// 뱀 
	_hSnakeLeft = LoadBitmap(_hInstance, MAKEINTRESOURCE(IDB_SNAKE_LEFT));
	_hSnakeRight = LoadBitmap(_hInstance, MAKEINTRESOURCE(IDB_SNAKE_RIGHT));

	// 아기 
	_hBaby = LoadBitmap(_hInstance, MAKEINTRESOURCE(IDB_BABY));
}
void GS_Play::GameInit()
{
	//초기화
	_Rac.Init();
	_iTime = 500;			//게임 제한 시간 
	_iItemScoreRate = 5;	//아이템 점수, 2배씩 곱해지면서 증가 
	_iEat = 0;				//먹은 과일수 
	_iAni = 0;				//애니메이션 효과를 위해 
	_bIsDrop_Sound = FALSE;	//너구리가 떨어질때 나는 소리 상태 

	_iLevel=1;
	_EnemyCount = 0;		//적의 숫자 

	_ScoreShow = 0;			//먹은 과일 점수 표시 시간 
	_JumpFrame = 0;			//점프를 보여 줄때 필요 (카운터)
}
void GS_Play::Update()
{
	_iLevel=1;
	static HINSTANCE _hInstance= GameEngine::GetEngine()->GetInstance();
	switch (_Rac.state) {
		case 1:
			if (GetAsyncKeyState(LEFT) < 0) 
				_Rac.state = 2;		

			else if (GetAsyncKeyState(RIGHT) < 0) 
				_Rac.state = 3;		

			// 'F'는 사다리,  +20 한 것은 너구리의 중심을 맞추기 위해 
			else if (GetAsyncKeyState(UP) < 0) {
				if ( _cMap[(_Rac.y) / 25][(_Rac.x + 20) / 25] == 'F') 
					_Rac.state = 4;
			}			
			else if (GetAsyncKeyState(DOWN) < 0) {
				if ( _cMap[(_Rac.y+50) / 25][(_Rac.x + 20) / 25] == 'F' ) 				
					_Rac.state = 4;			
			}
			else if (GetAsyncKeyState(JUMP) < 0) {
				PlaySound(MAKEINTRESOURCE(IDR_RAC_JUMP), _hInstance, SND_RESOURCE | SND_ASYNC);
				_Rac.state = 5;		
			}

			break;

		case 2:
			if (GetAsyncKeyState(LEFT) < 0) {
				if (_Rac.x >= 25) {					
					_Rac.x -= _Rac.speedx;
					_Rac.step = !_Rac.step;
					if (_Rac.x % 20 == 0)
						PlaySound(MAKEINTRESOURCE(IDR_RAC_STEP), _hInstance, SND_RESOURCE | SND_ASYNC | SND_NOSTOP);
					CheckCollision();	
				} //키 버퍼 
				if (GetAsyncKeyState(JUMP) < 0) {
					PlaySound(MAKEINTRESOURCE(IDR_RAC_JUMP), _hInstance, SND_RESOURCE | SND_ASYNC);
					_Rac.state = 7;
				}
			}
			else if (GetAsyncKeyState(RIGHT) < 0)
				_Rac.state = 3;

			else if (GetAsyncKeyState(UP) < 0) {
				if (_cMap[(_Rac.y -25) / 25][(_Rac.x + 20) / 25] == 'F' ) 				
					_Rac.state = 4;
			}			
			else if (GetAsyncKeyState(DOWN) < 0) {
				if (_cMap[(_Rac.y+50) / 25][(_Rac.x + 20) / 25] == 'F' ) 				
					_Rac.state = 4;
			}			
			else if (GetAsyncKeyState(JUMP) < 0) {
				PlaySound(MAKEINTRESOURCE(IDR_RAC_JUMP), _hInstance, SND_RESOURCE | SND_ASYNC);
				_Rac.state = 6;
			}

			break;

		case 3:
			if (GetAsyncKeyState(LEFT) < 0)  
				_Rac.state = 2;			

			else if (GetAsyncKeyState(RIGHT) < 0) {
				if ( (_Rac.x <= 670 ) || (_Rac.x <= 775 && _Rac.y == 578) ) {
					_Rac.x += _Rac.speedx;
					_Rac.step = !_Rac.step;
					if (_Rac.x % 20 == 0)
						PlaySound(MAKEINTRESOURCE(IDR_RAC_STEP), _hInstance, SND_RESOURCE | SND_ASYNC | SND_NOSTOP);
					CheckCollision();	
				} // 키 버퍼 
				if (GetAsyncKeyState(JUMP) < 0)  {
					PlaySound(MAKEINTRESOURCE(IDR_RAC_JUMP), _hInstance, SND_RESOURCE | SND_ASYNC);
					_Rac.state = 9;
				}
			}
			else if (GetAsyncKeyState(UP) < 0) {
				if ( _cMap[(_Rac.y -25) / 25][(_Rac.x + 20) / 25] == 'F' ) 
					_Rac.state = 4;
			}			
			else if (GetAsyncKeyState(DOWN) < 0 ) {
				if ( _cMap[(_Rac.y+50) / 25][(_Rac.x + 20) / 25] == 'F' )				
					_Rac.state = 4;
			}			
			else if (GetAsyncKeyState(JUMP) < 0) {
				PlaySound(MAKEINTRESOURCE(IDR_RAC_JUMP), _hInstance, SND_RESOURCE | SND_ASYNC);
				_Rac.state = 8;
			}

			break;

		case 4:
			if (GetAsyncKeyState(UP) < 0) 
				if ( _cMap[(_Rac.y + 20) / 25][(_Rac.x+20) / 25] == 'F') {
					_Rac.y -= _Rac.speedy;
					_Rac.step = !_Rac. step;
					if ((_Rac.y-3) % 20 == 0)
						PlaySound(MAKEINTRESOURCE(IDR_RAC_STEP), _hInstance, SND_RESOURCE | SND_ASYNC | SND_NOSTOP);
				} else 
					_Rac.state = 1;		

			else if (GetAsyncKeyState(DOWN) < 0) 
				if ( _cMap[(_Rac.y+50) / 25][(_Rac.x+20) / 25] == 'F') {
					_Rac.y += _Rac.speedy;
					_Rac.step = !_Rac. step;
					if ((_Rac.y-3) % 20 == 0)
						PlaySound(MAKEINTRESOURCE(IDR_RAC_STEP), _hInstance, SND_RESOURCE | SND_ASYNC | SND_NOSTOP);
				} else 
					_Rac.state = 1;			

				break;
	}
}
void GS_Play::Render(HDC hdc)
{


	///
	///
	///
	/// ,여기 정리하다가 중간에 잠.... 내일 일어나서학원가기전에 마져 하자꾸나...
	///
	///
	///
	int i=0;
	static HINSTANCE _hInstance=GameEngine::GetEngine()->GetInstance();
	// 시간 바를 표시할 브러시와 펜 
	HANDLE MyPen = CreatePen(PS_SOLID, 0, RGB(0, 0, 0));
	HANDLE OldPen = (HPEN)SelectObject(hdc, MyPen);

	HANDLE MyBrush = CreateSolidBrush(RGB(25, 184, 5));	
	HANDLE OldBrush = (HBRUSH)SelectObject(hdc, MyBrush);	

	//에니메이션 구현을 위해 	
	//너구리가 죽으면 움직임을 멈추는 것들은 _iTime을 기준으로 에니메이션
	//너구리가 죽으면서 하는 움직임은 _iAni을 이용 	
	_iAni++;

	// Play종료 조건 

	// 과일 다 먹음 
	// _ScoreShow == 1 을 한 이유는 마지막으로 먹은 과일 점수 까지 보여주려고 
	if ( _iEat == 8 && _ScoreShow == 1) {					
		Sleep(500);
		_iAni = 0;			
		_GameState = 2;			
	}	
	// 너구리 죽음 	
	if (_Rac.state == 11) {
		PlaySound(NULL, _hInstance, 0);
		PlaySound(MAKEINTRESOURCE(IDR_RAC_DIE), _hInstance, SND_RESOURCE | SND_ASYNC );
		Sleep(1500);			
		if (_iLive == 0) { // 마지막 너구리가 죽으면 
			GameInit();
			_GameState = 3;	//gameover
		} else {
			_iLive--;
			GameInit();
			LoadMap();
		}
	}


	// 남은 시간	
	// 너구리가 떨어지거나 죽으면 시간은 멈춘다.
	if ( _iAni % 5 == 0 && _Rac.state != 10 && _Rac.state != 11 ) {
		_iTime--;
		if (_iTime == 0) 			
			_Rac.state = 10;	//너구리 죽음 		
	}

	// 맵(처음 시작 할때 맵 전체를 한번 그린다.)
	DrawMap(hdc);


	// 점수 표시 (이전과 변화가 있을 때만 그린다.)
	static int Score;
	DrawDigit(hdc, 25, 50, _iScore, _hDigit, 7);	
	Score = _iScore;

	// 시간 바 표시 
	Rectangle(hdc, 650-_iTime, 25, 650, 50); 	





	// 항아리 & 과일 표시 
	for (i = 0; i < 12; i++) {	
		//항아리 
		if (_Item[i].ch == 'M' || _Item[i].ch == 'N' || _Item[i].ch == 'O') 
			DrawBitmap(hdc, _Item[i].x, _Item[i].y, _hPot, FALSE);			 
		//과일 
		else if (_Item[i].ch >= 'Q') 
			DrawBitmap(hdc, _Item[i].x, _Item[i].y, _hFruit[_iLevel-1], FALSE);
		//너구리가 먹은 것에 대한 점수 표시 
		//너구리가 먹으면 *로 대치하고 점수를 표시한다음에 .로 바꿈
		else if (_Item[i].ch == '*') {			
			DrawDigit(hdc, _Item[i].x, _Item[i].y+25, _iItemScoreRate, _hDigit_sm);

			//점수를 10프레임 동안 보여짐 			
			if ( ++_ScoreShow == 11 ) {				
				DrawBitmap(hdc, _Item[i].x, _Item[i].y+25, _hMap, FALSE, 200, 0, 240, 14);
				_Item[i].ch = '.';
				_ScoreShow = 0;
			}
		} else if (_Item[i].ch == '#') {
			DrawBitmap(hdc, _Item[i].x, _Item[i].y, _hMap, FALSE, 200,0, 250,50);
			_Item[i].ch = '.';
		}
	}

	//적 표시 
	//뱀과 일반적인 적을 따로 그리는 이유는 투명 효과(Alpha) 때문에.	

	for (i = 0; i <_EnemyCount; i++) {

		//뱀
		if (_Ene[i].type == FALSE) {	

			//너구리가 살아있고 (너구리가 죽으면 화면이 멈춰야 되므로)
			//알파 값이 255가 되면 뱀은 움직인다.
			if (_Rac.state != 10 && _Rac.state != 11 && _Ene[i].alpha == 255) _Ene[i].x += _Ene[i].speed;

			if ( _Ene[i].x <= 30) {	//왼쪽 끝
				_Ene[i].x = 30;
				_Ene[i].speed *= -1;
				_Ene[i].state = !_Ene[i].state;
			} else if ( _Ene[i].x >= 670) { //오른쪽 끝
				_Ene[i].x = 670;
				_Ene[i].speed *= -1;
				_Ene[i].state = !_Ene[i].state;
			}

			if (_Ene[i].state == FALSE) //방향(오른쪽을 보고 있을때)
				if (_Ene[i].alpha != 255 ) {
					_Ene[i].alpha += 5;	//선명하게
					DrawBitmap(hdc, _Ene[i].x, _Ene[i].y ,_hMap, FALSE, 200, 0, 250, 50); //검은색으로 기존의 것을 지우기 (200,0 ~ 250,50은 검정색)
					DrawBitmapAlpha(hdc, _Ene[i].x, _Ene[i].y , _hSnakeRight, _Ene[i].alpha, (_iAni/5%2)*50, 0, (_iAni/5%2+1)*50, 50);
				} else
					DrawBitmap(hdc, _Ene[i].x, _Ene[i].y , _hSnakeRight, TRUE, (_iAni/5%2)*50, 0, (_iAni/5%2+1)*50, 50);
			else if (_Ene[i].alpha != 255 ) {
				_Ene[i].alpha += 5;	//선명하게  
				DrawBitmapAlpha(hdc, _Ene[i].x, _Ene[i].y , _hSnakeLeft, _Ene[i].alpha, (_iAni/5%2)*50, 0, (_iAni/5%2+1)*50, 50);
			} else // 방향(왼쪽을 보고 있을때)
				DrawBitmap(hdc, _Ene[i].x, _Ene[i].y , _hSnakeLeft, TRUE, (_iAni/5%2)*50, 0, (_iAni/5%2+1)*50, 50);
		}
	}



	//일반적인 적 
	for (i = 0; i <_EnemyCount; i++) {

		if (_Ene[i].type == TRUE) {

			if (_Rac.state != 10 && _Rac.state != 11) _Ene[i].x += _Ene[i].speed;

			if ( _Ene[i].x <= 30) {
				_Ene[i].x = 30;
				_Ene[i].speed *= -1;
				_Ene[i].state = !_Ene[i].state;
			} else if ( _Ene[i].x >= 670) {
				_Ene[i].x = 670;
				_Ene[i].speed *= -1;
				_Ene[i].state = !_Ene[i].state;
			}

			if (_Ene[i].speed == 4 || _Ene[i].speed == -4) { // 속도가 4인 적은 빨간색, 나머지는 초록색 
				if (_Ene[i].state == FALSE) //방향
					DrawBitmap(hdc, _Ene[i].x, _Ene[i].y , _hEnemyRightRed, TRUE, (_iAni%2)*50, 0, (_iAni%2+1)*50, 50);
				else 
					DrawBitmap(hdc, _Ene[i].x, _Ene[i].y , _hEnemyLeftRed, TRUE, (_iAni%2)*50, 0, (_iAni%2+1)*50, 50);
			} else {
				if (_Ene[i].state == FALSE) //방향
					DrawBitmap(hdc, _Ene[i].x, _Ene[i].y , _hEnemyRight, TRUE, (_iAni%2)*50, 0, (_iAni%2+1)*50, 50);
				else
					DrawBitmap(hdc, _Ene[i].x, _Ene[i].y , _hEnemyLeft, TRUE, (_iAni%2)*50, 0, (_iAni%2+1)*50, 50);				
			}		
		}
	}

	//적 충돌 감지 
	CheckCollision_Enemy();


	//너구리 
	switch (_Rac.state) {
		case 1:
			DrawBitmap(hdc, _Rac.x, _Rac.y, _hStand, TRUE, 0, 0, 50, 50);
			break;		

		case 2:
			DrawBitmap(hdc, _Rac.x, _Rac.y, _hLeft, TRUE, _Rac.step*50, 0, (_Rac.step+1)*50, 50);
			break;

		case 3:
			DrawBitmap(hdc, _Rac.x, _Rac.y, _hRight, TRUE, _Rac.step*50, 0, (_Rac.step+1)*50, 50);
			break;

		case 4:
			DrawBitmap(hdc, _Rac.x, _Rac.y, _hUpDown, TRUE, _Rac.step*50, 0, (_Rac.step+1)*50, 50);
			break;

		case 5:	
			// 점프 패턴을 읽어 와서 그림 
			_Rac.x += _StandJump[_JumpFrame].x;
			_Rac.y += _StandJump[_JumpFrame].y;				

			DrawBitmap(hdc, _Rac.x, _Rac.y, _hStand, TRUE, (_StandJump[_JumpFrame].frame)*50, 0, 
				(_StandJump[_JumpFrame].frame+1)*50, 50);			

			_JumpFrame++;

			if ( _JumpFrame == 10 ) {
				_JumpFrame = 0;	//초기화 
				_Rac.state = 1;	//착지 하면 다시 1상태로 
			}			
			break;

		case 6:	
			//화면 왼쪽 끝이므로 화면 밖으로 안나가게 
			if ( _Rac.x >= 30 ) 
				_Rac.x += _LeftShortJump[_JumpFrame].x;
			_Rac.y += _LeftShortJump[_JumpFrame].y;		

			DrawBitmap(hdc, _Rac.x, _Rac.y, _hLeftJump, TRUE, (_LeftShortJump[_JumpFrame].frame)*50, 0,
				(_LeftShortJump[_JumpFrame].frame+1)*50, 50);	

			_JumpFrame++;

			if ( _JumpFrame == 11 ) {					
				_JumpFrame = 0;
				_Rac.state = 2;	
				//충돌 검사 
				//이 프로그램은 화살표키로 움직일때 충동 검사를 (압정, 낭떠러지)한다.
				//문제는 너구리가 점프를 끝내고 바닥에 착지 하면 
				//방향키를 안눌렀기 때문에 충돌 검사가 안된다.
				//따라서 착지하면 충돌검사를 한다.
				CheckCollision();					
			}			

			break;

		case 7:	
			//화면 왼쪽 끝이므로 화면 밖으로 안나가게 
			if ( _Rac.x >= 30 ) 
				_Rac.x += _LeftLongJump[_JumpFrame].x;
			_Rac.y += _LeftLongJump[_JumpFrame].y;				

			DrawBitmap(hdc, _Rac.x, _Rac.y, _hLeftJump, TRUE, (_LeftLongJump[_JumpFrame].frame)*50, 0, 
				(_LeftLongJump[_JumpFrame].frame+1)*50, 50);			

			_JumpFrame++;

			if ( _JumpFrame == 17 ) {
				_JumpFrame = 0;				
				_Rac.state = 2;	
				//충돌 검사 
				//이 프로그램은 화살표키로 움직일때 충동 검사를 (압정, 낭떠러지)한다.
				//문제는 너구리가 점프를 끝내고 바닥에 착지 하면 
				//방향키를 안눌렀기 때문에 충돌 검사가 안된다.
				//따라서 착지하면 충돌검사를 한다.
				CheckCollision();		
			}							
			break;

		case 8:	
			//오른쪽 경계면 
			//1층은 y좌표 775, 2층이상은 y좌표 670 
			if ( _Rac.x <= 670  || (_Rac.x <= 775 && _Rac.y >= 530) )
				_Rac.x -= _LeftShortJump[_JumpFrame].x;
			_Rac.y += _LeftShortJump[_JumpFrame].y;

			DrawBitmap(hdc, _Rac.x, _Rac.y, _hRightJump, TRUE, (_LeftShortJump[_JumpFrame].frame)*50, 0,
				(_LeftShortJump[_JumpFrame].frame+1)*50, 50);			

			_JumpFrame++;

			if ( _JumpFrame == 11 ) {				
				_JumpFrame = 0; 
				_Rac.state = 3;
				//충돌 검사 
				//이 프로그램은 화살표키로 움직일때 충동 검사를 (압정, 낭떠러지)한다.
				//문제는 너구리가 점프를 끝내고 바닥에 착지 하면 
				//방향키를 안눌렀기 때문에 충돌 검사가 안된다.
				//따라서 착지하면 충돌검사를 한다.
				CheckCollision();		

			}

			break;

		case 9:
			//오른쪽 경계면 
			//1층은 y좌표 775, 2층이상은 y좌표 670 
			if ( _Rac.x <= 670  || (_Rac.x <= 775 && _Rac.y >= 530) )
				_Rac.x -= _LeftLongJump[_JumpFrame].x;
			_Rac.y += _LeftLongJump[_JumpFrame].y;

			DrawBitmap(hdc, _Rac.x, _Rac.y, _hRightJump, TRUE, (_LeftLongJump[_JumpFrame].frame)*50, 0,
				(_LeftLongJump[_JumpFrame].frame+1)*50, 50);			

			_JumpFrame++;

			if ( _JumpFrame == 17 ) {				
				_JumpFrame = 0;
				_Rac.state = 3;
				//충돌 검사 
				//이 프로그램은 화살표키로 움직일때 충동 검사를 (압정, 낭떠러지)한다.
				//문제는 너구리가 점프를 끝내고 바닥에 착지 하면 
				//방향키를 안눌렀기 때문에 충돌 검사가 안된다.
				//따라서 착지하면 충돌검사를 한다.
				CheckCollision();	
			}

			break;

		case 10: //떨어지는 너구리  
			if (_bIsDrop_Sound == FALSE ) {
				PlaySound(NULL, _hInstance, 0);
				PlaySound(MAKEINTRESOURCE(IDR_RAC_DROP), _hInstance, SND_RESOURCE | SND_ASYNC );
				_bIsDrop_Sound = TRUE;
			}

			if (_Rac.y >= 578) {
				_Rac.y = 578;
				//다 떨어진 상태
				DrawBitmap(hdc, _Rac.x, _Rac.y, _hDie, TRUE);
				_Rac.state = 11;
			} else {
				_Rac.y += 5;	
				//덜 떨어진 상태 
				DrawBitmap(hdc, _Rac.x, _Rac.y, _hDrop, TRUE, (_iAni/2%6)*50, 0,
					(_iAni/2%6+1)*50, 50);						
			}
			break;

		case 11: //너구리 떨어진 상태 				
			break;	

	}	
	DeleteObject(SelectObject(hdc, MyBrush));		
	DeleteObject(SelectObject(hdc, MyPen));	
}
GameState GS_Play::ChangeGameState()
{
	return flag;
}

void GS_Play::CheckCollision()
{
	static HINSTANCE _hInstance=GameEngine::GetEngine()->GetInstance();
	static int x1, y1, x2, y2;
	static int xx1, yy1, xx2, yy2;

	// 너구리가 어느 방향을 보고 있느냐에 따라 좌표 설정 
	// 바운딩 박스를 방향에 따라 달리 설정, 좀더 자연스런 효과를 위해 
	// 너구리가 왼쪽을 보고 있을때 
	if (_Rac.state == 2 || _Rac.state == 6 || _Rac.state == 7) {
		x1 = _Rac.x + 10;
		y1 = _Rac.y + 5;
		x2 = _Rac.x + 30;
		y2 = _Rac.y + 45;
	} else {
		//너구리가 오른쪽 보고 있을때 
		x1 = _Rac.x + 20;
		y1 = _Rac.y + 5;
		x2 = _Rac.x + 40;
		y2 = _Rac.y + 40;

	}	

	//너구리가 바닥에 있을 때만 검사 = 점프 중이 아닐때 (앞정, 떨어짐)
	if (_Rac.state == 2 || _Rac.state == 3 ) {
		// 너구리가 4블럭이므로 아래 두 블럭을 두부분으로 나누어 충돌 검사 
		// 너구리 왼쪽 블럭
		if ( _cMap[y2/25][x1/25] == 'E' || _cMap[y2/25+1][x1/25] == '.') {
			xx1 = (x1/25 * 25)+5;		
			xx2 = ((x1/25+1) * 25)-5;		
			if ( (xx1 > x1 && xx1 < x2) || (xx2 > x1 && xx2 < x2) )	{		
				_Rac.state = 10; 
				return;
			}
		}
		// 너구리 오른쪽 블럭
		if ( _cMap[y2/25][x2/25] == 'E' || _cMap[y2/25+1][x2/25] == '.') {
			xx1 = (x2/25 * 25)+5;		
			xx2 = ((x2/25+1) * 25)-5;		
			if ( (xx1 > x1 && xx1 < x2) || (xx2 > x1 && xx2 < x2) ) {			
				_Rac.state = 10; 
				return;
			}
		}
	}

	//과일, 항아리 충돌 검사 
	char i;

	for (i = 0; i < 12; i++) {

		if (_Item[i].ch >= 'M') {
			xx1 = _Item[i].x+10;
			yy1 = _Item[i].y+30;
			xx2 = _Item[i].x+40;
			yy2 = _Item[i].y+40;

			// 충돌 감지 (바운딩 박스)
			// 너구리가 과일이나 항아리를 안 먹고 점프로 뛰어 넘을수 있게 하려고
			// y좌표까지 고려한 바운드 박스로 검사 한다. 		
			if ((xx1 > x1 && xx1 < x2 && yy1 > y1 && yy1 < y2 ) || 
				(xx1 > x1 && xx1 < x2 && yy2 > y1 && yy2 < y2 ) ||
				(xx2 > x1 && xx2 < x2 && yy1 > y1 && yy1 < y2 ) ||
				(xx2 > x1 && xx2 < x2 && yy2 > y1 && yy2 < y2 )) {

					PlaySound(MAKEINTRESOURCE(IDR_RAC_EAT), _hInstance, SND_RESOURCE | SND_ASYNC );

					//먹은 과일수 
					if (_Item[i].ch >= 'Q') _iEat++;				

					//뱀 항아리 
					if (_Item[i].ch == 'N' || _Item[i].ch == 'M') {					

						PlaySound(MAKEINTRESOURCE(IDR_SNAKE), _hInstance, SND_RESOURCE | SND_ASYNC );

						// 뱀 활성화 
						_Ene[_EnemyCount].x = _Item[i].x;
						_Ene[_EnemyCount].y = _Item[i].y;					
						_Ene[_EnemyCount].type = FALSE;
						_Ene[_EnemyCount].alpha = 10;

						if (_Item[i].ch == 'N') {
							_Ene[_EnemyCount].state = TRUE;
							_Ene[_EnemyCount].speed = -1;

						} else {
							_Ene[_EnemyCount].state = FALSE;
							_Ene[_EnemyCount].speed = 1;
						}					

						//먹은 아이템을 맵에서 지움 
						_Item[i].ch = '.';
						_EnemyCount++;

					} else {
						//점수 표시 준비 
						//나중에 점수를 표시하고 '.'으로 교체 
						_Item[i].ch = '*';
						//점수 계산 
						_iItemScoreRate *= 2;
						_iScore += _iItemScoreRate;					
					}				
			}
		}
	}
}


// 위의 함수와 비슷 
void GS_Play::CheckCollision_Enemy()
{
	static int x1, y1, x2, y2;
	static int xx1, yy1, xx2, yy2;

	if (_Rac.state == 2 || _Rac.state == 6 || _Rac.state == 7) {
		x1 = _Rac.x + 10;
		y1 = _Rac.y + 5;
		x2 = _Rac.x + 30;
		y2 = _Rac.y + 45;
	} else if (_Rac.state == 3 || _Rac.state == 8 || _Rac.state == 9) {
		x1 = _Rac.x + 20;
		y1 = _Rac.y + 5;
		x2 = _Rac.x + 40;
		y2 = _Rac.y + 45;

	} else {
		x1 = _Rac.x + 5;
		y1 = _Rac.y + 5;
		x2 = _Rac.x + 40;
		y2 = _Rac.y + 45;
	}

	char i;
	for (i = 0; i < _EnemyCount; i++) {
		if (_Ene[i].alpha == 255 ) {
			xx1 = _Ene[i].x+7;
			yy1 = _Ene[i].y+5;
			xx2 = _Ene[i].x+43;
			yy2 = _Ene[i].y+50;

			// 충돌 감지 		
			if ((x1 > xx1 && x1 < xx2 && y1 > yy1 && y1 < yy2 ) || 
				(x1 > xx1 && x1 < xx2 && y2 > yy1 && y2 < yy2 ) ||
				(x2 > xx1 && x2 < xx2 && y1 > yy1 && y2 < yy2 ) ||
				(x2 > xx1 && x2 < xx2 && y2 > yy1 && y2 < yy2 )) 				

				//너구리 죽는 상태로 
				_Rac.state = 10;
		}				
	}
}
void GS_Play::DrawMap(HDC hdc)
{
	int i,j;
	for (i = 0; i < 26; i++) {
		for (j = 0; j < 33; j++) {			
			if (_cMap[i][j] >= 'A' && _cMap[i][j] <= 'F') 
				// 기본 맵 구성품 (아이템(과일이나 항아리) 제외)
				// 아이템을 제외하는 이유는 기본 맵 구성품은 한 블럭 (25*25)인데
				// 아이템은 4블럭(50*50)이므로 그릴 좌표가 다르다. 
				// 그리고 아래의 주석 참고 
				DrawBitmap(hdc, j*25, i*25, _hMapEle[_cMap[i][j] - 65], TRUE);			 				 

			// 기본 맵 구성품을이 아닌 것들은 그리지 않고 좌표만 얻어옴 
			// 이유는 맵은 처음 부터 끝가지 변하지 않지만
			// 맵 이외의 것들은 좌표가 변하거나(적, 뱀) 없어진다(항아리, 과일) 
			// 즉, 고정된 것과 변하는 것을 따로 처리 한다.
		}
	}

	//먹은 과일 수 (이전과 변화가 있을 때만 그린다.)
	for (i = 0; i < _iEat; i++)
		DrawBitmap(hdc, 750, 490-(i*55), _hFruit[_iLevel-1], FALSE);
	// 과일 (게임 상단에 현재 레벨을 알수 있는 과일들)
	// 당근 , 앵두 ... 
	// 		for (i = 0; i < _iLevel; i++)
	// 			DrawBitmap(hdc, (670-(_iLevel-1)*55)+i*55, 70 ,_hFruit[i], TRUE);	

	//'SCORE'
	DrawBitmap(hdc, 25, 25, _hScore, TRUE);	

	//남은 너구리 수
	for (i = 0; i < _iLive; i++)
		DrawBitmap(hdc, 840, 600-(i*55), _hStand, FALSE, 0, 0, 50, 50);
}
void GS_Play::LoadMap()
{	
	static HINSTANCE _hInstance=GameEngine::GetEngine()->GetInstance();
	HRSRC hRSrc;
	HGLOBAL hMem;
	PVOID ptr;
	DWORD size;
	char *str = NULL;	
	int m_index = 0;
	char ch;	

	// 리소스에서 맵 읽어옴 
	hRSrc = FindResource(_hInstance, MAKEINTRESOURCE(IDR_MAP), "TEXT");
	size = SizeofResource(_hInstance, hRSrc);
	hMem = LoadResource(_hInstance, hRSrc);
	ptr = LockResource(hMem);
	str = (char*)malloc(size+1);
	memcpy(str, ptr, size);
	str[size] = 0;	

	// 레벨을 고려해서 맵 파일에서 offset계산 
	// +3은 #을 skip하기 위해 
	m_index += 913*(_iLevel-1)+3;

	static int i, j;

	for (i = 0; i < 26; i++) {
		for (j = 0; j < 35; j++) {	// 35까지 하는 이유는 개행문자 때문에 			
			ch = str[m_index++];
			if ( ch != '\n') 			
				_cMap[i][j] = ch;												
		}
	}		


	char index = 0;

	for (i = 0; i < 26; i++) {
		for (j = 0; j < 33; j++) {			
			if (_cMap[i][j] >= 'G' && _cMap[i][j] <= 'L') {	
				// 적이라면
				// *-- 적규칙 --
				// G부터 시작 
				// G는 오른쪽 봄 , H는 왼쪽 봄 , 둘은 속도 동일 2 + 0
				// I는 오른쪽 봄 , J는 왼쪽 봄 , 둘은 속도 동일 2 + 1 
				// K는 오른쪽 봄 , L는 왼쪽 봄 , 둘은 속도 동일 2 + 2 
				_Ene[_EnemyCount].x = j * 25;
				_Ene[_EnemyCount].y = i * 25 - 25;
				_Ene[_EnemyCount].type = TRUE;				 
				_Ene[_EnemyCount].state = (_cMap[i][j]-'G') % 2;
				_Ene[_EnemyCount].alpha = 255;	//나중에 뱀만 사용한다. 
				if (_Ene[_EnemyCount].state)
					_Ene[_EnemyCount].speed = (2+(_cMap[i][j]-'G')/2) * -1; //-1을 곱하면 반대 방향이 됨
				else
					_Ene[_EnemyCount].speed = 2+(_cMap[i][j]-'G')/2;
				_EnemyCount++;			 

			} else if (_cMap[i][j] >= 'M') {
				//아이템 (과일, 항아리)
				_Item[index].x = j * 25;
				_Item[index].y = i * 25 - 26;
				_Item[index].ch = _cMap[i][j];				 
				index++;
			}	
		}
	}


	//할당 받은 메모리 반환
	free(str);
}