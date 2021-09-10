#include "Omok.h"
// 주의 : 콘솔창(cmd실행창) 속성에서 레거시 콘솔 사용을 체크하고 다시 실행해야함.
/* * 콘솔의 글자와 배경 색을 변경해주는 함수. */ 
void setColor(int Color) 
{ 
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), Color); 
} 
/* * 창의 크기를 변경하는 함수. */ 
void setConSize(int row, int col) 
{ 
	char buf[256] = { 0 }; 
	sprintf(buf, "mode con: lines=%d cols=%d", row, col); 
	system(buf); 
} 

/* * 커서를 보일지 보이지 않을지 결정하는 함수. */ 
void setCursorview(char visible) 
{ 
	CONSOLE_CURSOR_INFO cursor = { 1, visible }; 
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursor); 
}


///////////////////////////////////////////////////////////////////
// 1 ~ 37 ( 가로 ) 2칸 단위
// 1 ~ 19 ( 세로 ) 1칸 단위
///////////////////////////////////////////////////////////////////
void KeyCheck()
{	
	int iPlayTurn = 1;
	while(iPlayTurn)
	{
		switch(getch())
		{
			case RIGHT:
			{
				g_iCursorX+=2;
				if(g_iCursorX>38) 
				{
					g_iCursorX-=2;
				}
				GotoXY(g_iCursorX,g_iCursorY);
				break;
			}
			case LEFT:
			{
				g_iCursorX-=2;
				if(g_iCursorX<0) 
				{
					g_iCursorX+=2;
				}
				GotoXY(g_iCursorX,g_iCursorY);
				break;
			}
			case UP:
			{
				g_iCursorY--;
				if(g_iCursorY<=0) 
				{
					g_iCursorY++;
				}
				GotoXY(g_iCursorX,g_iCursorY);
				break;
			}
			case DOWN:
			{
				g_iCursorY++;
				if(g_iCursorY>19) 
				{
					g_iCursorY--;
				}
				GotoXY(g_iCursorX,g_iCursorY);			  
				break;
			}
			case ENTER:
			{  // 화면에 출력될때 배열과 위치를 맞추기위해서는  g_iCursorX => g_iCursorX/2 g_iCursorY=> g_iCursorY-1 를 사용함
				if(g_OmokBoard[g_iCursorX/2][g_iCursorY-1]!=0) 
					break;   
	 
				if(!g_bPlayer)
				{
					g_OmokBoard[g_iCursorX/2][g_iCursorY-1]=2;
				}
				else
				{
					g_OmokBoard[g_iCursorX/2][g_iCursorY-1]=1;
				}			
				DO(1);
				iPlayTurn=0;
			}  
		}
		GotoXY(2,21);
		printf("1p=○      2p=● \n현재좌표g_OmokBoard(%d %d)",g_iCursorX/2,g_iCursorY-1);  
		GotoXY(g_iCursorX,g_iCursorY); 
	}	
}
///////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////
void GotoXY(int x, int y)
{
	COORD Pos={x-1,y-1};
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE),Pos);
}
///////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////
void Drawboard()
{ 
	system("cls");
	for( int i=0;i<19;i++)
	{
		for(int j=0;j<19;j++)
		{
			if(i==0)
			{
				if(j==0)
				{					
					if(g_OmokBoard[j][i]==1) 			printf("○");
					else if(g_OmokBoard[j][i]==2)		printf("●");
					else if(g_OmokBoard[j][i]==0)		printf("┌");
				}
				else if(j==18)
				{
					if(g_OmokBoard[j][i]==1)			printf("○");
					else if(g_OmokBoard[j][i]==2)		printf("●");
					else if(g_OmokBoard[j][i]==0)		printf("┐");
				}
				else 
				{
					if(g_OmokBoard[j][i]==1)			printf("○");
					else if(g_OmokBoard[j][i]==2)		printf("●");
					else if(g_OmokBoard[j][i]==0)		printf("┬");
				}
			}
			else if(i==18)
			{
				if(j==0) 
				{
					if(g_OmokBoard[j][i]==1)			printf("○");
					else if(g_OmokBoard[j][i]==2)		printf("●");
					else if(g_OmokBoard[j][i]==0)		printf("└");
				}
				else if(j==18) 
				{
					if(g_OmokBoard[j][i]==1)			printf("○");
					else if(g_OmokBoard[j][i]==2)		printf("●");
					else if(g_OmokBoard[j][i]==0)		printf("┘");
				}
				else 
				{
					if(g_OmokBoard[j][i]==1)			printf("○");
					else if(g_OmokBoard[j][i]==2)		printf("●");
					else if(g_OmokBoard[j][i]==0)		printf("┴");
				}
			}
			else
			{
				if(j==0) 
				{
					if(g_OmokBoard[j][i]==1)			printf("○");
					else if(g_OmokBoard[j][i]==2)		printf("●");
					else if(g_OmokBoard[j][i]==0)		printf("├");
				}				
				else if(j==18) 
				{
					if(g_OmokBoard[j][i]==1)			printf("○");
					else if(g_OmokBoard[j][i]==2)		printf("●");
					else if(g_OmokBoard[j][i]==0)		printf("┤");
				}
				else
				{
					if(g_OmokBoard[j][i]==1)			printf("○");
					else if(g_OmokBoard[j][i]==2)		printf("●");
					else if(g_OmokBoard[j][i]==0)		printf("┼");
				}
			}  
		} 
		printf("\n"); 
	}
	GotoXY(2,20);
	printf("방향키와 엔터키를 이용하여 돌을 놓습니다.");
	GotoXY(2,21);  //디버깅을 위해 수정
	printf("1p=○      2p=● \n현재좌표g_OmokBoard(%d %d)",g_iCursorX/2,g_iCursorY-1);
	GotoXY(g_iCursorX,g_iCursorY); 
}
/////////////////////////////////////////////////////////////////////
//// return Value  0 게임종료
////               1 게임진행중
/////////////////////////////////////////////////////////////////////
//void Judgment(){
//	g_iGameLoop = 1;
//	g_bPlayer = !g_bPlayer;
//
//	if (g_iGameLoop == 0)
//	{
//		// 게임 종료
//	}
//	if( g_bRestartGame == true ) // 다른 게임 시작
//	{
//		g_iGameStage = GAME_INIT;		
//	}
//}
void InitOmokBoard()
{
	// 오목판 초기화 ( 행 / 열 )
	for (int iRowBoard = 0; iRowBoard<19; iRowBoard++)
	{
		for (int iColBoard = 0; iColBoard<19; iColBoard++)
		{
			g_OmokBoard[iRowBoard][iColBoard] = 0;
		}
	}
	Drawboard();
}
///////////////////////////////////////////////////////////////////
// return Value  0 게임종료
//               1 게임진행중
///////////////////////////////////////////////////////////////////
void Judgment()
{
	g_bPlayer = !g_bPlayer;
	int cal[8][4] = { 0 };
	int total = 0;
	g_iGameLoop = 1;

	for (int iRowBoard = 0; iRowBoard<19; iRowBoard++)
	{
		for (int iColBoard = 0; iColBoard<19; iColBoard++)
		{
			//1:백돌,  2:흑돌	
			for (int iStone = 1; iStone < 3; iStone++)
			{
				if (iStone == g_OmokBoard[iRowBoard][iColBoard])
				{
					for (int k = 1; k < 5; k++)
					{
						if (iRowBoard - (1 * k) >= 0 && iColBoard - (1 * k) >= 0)
						{
							if (iStone == g_OmokBoard[iRowBoard - (1 * k)][iColBoard - (1 * k)])
								cal[0][0 + k - 1] = 1;
						}
						if (iRowBoard - (1 * k) >= 0 && iColBoard + (0 * k) < 19)
						{
							if (iStone == g_OmokBoard[iRowBoard - (1 * k)][iColBoard + (0 * k)])
								cal[1][0 + k - 1] = 1;
						}
						if (iRowBoard - (1 * k) >= 0 && iColBoard + (1 * k) < 19)
						{
							if (iStone == g_OmokBoard[iRowBoard - (1 * k)][iColBoard + (1 * k)])
								cal[2][0 + k - 1] = 1;
						}
						if (iRowBoard + (0 * k) >= 0 && iColBoard + (1 * k) < 19)
						{
							if (iStone == g_OmokBoard[iRowBoard + (0 * k)][iColBoard + (1 * k)])
								cal[3][0 + k - 1] = 1;
						}
						if (iRowBoard + (1 * k) >= 0 && iColBoard + (1 * k) < 19)
						{
							if (iStone == g_OmokBoard[iRowBoard + (1 * k)][iColBoard + (1 * k)])
								cal[4][0 + k - 1] = 1;
						}
						if (iRowBoard + (1 * k) >= 0 && iColBoard + (0 * k) < 19)
						{
							if (iStone == g_OmokBoard[iRowBoard + (1 * k)][iColBoard + (0 * k)])
								cal[5][0 + k - 1] = 1;
						}
						if (iRowBoard + (1 * k) >= 0 && iColBoard - (1 * k) < 19)
						{
							if (iStone == g_OmokBoard[iRowBoard + (1 * k)][iColBoard - (1 * k)])
								cal[6][0 + k - 1] = 1;
						}
						if (iRowBoard + (0 * k) >= 0 && iColBoard - (1 * k) < 19)
						{
							if (iStone == g_OmokBoard[iRowBoard + (0 * k)][iColBoard - (1 * k)])
								cal[7][0 + k - 1] = 1;
						}

					}

					for (int i = 0; i < 8; i++)
					{
						for (int j = 0; j < 4; j++)
						{
							total += cal[i][j];
						}

						if (total == 4)
						{
							if (1 == iStone)
								g_iGameLoop = WHITEWIN;
							else if (2 == iStone)
								g_iGameLoop = BLACKWIN;
						}
						else
						{
							total = 0;
						}
					}
					// 초기화
					for (int i = 0; i < 8; i++)
					{
						for (int j = 0; j < 4; j++)
						{
							cal[i][j] = 0;
						}
					}
				}
			}
		}
	}
	
}
///////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////

void main()
{
	g_iGameStage = GAME_INIT;
	setConSize(30, 200);
	system("title cmd 오목게임");
	// 게임루프
	while(g_iGameLoop != WHITEWIN && g_iGameLoop != BLACKWIN)
	{
		switch (g_iGameStage)
		{
			case GAME_INIT: {					
				GAME = InitOmokBoard;// 오목판 초기화 ( 행 / 열 )				
				g_iGameStage = KEY_CHECK;// 오목판을 그린다.
				g_bRestartGame = false;
				g_bPlayer = true;					
			}break;
			case KEY_CHECK: {	
				GAME = KeyCheck;	
				g_iGameStage = GAME_DRAW;
			}break;
			case GAME_DRAW: {
				GAME = Drawboard;				
				g_iGameStage = GAME_CHECK;
			}break;
			case GAME_CHECK: {
				GAME = Judgment;				
				g_iGameStage = KEY_CHECK;
			}break;
		}
		Execute();// 함수포인터 실행			
	}

	system("cls");
	GotoXY(2, 20);
	if (g_iGameLoop == WHITEWIN)
	{
		printf("백이 승리!!.");
	}
	if (g_iGameLoop == BLACKWIN)
	{
		printf("흑이 승리!!.");
	}
	setCursorview(0);
	_getch();
}
