#pragma once
#include "igamestate.h"
#include "Resource.h"
#include <windows.h>
#include "GameEngine.h"
#include "Racoon.h"
#include "Enemy.h"

#define	LIVE		3
#define LAST_LEVEL	4

//����, �׾Ƹ� 
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
	HBITMAP		_hPot;// �׾Ƹ� 
	HBITMAP		_hFruit[4];// ���� 
	HBITMAP		_hLeft;// �ʱ��� �̹��� 
	HBITMAP		_hRight;
	HBITMAP		_hUpDown;
	HBITMAP		_hStand;
	HBITMAP		_hLeftJump;
	HBITMAP		_hRightJump;
	HBITMAP		_hDrop;
	HBITMAP		_hDie;
	HBITMAP		_hSurprise;//���
	HBITMAP		_hEnemyRight;//�� 
	HBITMAP		_hEnemyLeft;
	HBITMAP		_hEnemyRightRed;
	HBITMAP		_hEnemyLeftRed;
	HBITMAP		_hSnakeLeft;//��
	HBITMAP		_hSnakeRight;
	HBITMAP		_hBaby;//�Ʊ� 
	//����
	HBITMAP		_hDigit;
	HBITMAP		_hDigit_sm;

	HBITMAP		_hLets;// 'LETS...'
	HBITMAP		_hBonus;// 'BONUS'
	HBITMAP		_hGameOver;// 'GAME OVER'
	HBITMAP		_hScore;// 'SCORE'
	HBITMAP		_hNext;// 'Next'
	HBITMAP		_hEnding;// ending
	HBITMAP		_hMap;//�ϼ��� ���̹��� 
	// ����ü 
	CRacoon		_Rac;
	CEnemy		_Ene[7];
	Item		_Item[12];
	char		_EnemyCount;	// ���� ���� 	
	char		_GameState;//���� ����(4���� ����)
	char		_cMap[26][33];//������ 
	char		_iLevel;//���� 
	char		_iLive;//���� �ʱ��� �� 
	char		_iEat;//���� ���� ��
	char		_JumpFrame;//������ �����ٶ� �ʿ��� ī���� 
	char		_ScoreShow;//���� �������� ���� ǥ�� �ð� 
	int			_iAni;//���ϸ��̼��� ���ؼ�
	int			_iTime;//���� �ð�
	int			_iScore;//���� 
	int			_iItemScoreRate;//���� �����̳� �׾Ƹ� ���� ������ (2�辿 ����)
	BOOL		_bIsDrawAll;
	BOOL		_bIsDrop_Sound;//�ʱ����� �׾ �������� ���� ���� ����
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