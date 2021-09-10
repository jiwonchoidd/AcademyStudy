#include "Omok.h"
// ���� : �ܼ�â(cmd����â) �Ӽ����� ���Ž� �ܼ� ����� üũ�ϰ� �ٽ� �����ؾ���.
/* * �ܼ��� ���ڿ� ��� ���� �������ִ� �Լ�. */ 
void setColor(int Color) 
{ 
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), Color); 
} 
/* * â�� ũ�⸦ �����ϴ� �Լ�. */ 
void setConSize(int row, int col) 
{ 
	char buf[256] = { 0 }; 
	sprintf(buf, "mode con: lines=%d cols=%d", row, col); 
	system(buf); 
} 

/* * Ŀ���� ������ ������ ������ �����ϴ� �Լ�. */ 
void setCursorview(char visible) 
{ 
	CONSOLE_CURSOR_INFO cursor = { 1, visible }; 
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursor); 
}


///////////////////////////////////////////////////////////////////
// 1 ~ 37 ( ���� ) 2ĭ ����
// 1 ~ 19 ( ���� ) 1ĭ ����
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
			{  // ȭ�鿡 ��µɶ� �迭�� ��ġ�� ���߱����ؼ���  g_iCursorX => g_iCursorX/2 g_iCursorY=> g_iCursorY-1 �� �����
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
		printf("1p=��      2p=�� \n������ǥg_OmokBoard(%d %d)",g_iCursorX/2,g_iCursorY-1);  
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
					if(g_OmokBoard[j][i]==1) 			printf("��");
					else if(g_OmokBoard[j][i]==2)		printf("��");
					else if(g_OmokBoard[j][i]==0)		printf("��");
				}
				else if(j==18)
				{
					if(g_OmokBoard[j][i]==1)			printf("��");
					else if(g_OmokBoard[j][i]==2)		printf("��");
					else if(g_OmokBoard[j][i]==0)		printf("��");
				}
				else 
				{
					if(g_OmokBoard[j][i]==1)			printf("��");
					else if(g_OmokBoard[j][i]==2)		printf("��");
					else if(g_OmokBoard[j][i]==0)		printf("��");
				}
			}
			else if(i==18)
			{
				if(j==0) 
				{
					if(g_OmokBoard[j][i]==1)			printf("��");
					else if(g_OmokBoard[j][i]==2)		printf("��");
					else if(g_OmokBoard[j][i]==0)		printf("��");
				}
				else if(j==18) 
				{
					if(g_OmokBoard[j][i]==1)			printf("��");
					else if(g_OmokBoard[j][i]==2)		printf("��");
					else if(g_OmokBoard[j][i]==0)		printf("��");
				}
				else 
				{
					if(g_OmokBoard[j][i]==1)			printf("��");
					else if(g_OmokBoard[j][i]==2)		printf("��");
					else if(g_OmokBoard[j][i]==0)		printf("��");
				}
			}
			else
			{
				if(j==0) 
				{
					if(g_OmokBoard[j][i]==1)			printf("��");
					else if(g_OmokBoard[j][i]==2)		printf("��");
					else if(g_OmokBoard[j][i]==0)		printf("��");
				}				
				else if(j==18) 
				{
					if(g_OmokBoard[j][i]==1)			printf("��");
					else if(g_OmokBoard[j][i]==2)		printf("��");
					else if(g_OmokBoard[j][i]==0)		printf("��");
				}
				else
				{
					if(g_OmokBoard[j][i]==1)			printf("��");
					else if(g_OmokBoard[j][i]==2)		printf("��");
					else if(g_OmokBoard[j][i]==0)		printf("��");
				}
			}  
		} 
		printf("\n"); 
	}
	GotoXY(2,20);
	printf("����Ű�� ����Ű�� �̿��Ͽ� ���� �����ϴ�.");
	GotoXY(2,21);  //������� ���� ����
	printf("1p=��      2p=�� \n������ǥg_OmokBoard(%d %d)",g_iCursorX/2,g_iCursorY-1);
	GotoXY(g_iCursorX,g_iCursorY); 
}
/////////////////////////////////////////////////////////////////////
//// return Value  0 ��������
////               1 ����������
/////////////////////////////////////////////////////////////////////
//void Judgment(){
//	g_iGameLoop = 1;
//	g_bPlayer = !g_bPlayer;
//
//	if (g_iGameLoop == 0)
//	{
//		// ���� ����
//	}
//	if( g_bRestartGame == true ) // �ٸ� ���� ����
//	{
//		g_iGameStage = GAME_INIT;		
//	}
//}
void InitOmokBoard()
{
	// ������ �ʱ�ȭ ( �� / �� )
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
// return Value  0 ��������
//               1 ����������
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
			//1:�鵹,  2:�浹	
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
					// �ʱ�ȭ
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
	system("title cmd �������");
	// ���ӷ���
	while(g_iGameLoop != WHITEWIN && g_iGameLoop != BLACKWIN)
	{
		switch (g_iGameStage)
		{
			case GAME_INIT: {					
				GAME = InitOmokBoard;// ������ �ʱ�ȭ ( �� / �� )				
				g_iGameStage = KEY_CHECK;// �������� �׸���.
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
		Execute();// �Լ������� ����			
	}

	system("cls");
	GotoXY(2, 20);
	if (g_iGameLoop == WHITEWIN)
	{
		printf("���� �¸�!!.");
	}
	if (g_iGameLoop == BLACKWIN)
	{
		printf("���� �¸�!!.");
	}
	setCursorview(0);
	_getch();
}
