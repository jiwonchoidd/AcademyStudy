#pragma once
#include "igamestate.h"
#include "Resource.h"
#include <windows.h>
#include "GameEngine.h"
#include "Racoon.h"
#include "Enemy.h"

#define	LIVE		3
#define LAST_LEVEL	4

//과일, 항아리 
struct Item {
	int x;
	int y;
	char ch;
};	

class GS_Play :
	public IGameState
{
private:
	GameState flag;
	HBITMAP		_hMapEle[6];
	HBITMAP		_hPot;// 항아리 
	HBITMAP		_hFruit[4];// 과일 
	HBITMAP		_hLeft;// 너구리 이미지 
	HBITMAP		_hRight;
	HBITMAP		_hUpDown;
	HBITMAP		_hStand;
	HBITMAP		_hLeftJump;
	HBITMAP		_hRightJump;
	HBITMAP		_hDrop;
	HBITMAP		_hDie;
	HBITMAP		_hSurprise;//놀람
	HBITMAP		_hEnemyRight;//적 
	HBITMAP		_hEnemyLeft;
	HBITMAP		_hEnemyRightRed;
	HBITMAP		_hEnemyLeftRed;
	HBITMAP		_hSnakeLeft;//뱀
	HBITMAP		_hSnakeRight;
	HBITMAP		_hBaby;//아기 
	//숫자
	HBITMAP		_hDigit;
	HBITMAP		_hDigit_sm;

	HBITMAP		_hLets;// 'LETS...'
	HBITMAP		_hBonus;// 'BONUS'
	HBITMAP		_hGameOver;// 'GAME OVER'
	HBITMAP		_hScore;// 'SCORE'
	HBITMAP		_hNext;// 'Next'
	HBITMAP		_hEnding;// ending
	HBITMAP		_hMap;//완성된 맵이미지 
	// 구조체 
	CRacoon		_Rac;
	CEnemy		_Ene[7];
	Item		_Item[12];
	char		_EnemyCount;	// 적의 갯수 	
	char		_GameState;//게임 상태(4가지 상태)
	char		_cMap[26][33];//맵정보 
	char		_iLevel;//레벨 
	char		_iLive;//남은 너구리 수 
	char		_iEat;//먹은 과일 수
	char		_JumpFrame;//점프를 보여줄때 필요한 카운터 
	char		_ScoreShow;//먹은 아이템의 점수 표시 시간 
	int			_iAni;//에니메이션을 위해서
	int			_iTime;//남은 시간
	int			_iScore;//점수 
	int			_iItemScoreRate;//먹은 과일이나 항아리 점수 증가폭 (2배씩 증가)
	BOOL		_bIsDrawAll;
	BOOL		_bIsDrop_Sound;//너구리가 죽어서 떨어질때 음악 연주 상태
public:
	GS_Play(void);
	~GS_Play(void);
	void Init();
	void GameInit();
	void Update();
	void Render(HDC hdc);
	GameState ChangeGameState();
	void CheckCollision();
	void CheckCollision_Enemy();
	void DrawMap(HDC hdc);
	void LoadMap();
};