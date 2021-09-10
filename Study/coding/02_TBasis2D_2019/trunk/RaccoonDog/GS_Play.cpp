#include "GS_Play.h"

GS_Play::GS_Play(void)
{
	flag=GS_WAIT;
	// �ʱ�ȭ 
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

	// ���� 
	_hDigit = LoadBitmap(_hInstance, MAKEINTRESOURCE(IDB_DIGIT));
	_hDigit_sm = LoadBitmap(_hInstance, MAKEINTRESOURCE(IDB_DIGIT_SM));

	// �׾Ƹ� 
	_hPot = LoadBitmap(_hInstance, MAKEINTRESOURCE(IDB_MAP_N));

	// ���� 
	_hFruit[0] = LoadBitmap(_hInstance, MAKEINTRESOURCE(IDB_MAP_Q));
	_hFruit[1] = LoadBitmap(_hInstance, MAKEINTRESOURCE(IDB_MAP_R));
	_hFruit[2] = LoadBitmap(_hInstance, MAKEINTRESOURCE(IDB_MAP_S));
	_hFruit[3] = LoadBitmap(_hInstance, MAKEINTRESOURCE(IDB_MAP_T));

	// �ʱ��� 
	_hLeft = LoadBitmap(_hInstance, MAKEINTRESOURCE(IDB_LEFT));
	_hRight = LoadBitmap(_hInstance, MAKEINTRESOURCE(IDB_RIGHT));
	_hUpDown = LoadBitmap(_hInstance, MAKEINTRESOURCE(IDB_UPDOWN));
	_hStand = LoadBitmap(_hInstance, MAKEINTRESOURCE(IDB_STAND));	
	_hLeftJump = LoadBitmap(_hInstance, MAKEINTRESOURCE(IDB_LEFT_JUMP));
	_hRightJump = LoadBitmap(_hInstance, MAKEINTRESOURCE(IDB_RIGHT_JUMP));
	_hDrop = LoadBitmap(_hInstance, MAKEINTRESOURCE(IDB_DROP));
	_hDie = LoadBitmap(_hInstance, MAKEINTRESOURCE(IDB_DIE));

	// ��¦ ��� ����
	_hSurprise = LoadBitmap(_hInstance, MAKEINTRESOURCE(IDB_SURPRISE));

	// �� 
	_hEnemyLeft = LoadBitmap(_hInstance, MAKEINTRESOURCE(IDB_ENEMY_LEFT));
	_hEnemyRight = LoadBitmap(_hInstance, MAKEINTRESOURCE(IDB_ENEMY_RIGHT));
	_hEnemyLeftRed = LoadBitmap(_hInstance, MAKEINTRESOURCE(IDB_ENEMY_LEFT_RED));
	_hEnemyRightRed = LoadBitmap(_hInstance, MAKEINTRESOURCE(IDB_ENEMY_RIGHT_RED));

	// �� 
	_hSnakeLeft = LoadBitmap(_hInstance, MAKEINTRESOURCE(IDB_SNAKE_LEFT));
	_hSnakeRight = LoadBitmap(_hInstance, MAKEINTRESOURCE(IDB_SNAKE_RIGHT));

	// �Ʊ� 
	_hBaby = LoadBitmap(_hInstance, MAKEINTRESOURCE(IDB_BABY));
}
void GS_Play::GameInit()
{
	//�ʱ�ȭ
	_Rac.Init();
	_iTime = 500;			//���� ���� �ð� 
	_iItemScoreRate = 5;	//������ ����, 2�辿 �������鼭 ���� 
	_iEat = 0;				//���� ���ϼ� 
	_iAni = 0;				//�ִϸ��̼� ȿ���� ���� 
	_bIsDrop_Sound = FALSE;	//�ʱ����� �������� ���� �Ҹ� ���� 

	_iLevel=1;
	_EnemyCount = 0;		//���� ���� 

	_ScoreShow = 0;			//���� ���� ���� ǥ�� �ð� 
	_JumpFrame = 0;			//������ ���� �ٶ� �ʿ� (ī����)
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

			// 'F'�� ��ٸ�,  +20 �� ���� �ʱ����� �߽��� ���߱� ���� 
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
				} //Ű ���� 
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
				} // Ű ���� 
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
	/// ,���� �����ϴٰ� �߰��� ��.... ���� �Ͼ���п��������� ���� ���ڲٳ�...
	///
	///
	///
	int i=0;
	static HINSTANCE _hInstance=GameEngine::GetEngine()->GetInstance();
	// �ð� �ٸ� ǥ���� �귯�ÿ� �� 
	HANDLE MyPen = CreatePen(PS_SOLID, 0, RGB(0, 0, 0));
	HANDLE OldPen = (HPEN)SelectObject(hdc, MyPen);

	HANDLE MyBrush = CreateSolidBrush(RGB(25, 184, 5));	
	HANDLE OldBrush = (HBRUSH)SelectObject(hdc, MyBrush);	

	//���ϸ��̼� ������ ���� 	
	//�ʱ����� ������ �������� ���ߴ� �͵��� _iTime�� �������� ���ϸ��̼�
	//�ʱ����� �����鼭 �ϴ� �������� _iAni�� �̿� 	
	_iAni++;

	// Play���� ���� 

	// ���� �� ���� 
	// _ScoreShow == 1 �� �� ������ ���������� ���� ���� ���� ���� �����ַ��� 
	if ( _iEat == 8 && _ScoreShow == 1) {					
		Sleep(500);
		_iAni = 0;			
		_GameState = 2;			
	}	
	// �ʱ��� ���� 	
	if (_Rac.state == 11) {
		PlaySound(NULL, _hInstance, 0);
		PlaySound(MAKEINTRESOURCE(IDR_RAC_DIE), _hInstance, SND_RESOURCE | SND_ASYNC );
		Sleep(1500);			
		if (_iLive == 0) { // ������ �ʱ����� ������ 
			GameInit();
			_GameState = 3;	//gameover
		} else {
			_iLive--;
			GameInit();
			LoadMap();
		}
	}


	// ���� �ð�	
	// �ʱ����� �������ų� ������ �ð��� �����.
	if ( _iAni % 5 == 0 && _Rac.state != 10 && _Rac.state != 11 ) {
		_iTime--;
		if (_iTime == 0) 			
			_Rac.state = 10;	//�ʱ��� ���� 		
	}

	// ��(ó�� ���� �Ҷ� �� ��ü�� �ѹ� �׸���.)
	DrawMap(hdc);


	// ���� ǥ�� (������ ��ȭ�� ���� ���� �׸���.)
	static int Score;
	DrawDigit(hdc, 25, 50, _iScore, _hDigit, 7);	
	Score = _iScore;

	// �ð� �� ǥ�� 
	Rectangle(hdc, 650-_iTime, 25, 650, 50); 	





	// �׾Ƹ� & ���� ǥ�� 
	for (i = 0; i < 12; i++) {	
		//�׾Ƹ� 
		if (_Item[i].ch == 'M' || _Item[i].ch == 'N' || _Item[i].ch == 'O') 
			DrawBitmap(hdc, _Item[i].x, _Item[i].y, _hPot, FALSE);			 
		//���� 
		else if (_Item[i].ch >= 'Q') 
			DrawBitmap(hdc, _Item[i].x, _Item[i].y, _hFruit[_iLevel-1], FALSE);
		//�ʱ����� ���� �Ϳ� ���� ���� ǥ�� 
		//�ʱ����� ������ *�� ��ġ�ϰ� ������ ǥ���Ѵ����� .�� �ٲ�
		else if (_Item[i].ch == '*') {			
			DrawDigit(hdc, _Item[i].x, _Item[i].y+25, _iItemScoreRate, _hDigit_sm);

			//������ 10������ ���� ������ 			
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

	//�� ǥ�� 
	//��� �Ϲ����� ���� ���� �׸��� ������ ���� ȿ��(Alpha) ������.	

	for (i = 0; i <_EnemyCount; i++) {

		//��
		if (_Ene[i].type == FALSE) {	

			//�ʱ����� ����ְ� (�ʱ����� ������ ȭ���� ����� �ǹǷ�)
			//���� ���� 255�� �Ǹ� ���� �����δ�.
			if (_Rac.state != 10 && _Rac.state != 11 && _Ene[i].alpha == 255) _Ene[i].x += _Ene[i].speed;

			if ( _Ene[i].x <= 30) {	//���� ��
				_Ene[i].x = 30;
				_Ene[i].speed *= -1;
				_Ene[i].state = !_Ene[i].state;
			} else if ( _Ene[i].x >= 670) { //������ ��
				_Ene[i].x = 670;
				_Ene[i].speed *= -1;
				_Ene[i].state = !_Ene[i].state;
			}

			if (_Ene[i].state == FALSE) //����(�������� ���� ������)
				if (_Ene[i].alpha != 255 ) {
					_Ene[i].alpha += 5;	//�����ϰ�
					DrawBitmap(hdc, _Ene[i].x, _Ene[i].y ,_hMap, FALSE, 200, 0, 250, 50); //���������� ������ ���� ����� (200,0 ~ 250,50�� ������)
					DrawBitmapAlpha(hdc, _Ene[i].x, _Ene[i].y , _hSnakeRight, _Ene[i].alpha, (_iAni/5%2)*50, 0, (_iAni/5%2+1)*50, 50);
				} else
					DrawBitmap(hdc, _Ene[i].x, _Ene[i].y , _hSnakeRight, TRUE, (_iAni/5%2)*50, 0, (_iAni/5%2+1)*50, 50);
			else if (_Ene[i].alpha != 255 ) {
				_Ene[i].alpha += 5;	//�����ϰ�  
				DrawBitmapAlpha(hdc, _Ene[i].x, _Ene[i].y , _hSnakeLeft, _Ene[i].alpha, (_iAni/5%2)*50, 0, (_iAni/5%2+1)*50, 50);
			} else // ����(������ ���� ������)
				DrawBitmap(hdc, _Ene[i].x, _Ene[i].y , _hSnakeLeft, TRUE, (_iAni/5%2)*50, 0, (_iAni/5%2+1)*50, 50);
		}
	}



	//�Ϲ����� �� 
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

			if (_Ene[i].speed == 4 || _Ene[i].speed == -4) { // �ӵ��� 4�� ���� ������, �������� �ʷϻ� 
				if (_Ene[i].state == FALSE) //����
					DrawBitmap(hdc, _Ene[i].x, _Ene[i].y , _hEnemyRightRed, TRUE, (_iAni%2)*50, 0, (_iAni%2+1)*50, 50);
				else 
					DrawBitmap(hdc, _Ene[i].x, _Ene[i].y , _hEnemyLeftRed, TRUE, (_iAni%2)*50, 0, (_iAni%2+1)*50, 50);
			} else {
				if (_Ene[i].state == FALSE) //����
					DrawBitmap(hdc, _Ene[i].x, _Ene[i].y , _hEnemyRight, TRUE, (_iAni%2)*50, 0, (_iAni%2+1)*50, 50);
				else
					DrawBitmap(hdc, _Ene[i].x, _Ene[i].y , _hEnemyLeft, TRUE, (_iAni%2)*50, 0, (_iAni%2+1)*50, 50);				
			}		
		}
	}

	//�� �浹 ���� 
	CheckCollision_Enemy();


	//�ʱ��� 
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
			// ���� ������ �о� �ͼ� �׸� 
			_Rac.x += _StandJump[_JumpFrame].x;
			_Rac.y += _StandJump[_JumpFrame].y;				

			DrawBitmap(hdc, _Rac.x, _Rac.y, _hStand, TRUE, (_StandJump[_JumpFrame].frame)*50, 0, 
				(_StandJump[_JumpFrame].frame+1)*50, 50);			

			_JumpFrame++;

			if ( _JumpFrame == 10 ) {
				_JumpFrame = 0;	//�ʱ�ȭ 
				_Rac.state = 1;	//���� �ϸ� �ٽ� 1���·� 
			}			
			break;

		case 6:	
			//ȭ�� ���� ���̹Ƿ� ȭ�� ������ �ȳ����� 
			if ( _Rac.x >= 30 ) 
				_Rac.x += _LeftShortJump[_JumpFrame].x;
			_Rac.y += _LeftShortJump[_JumpFrame].y;		

			DrawBitmap(hdc, _Rac.x, _Rac.y, _hLeftJump, TRUE, (_LeftShortJump[_JumpFrame].frame)*50, 0,
				(_LeftShortJump[_JumpFrame].frame+1)*50, 50);	

			_JumpFrame++;

			if ( _JumpFrame == 11 ) {					
				_JumpFrame = 0;
				_Rac.state = 2;	
				//�浹 �˻� 
				//�� ���α׷��� ȭ��ǥŰ�� �����϶� �浿 �˻縦 (����, ��������)�Ѵ�.
				//������ �ʱ����� ������ ������ �ٴڿ� ���� �ϸ� 
				//����Ű�� �ȴ����� ������ �浹 �˻簡 �ȵȴ�.
				//���� �����ϸ� �浹�˻縦 �Ѵ�.
				CheckCollision();					
			}			

			break;

		case 7:	
			//ȭ�� ���� ���̹Ƿ� ȭ�� ������ �ȳ����� 
			if ( _Rac.x >= 30 ) 
				_Rac.x += _LeftLongJump[_JumpFrame].x;
			_Rac.y += _LeftLongJump[_JumpFrame].y;				

			DrawBitmap(hdc, _Rac.x, _Rac.y, _hLeftJump, TRUE, (_LeftLongJump[_JumpFrame].frame)*50, 0, 
				(_LeftLongJump[_JumpFrame].frame+1)*50, 50);			

			_JumpFrame++;

			if ( _JumpFrame == 17 ) {
				_JumpFrame = 0;				
				_Rac.state = 2;	
				//�浹 �˻� 
				//�� ���α׷��� ȭ��ǥŰ�� �����϶� �浿 �˻縦 (����, ��������)�Ѵ�.
				//������ �ʱ����� ������ ������ �ٴڿ� ���� �ϸ� 
				//����Ű�� �ȴ����� ������ �浹 �˻簡 �ȵȴ�.
				//���� �����ϸ� �浹�˻縦 �Ѵ�.
				CheckCollision();		
			}							
			break;

		case 8:	
			//������ ���� 
			//1���� y��ǥ 775, 2���̻��� y��ǥ 670 
			if ( _Rac.x <= 670  || (_Rac.x <= 775 && _Rac.y >= 530) )
				_Rac.x -= _LeftShortJump[_JumpFrame].x;
			_Rac.y += _LeftShortJump[_JumpFrame].y;

			DrawBitmap(hdc, _Rac.x, _Rac.y, _hRightJump, TRUE, (_LeftShortJump[_JumpFrame].frame)*50, 0,
				(_LeftShortJump[_JumpFrame].frame+1)*50, 50);			

			_JumpFrame++;

			if ( _JumpFrame == 11 ) {				
				_JumpFrame = 0; 
				_Rac.state = 3;
				//�浹 �˻� 
				//�� ���α׷��� ȭ��ǥŰ�� �����϶� �浿 �˻縦 (����, ��������)�Ѵ�.
				//������ �ʱ����� ������ ������ �ٴڿ� ���� �ϸ� 
				//����Ű�� �ȴ����� ������ �浹 �˻簡 �ȵȴ�.
				//���� �����ϸ� �浹�˻縦 �Ѵ�.
				CheckCollision();		

			}

			break;

		case 9:
			//������ ���� 
			//1���� y��ǥ 775, 2���̻��� y��ǥ 670 
			if ( _Rac.x <= 670  || (_Rac.x <= 775 && _Rac.y >= 530) )
				_Rac.x -= _LeftLongJump[_JumpFrame].x;
			_Rac.y += _LeftLongJump[_JumpFrame].y;

			DrawBitmap(hdc, _Rac.x, _Rac.y, _hRightJump, TRUE, (_LeftLongJump[_JumpFrame].frame)*50, 0,
				(_LeftLongJump[_JumpFrame].frame+1)*50, 50);			

			_JumpFrame++;

			if ( _JumpFrame == 17 ) {				
				_JumpFrame = 0;
				_Rac.state = 3;
				//�浹 �˻� 
				//�� ���α׷��� ȭ��ǥŰ�� �����϶� �浿 �˻縦 (����, ��������)�Ѵ�.
				//������ �ʱ����� ������ ������ �ٴڿ� ���� �ϸ� 
				//����Ű�� �ȴ����� ������ �浹 �˻簡 �ȵȴ�.
				//���� �����ϸ� �浹�˻縦 �Ѵ�.
				CheckCollision();	
			}

			break;

		case 10: //�������� �ʱ���  
			if (_bIsDrop_Sound == FALSE ) {
				PlaySound(NULL, _hInstance, 0);
				PlaySound(MAKEINTRESOURCE(IDR_RAC_DROP), _hInstance, SND_RESOURCE | SND_ASYNC );
				_bIsDrop_Sound = TRUE;
			}

			if (_Rac.y >= 578) {
				_Rac.y = 578;
				//�� ������ ����
				DrawBitmap(hdc, _Rac.x, _Rac.y, _hDie, TRUE);
				_Rac.state = 11;
			} else {
				_Rac.y += 5;	
				//�� ������ ���� 
				DrawBitmap(hdc, _Rac.x, _Rac.y, _hDrop, TRUE, (_iAni/2%6)*50, 0,
					(_iAni/2%6+1)*50, 50);						
			}
			break;

		case 11: //�ʱ��� ������ ���� 				
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

	// �ʱ����� ��� ������ ���� �ִ��Ŀ� ���� ��ǥ ���� 
	// �ٿ�� �ڽ��� ���⿡ ���� �޸� ����, ���� �ڿ����� ȿ���� ���� 
	// �ʱ����� ������ ���� ������ 
	if (_Rac.state == 2 || _Rac.state == 6 || _Rac.state == 7) {
		x1 = _Rac.x + 10;
		y1 = _Rac.y + 5;
		x2 = _Rac.x + 30;
		y2 = _Rac.y + 45;
	} else {
		//�ʱ����� ������ ���� ������ 
		x1 = _Rac.x + 20;
		y1 = _Rac.y + 5;
		x2 = _Rac.x + 40;
		y2 = _Rac.y + 40;

	}	

	//�ʱ����� �ٴڿ� ���� ���� �˻� = ���� ���� �ƴҶ� (����, ������)
	if (_Rac.state == 2 || _Rac.state == 3 ) {
		// �ʱ����� 4���̹Ƿ� �Ʒ� �� ���� �κκ����� ������ �浹 �˻� 
		// �ʱ��� ���� ��
		if ( _cMap[y2/25][x1/25] == 'E' || _cMap[y2/25+1][x1/25] == '.') {
			xx1 = (x1/25 * 25)+5;		
			xx2 = ((x1/25+1) * 25)-5;		
			if ( (xx1 > x1 && xx1 < x2) || (xx2 > x1 && xx2 < x2) )	{		
				_Rac.state = 10; 
				return;
			}
		}
		// �ʱ��� ������ ��
		if ( _cMap[y2/25][x2/25] == 'E' || _cMap[y2/25+1][x2/25] == '.') {
			xx1 = (x2/25 * 25)+5;		
			xx2 = ((x2/25+1) * 25)-5;		
			if ( (xx1 > x1 && xx1 < x2) || (xx2 > x1 && xx2 < x2) ) {			
				_Rac.state = 10; 
				return;
			}
		}
	}

	//����, �׾Ƹ� �浹 �˻� 
	char i;

	for (i = 0; i < 12; i++) {

		if (_Item[i].ch >= 'M') {
			xx1 = _Item[i].x+10;
			yy1 = _Item[i].y+30;
			xx2 = _Item[i].x+40;
			yy2 = _Item[i].y+40;

			// �浹 ���� (�ٿ�� �ڽ�)
			// �ʱ����� �����̳� �׾Ƹ��� �� �԰� ������ �پ� ������ �ְ� �Ϸ���
			// y��ǥ���� ����� �ٿ�� �ڽ��� �˻� �Ѵ�. 		
			if ((xx1 > x1 && xx1 < x2 && yy1 > y1 && yy1 < y2 ) || 
				(xx1 > x1 && xx1 < x2 && yy2 > y1 && yy2 < y2 ) ||
				(xx2 > x1 && xx2 < x2 && yy1 > y1 && yy1 < y2 ) ||
				(xx2 > x1 && xx2 < x2 && yy2 > y1 && yy2 < y2 )) {

					PlaySound(MAKEINTRESOURCE(IDR_RAC_EAT), _hInstance, SND_RESOURCE | SND_ASYNC );

					//���� ���ϼ� 
					if (_Item[i].ch >= 'Q') _iEat++;				

					//�� �׾Ƹ� 
					if (_Item[i].ch == 'N' || _Item[i].ch == 'M') {					

						PlaySound(MAKEINTRESOURCE(IDR_SNAKE), _hInstance, SND_RESOURCE | SND_ASYNC );

						// �� Ȱ��ȭ 
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

						//���� �������� �ʿ��� ���� 
						_Item[i].ch = '.';
						_EnemyCount++;

					} else {
						//���� ǥ�� �غ� 
						//���߿� ������ ǥ���ϰ� '.'���� ��ü 
						_Item[i].ch = '*';
						//���� ��� 
						_iItemScoreRate *= 2;
						_iScore += _iItemScoreRate;					
					}				
			}
		}
	}
}


// ���� �Լ��� ��� 
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

			// �浹 ���� 		
			if ((x1 > xx1 && x1 < xx2 && y1 > yy1 && y1 < yy2 ) || 
				(x1 > xx1 && x1 < xx2 && y2 > yy1 && y2 < yy2 ) ||
				(x2 > xx1 && x2 < xx2 && y1 > yy1 && y2 < yy2 ) ||
				(x2 > xx1 && x2 < xx2 && y2 > yy1 && y2 < yy2 )) 				

				//�ʱ��� �״� ���·� 
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
				// �⺻ �� ����ǰ (������(�����̳� �׾Ƹ�) ����)
				// �������� �����ϴ� ������ �⺻ �� ����ǰ�� �� �� (25*25)�ε�
				// �������� 4��(50*50)�̹Ƿ� �׸� ��ǥ�� �ٸ���. 
				// �׸��� �Ʒ��� �ּ� ���� 
				DrawBitmap(hdc, j*25, i*25, _hMapEle[_cMap[i][j] - 65], TRUE);			 				 

			// �⺻ �� ����ǰ���� �ƴ� �͵��� �׸��� �ʰ� ��ǥ�� ���� 
			// ������ ���� ó�� ���� ������ ������ ������
			// �� �̿��� �͵��� ��ǥ�� ���ϰų�(��, ��) ��������(�׾Ƹ�, ����) 
			// ��, ������ �Ͱ� ���ϴ� ���� ���� ó�� �Ѵ�.
		}
	}

	//���� ���� �� (������ ��ȭ�� ���� ���� �׸���.)
	for (i = 0; i < _iEat; i++)
		DrawBitmap(hdc, 750, 490-(i*55), _hFruit[_iLevel-1], FALSE);
	// ���� (���� ��ܿ� ���� ������ �˼� �ִ� ���ϵ�)
	// ��� , �޵� ... 
	// 		for (i = 0; i < _iLevel; i++)
	// 			DrawBitmap(hdc, (670-(_iLevel-1)*55)+i*55, 70 ,_hFruit[i], TRUE);	

	//'SCORE'
	DrawBitmap(hdc, 25, 25, _hScore, TRUE);	

	//���� �ʱ��� ��
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

	// ���ҽ����� �� �о�� 
	hRSrc = FindResource(_hInstance, MAKEINTRESOURCE(IDR_MAP), "TEXT");
	size = SizeofResource(_hInstance, hRSrc);
	hMem = LoadResource(_hInstance, hRSrc);
	ptr = LockResource(hMem);
	str = (char*)malloc(size+1);
	memcpy(str, ptr, size);
	str[size] = 0;	

	// ������ ����ؼ� �� ���Ͽ��� offset��� 
	// +3�� #�� skip�ϱ� ���� 
	m_index += 913*(_iLevel-1)+3;

	static int i, j;

	for (i = 0; i < 26; i++) {
		for (j = 0; j < 35; j++) {	// 35���� �ϴ� ������ ���๮�� ������ 			
			ch = str[m_index++];
			if ( ch != '\n') 			
				_cMap[i][j] = ch;												
		}
	}		


	char index = 0;

	for (i = 0; i < 26; i++) {
		for (j = 0; j < 33; j++) {			
			if (_cMap[i][j] >= 'G' && _cMap[i][j] <= 'L') {	
				// ���̶��
				// *-- ����Ģ --
				// G���� ���� 
				// G�� ������ �� , H�� ���� �� , ���� �ӵ� ���� 2 + 0
				// I�� ������ �� , J�� ���� �� , ���� �ӵ� ���� 2 + 1 
				// K�� ������ �� , L�� ���� �� , ���� �ӵ� ���� 2 + 2 
				_Ene[_EnemyCount].x = j * 25;
				_Ene[_EnemyCount].y = i * 25 - 25;
				_Ene[_EnemyCount].type = TRUE;				 
				_Ene[_EnemyCount].state = (_cMap[i][j]-'G') % 2;
				_Ene[_EnemyCount].alpha = 255;	//���߿� �츸 ����Ѵ�. 
				if (_Ene[_EnemyCount].state)
					_Ene[_EnemyCount].speed = (2+(_cMap[i][j]-'G')/2) * -1; //-1�� ���ϸ� �ݴ� ������ ��
				else
					_Ene[_EnemyCount].speed = 2+(_cMap[i][j]-'G')/2;
				_EnemyCount++;			 

			} else if (_cMap[i][j] >= 'M') {
				//������ (����, �׾Ƹ�)
				_Item[index].x = j * 25;
				_Item[index].y = i * 25 - 26;
				_Item[index].ch = _cMap[i][j];				 
				index++;
			}	
		}
	}


	//�Ҵ� ���� �޸� ��ȯ
	free(str);
}