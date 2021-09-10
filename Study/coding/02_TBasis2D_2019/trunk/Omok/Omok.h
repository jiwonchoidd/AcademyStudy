//////////////////////  제어문/반복문 /////////////////////////
// 사용자 정의 함수사용
// while()
// switch() case:
// if ~ else
// for()
//////////////////////////////////////////////////////////////
#define _CRT_SECURE_NO_WARNINGS

#include <windows.h>	//gotoxy(),SetConsoleCursorPosition()
#include <cstdio>
#include <cstdlib> 
#include <conio.h>		//kbhit(),_getch()

#define	RIGHT	77
#define LEFT	75
#define UP		72
#define DOWN	80
#define ENTER	13
#define WHITEWIN 2
#define BLACKWIN 3
#define DO(t) Beep((DWORD)(131*pow(1.06f,0)),(DWORD)(300*(t)));
// 함수 포인터 선언
void(*GAME)();
enum STATE { GAME_INIT = 0, KEY_CHECK, GAME_DRAW, GAME_CHECK, };


void	KeyCheck();
void	GotoXY(int x, int y);
void	Drawboard();
void  	Judgment();

int		g_OmokBoard[19][19];               
int		g_iCursorX=19;
int		g_iCursorY=9;
bool	g_bPlayer = true;// 게임플레이어 
int		g_iGameLoop = 1;// 게임 진행 판정 
int		g_iGameStage = GAME_INIT;
bool	g_bRestartGame = false; // 다른 게임 시작

void Execute()
{
	GAME();
}