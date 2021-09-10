/////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////// AI.CPP /////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////
#include <windows.h>
#include <windowsx.h>
#include <ddraw.h>
#include <stdio.h>
#include "AI.h"
#include "Ddutil.h"
#define  SHIFT 4                 //타일 사이즈 16



///////////////////////////////////////////////////
////////////////// WinMain() //////////////////////
///////////////////////////////////////////////////
int PASCAL WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow )
{
    int index=0;
	MSG msg;
  	if ( !GameMode(hInstance, nCmdShow, 640, 480, 16) ) 
		return FALSE;	
	UnitImage=DDLoadBitmap(DirectOBJ,L"tile2.bmp",0,0);        //바탕타일 
	BackImage=DDLoadBitmap(DirectOBJ, L"tile4.bmp",0,0);        //바탕타일  
	BackImage2=DDLoadBitmap(DirectOBJ, L"Scar.bmp",0,0);        //적 유닛 1,2
	Units=DDLoadBitmap(DirectOBJ, L"sand.bmp",0,0);
	UnitImage2=DDLoadBitmap(DirectOBJ, L"PlayerUnit.bmp",0,0);  //플레이어 유닛 
	EndImage=DDLoadBitmap(DirectOBJ, L"end.bmp",0,0);
	StartImage=DDLoadBitmap(DirectOBJ, L"start.bmp",0,0);

	DDSetColorKey(UnitImage,RGB(0,0,0));	                  //투명색 지정 (검정색)
	DDSetColorKey(UnitImage2,RGB(0,0,0));
	DDSetColorKey(BackImage2,RGB(0,0,0));
	
	Astar=new ASTAR(Tile_Map,COLS,ROWS);
	Astar2=new ASTAR(Tile_Map,COLS,ROWS);
	Astar3=new ASTAR(Tile_Map,COLS,ROWS);
	
	INIT_UNITS_POINT_XY();									//모든 유닛의 초기 좌표 지정 
	InitCreate(0,0,0);	                                    //처음 플레이어의 이동 좌표 고정으로 지정.
	                                                        //만약 변경하면 초기 목표가 설정되어 유닛이 이동됨.
	RECT BackRect={0,0,640,480};
	BackScreen->BltFast(0,0,StartImage,&BackRect,DDBLTFAST_WAIT|DDBLTFAST_NOCOLORKEY);
	RealScreen -> Flip( NULL, DDFLIP_WAIT );

	while (1)
	{
        if ( PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE) )
        {
            if ( !GetMessage(&msg, NULL, 0, 0) ) 
				return msg.wParam;
            TranslateMessage( &msg );
            DispatchMessage( &msg );
        }
        else 
		{
			if(startindex!=0)
				GameProc();		                                //주 게임 루틴 호출.                  		
		}
    }
    DestroyWindow( MainHwnd );
    return TRUE;
}

////////////////////////////////////////////////Documention
/////////////////// MessageLoop ////////////////
////////////////////////////////////////////////

LRESULT WINAPI MessageLoop( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
{
    switch ( message )
    {
		
		case   WM_DESTROY      : ReleaseAll();PostQuitMessage( 0 );break;
		case   WM_LBUTTONDOWN  : MapSecond[MOUSEY][MOUSEX]=1;
								 MouseX = LOWORD(lParam);MouseY = HIWORD(lParam); chk2=1;
								 MOUSEX=MouseX>>SHIFT;		
								 MOUSEY=MouseY>>SHIFT;
								 
			//A*에 위한 길찾기 실행 
			if(Astar->NewPath(UNITS[0].Px,UNITS[0].Py,MouseX-=MouseX%TileSize,MouseY-=MouseY%TileSize ) )
			{
				Astar->PathNextNode();
				nextdestx=Astar->NodeGetX();
				nextdesty=Astar->NodeGetY();
			}	
			break;
		case   WM_RBUTTONDOWN  : startindex=1;	break;	 
		case   WM_KEYDOWN	   : chk2=0;
			UNITS[0].Px-=(UNITS[0].Px%TileSize),UNITS[0].Py-=(UNITS[0].Py%TileSize);
			//키보드 이동 타일사이즈 단위로 이동됨.
			switch(wParam)
			{
				case VK_ESCAPE:ReleaseAll();PostQuitMessage(0);break;
				case VK_SPACE: attack=1;HITCOUNT--;break;
				case VK_END  :
				case VK_HOME :
				case VK_UP   : UNITS[0].Py-=TileSize;	if(!CheckMap(UNITS[0].Px,UNITS[0].Py))		{	UNITS[0].Py+=TileSize;  }break;
				case VK_LEFT : UNITS[0].Px-=TileSize;	if(!CheckMap(UNITS[0].Px,UNITS[0].Py))		{	UNITS[0].Px+=TileSize;	}break;
				case VK_RIGHT: UNITS[0].Px+=TileSize;	if(!CheckMap(UNITS[0].Px,UNITS[0].Py))		{	UNITS[0].Px-=TileSize;	}break;
				case VK_DOWN : UNITS[0].Py+=TileSize;	if(!CheckMap(UNITS[0].Px,UNITS[0].Py))		{	UNITS[0].Py-=TileSize;	}break;
			}							
	}
    return DefWindowProc( hWnd, message, wParam, lParam );
}

/////////////////////////////////////////////////
//////////////// GameProc()/////// //////////////
/////////////////////////////////////////////////

void GameProc( void )
{   
	static int CHK,tmp1;
	static int FLAG=1;
	int reach=0;
	                   // 보여 주였던 바탕타일를 감춘다.  	
	if(!GAME_OVER_CHECK())  //장막의 상태를 비교하여 게임종료를 판단한다.
	{	
		//충돌검사(현좌표에서 다시 시작), 모든 적 유닛 동시 검사 
		if(!COLLISION(UNITS[0].Px,UNITS[0].Py,UNITS[2].Px,UNITS[2].Py,UNITS[1].Px,UNITS[1].Py))
		{	
			if(!(attack==1 && HITCOUNT > 0))
			{CollisionSprite();	GAME_AGAIN(0);	}
			GAME_AGAIN(2);
		}
		//충돌검사(처음부터 다시 시작),  모든  적 유닛 동시 검사   
		if(!COLLISION(UNITS[0].Px,UNITS[0].Py,UNITS[3].Px,UNITS[3].Py,UNITS[4].Px,UNITS[4].Py))
		{	
			if(!(attack==1 && HITCOUNT > 0))
			{CollisionSprite();	GAME_AGAIN(1);	}
			GAME_AGAIN(2);
			HITCOUNT=50;
		}
		//플레이어 유닛의 주변지역 검사 => 만약 적유닛이 있으면 회피하면서 이동좌표를 전환,목표이동  
		//[tmp : 1] 는 적 1유닛 ,[tmp : 2]는 적 2 유닛 
		int tmp=DISTANCE_COLLSION(UNITS[0].Px,UNITS[0].Py,UNITS[2].Px,UNITS[2].Py,UNITS[1].Px,UNITS[1].Py) ;  
		if(tmp==1 && attack == 0)
		{			
			int x1=UNITS[2].Px>>SHIFT;int y1=UNITS[2].Py>>SHIFT;
			//NewPath2()의 x1,y1는 적유닛의 자리로 막혀있는 타일로 설정됨.
			if(Astar->NewPath2(UNITS[0].Px,UNITS[0].Py,MouseX-=MouseX%TileSize,MouseY-=MouseY%TileSize ,x1,y1) )
			{
				Astar->PathNextNode();                  //이동될 댜음노드 읽어옴.
				nextdestx=Astar->NodeGetX();            //이동될 다음노드의 x 좌표 
				nextdesty=Astar->NodeGetY();            //이동될 다음노드의 y 좌표 
				chk2=1;                                 //플레이 유닛의 대각선 방향이동을 제한하기 위해 (chk2:1) 설정 
			}
		}
		if(tmp==2 && attack==0)
		{			
			int x1=UNITS[1].Px>>SHIFT;int y1=UNITS[1].Py>>SHIFT;  //각 좌표의 맵상의 타일번호를 구하기 위해서 타일사이즈로 나눈다.
			if(Astar->NewPath2(UNITS[0].Px,UNITS[0].Py,MouseX-=MouseX%TileSize,MouseY-=MouseY%TileSize ,x1,y1) )
			{
				Astar->PathNextNode();
				nextdestx=Astar->NodeGetX();
				nextdesty=Astar->NodeGetY();
				chk2=1;
			}
		}

		//플레이 유닛의 대각선 방향이동을 제한하기 위해 (chk2:1) 설정 
		//또한 처음부터 다시 시작하는 충돌발생시 유닛의 이동을 제한하기 의해 설정 
		if(chk2==1)
		{	
			//만약 다음노드에 도착하였다면 다음노드를 다시 읽어옴.
			if(!Astar->ReachedGoal() && nextdestx==UNITS[0].Px && nextdesty == UNITS[0].Py)
			{	Astar->PathNextNode();		nextdestx=Astar->NodeGetX();	nextdesty=Astar->NodeGetY();	}
						
			if(!Astar->ReachedGoal())                                                //0을 리턴하면 않되기 때문에 기입하였음.
			{
				if(nextdesty > UNITS[0].Py)				UNITS[0].Py++;               //유닛이 노드 위치보다  위에 있으면 
				if(nextdesty < UNITS[0].Py)				UNITS[0].Py--;		         //유닛이                아래에 있으면
				if(nextdestx > UNITS[0].Px)				UNITS[0].Px++;	             //유닛이                우측에 있으면 
				if(nextdestx < UNITS[0].Px)				UNITS[0].Px--;               //유닛이                좌측에 있으면  
				//이동명령이 있을 때만 비교계산 위해 설정 
				CHK=1;                                                               //노드에 도착하지 않으면 계속 1식 증가하기 위한 플래그. 
				                                                                     
			}
			else if(CHK==1)                                                          //이동명령일 때만 계산됨.
			{
				if(nextdesty > UNITS[0].Py)				UNITS[0].Py++;
				if(nextdesty < UNITS[0].Py)				UNITS[0].Py--;		
				if(nextdestx > UNITS[0].Px)				UNITS[0].Px++;	
				if(nextdestx < UNITS[0].Px)				UNITS[0].Px--;
			}
		}
		BackTileDraw(0);		//바탕화면 출력 
		Unit345Draw();			//인공지능 유닛
		Unit2ImageDraw();		//추적유닛 출력 
		Unit1ImageDraw();		//추적유닛 출력  
		 
		BackTileDraw(1);	    //검은 장막 
		UnitImageDraw();		//플레이어 출력 
		
	}
	else
	{	GAMEOVER();	}	
	RealScreen -> Flip( NULL, DDFLIP_WAIT );
}

////////////////////////////////////////
//////////////// Unit1ImageDraw() //////
////////////////////////////////////////       // 추적 유닛 출력 
void Unit1ImageDraw()
{
	static int Frame = 0;
	static int Count=0;	
	static int rr;
	static int tt;
	static int temp2;
	rr=UNITS[0].Px;	tt=UNITS[0].Py;           //임시변수에 현 유닛의 좌표 대입 
	if(temp%3)
	{
		if(Astar3->NewPath(UNITS[1].Px,UNITS[1].Py,rr-=(rr%TileSize),tt-=(tt%TileSize)))
		{	Astar3->PathNextNode();			nextx=Astar3->NodeGetX();			nexty=Astar3->NodeGetY();		}
		if(!Astar3->ReachedGoal() && nextx==UNITS[1].Px && nexty == UNITS[1].Py)
		{	Astar3->PathNextNode();			nextx=Astar3->NodeGetX();			nexty=Astar3->NodeGetY();		}
		if(nextx > UNITS[1].Px)				UNITS[1].Px++;	
		if(nexty < UNITS[1].Py)				UNITS[1].Py--;	
		if(nextx < UNITS[1].Px)				UNITS[1].Px--;	
		if(nexty > UNITS[1].Py)				UNITS[1].Py++;	
	}temp2++;
	
	//유닛 2 출력 
	RECT Srect,Drect;
	Srect.left     =   Frame * 100;
	Srect.top      =   0;
	Srect.right    =   Srect.left + 100;
	Srect.bottom   =   125;
	SetRect(&Drect,UNITS[1].Px,UNITS[1].Py,UNITS[1].Px+TileSize,UNITS[1].Py+TileSize);
	BackScreen->Blt(&Drect,BackImage2,&Srect,DDBLT_KEYSRC|DDBLT_WAIT,NULL);
			
	TCHAR temp[20];	
	int XX=UNITS[1].Px>>SHIFT;
	int YY=UNITS[1].Py>>SHIFT;
	wsprintf(temp, L" U1    %d %d",YY,XX);
	PaintText(556,48,temp);	     // 현 위치 좌표 출력, 화면상에 현 위치좌표 출력 
}

////////////////////////////////////////
//////////////// Unit345Draw() /////////
//////////////////////////////////////// // 교차이동 유닛 출력 
void Unit345Draw()
{
	static int Frame;
	static int Count=0;
	static int CHK=0;	
	if(CHK==0)
	{
		if(Count < TileSize*3)
		{	Count++;
				RECT Srect;
			SetRect(&Srect,0,0,16,16);
			BackScreen->BltFast(UNITS[3].Px,UNITS[3].Py--,Units,&Srect,DDBLTFAST_WAIT);	PaintText(UNITS[3].Px,UNITS[3].Py-10, L"unit3");
			
			SetRect(&Srect,0,0,16,16);
			BackScreen->BltFast(UNITS[4].Px,UNITS[4].Py++,Units,&Srect,DDBLTFAST_WAIT);	PaintText(UNITS[4].Px,UNITS[4].Py-10, L"unit4");
		}
		else
		{	CHK=1;				Count=0;		}		
	}
	else
	{
		if(Count < TileSize*3)          // y좌표는 3개(타일사이즈 * 3 )타일 계속 이동. 
		{	
			Count++;
		    RECT Srect;
			SetRect(&Srect,0,0,16,16);
			BackScreen->BltFast(UNITS[3].Px,UNITS[3].Py++,Units,&Srect,DDBLTFAST_WAIT);
			SetRect(&Srect,0,0,16,16);
			BackScreen->BltFast(UNITS[4].Px,UNITS[4].Py--,Units,&Srect,DDBLTFAST_WAIT);
		}
		else
		{			CHK=0;			Count=0;		}     //만약 Count가 타일사이즈*3보다 크다면 다시 왕복시키기 위해 초기화 설정.
	}
	
}

/////////////////////////////////////////////
///////////// UnitImageDraw() ///////////////
/////////////////////////////////////////////

void UnitImageDraw()
{
	static int Frame;
	static int Count;	
	TargetX=UNITS[0].Px;  //임시 변수에 대입 
	TargetY=UNITS[0].Py;  //입시 변수에 대입 
	
	//chk 가 1 이면 플레이어의 스프라이트 실행 
	if(attack) 
	{
		RECT Srect,Drect;
		Srect.left     =   Frame * 100;
		Srect.top      =   0;
		Srect.right    =   Srect.left + 100;
		Srect.bottom   =   70;
		SetRect(&Drect,TargetX,TargetY,TargetX+TileSize,TargetY+TileSize);
		BackScreen->Blt(&Drect,UnitImage2,&Srect,DDBLT_KEYSRC|DDBLT_WAIT,NULL);
		if ( Frame > 4 ) 
		{
			Frame = 0;	
			attack=0;
		}
		else
			Frame++;
	}
	else
	{
		RECT Srect,Drect;
			Srect.left     =   Frame * 100;
			Srect.top      =   0;
			Srect.right    =   Srect.left + 100;
			Srect.bottom   =   70;
			SetRect(&Drect,TargetX,TargetY,TargetX+TileSize,TargetY+TileSize);
			BackScreen->Blt(&Drect,UnitImage2,&Srect,DDBLT_KEYSRC|DDBLT_WAIT,NULL);        // 이미지 축소 출력 
	}
	
	//플레이어 주변 3개  타일의 장막을 거둔다.
	for(int c=UNITS[0].Py-TileSize*3 ; c<=UNITS[0].Py+TileSize*3 ; c++)
	for(int r=UNITS[0].Px-TileSize*3 ; r<=UNITS[0].Px+TileSize*3 ; r++)
	{
		int x1=c>>SHIFT;
		int y1=r>>SHIFT;
		if(x1 >= 0 && x1 < COLS && y1 >=0 && y1 < ROWS) 
			MapSecond[x1][y1]=0;                              // 장막이 없는 타일 대입 
	}
	MapSecond[MOUSEY][MOUSEX]=0;			                   // 바탕타일를 보여 준다.
	TCHAR temp[20],temp2[2],temp3[20],temp4[20],temp5[20];
    const TCHAR *games=L" ID";
	const TCHAR *title=L"X Y";
	const TCHAR *player=L"P";
	const TCHAR *hitcount=L"HITCount";

	int XX=UNITS[0].Px>>SHIFT;
	int YY=UNITS[0].Py>>SHIFT;

	wsprintf(temp2,L"%s",player); 
	if(HITCOUNT > 0)
	{
		wsprintf(temp5,L" %s:%d",hitcount,HITCOUNT);
		PaintText(556,80,temp5);
	}
	else
	{
		wsprintf(temp5,L" %s:%d",hitcount,HITCOUNT*=0);
		PaintText(556,80,temp5);
	}

	wsprintf(temp,L"             %s",title);
	wsprintf(temp3,L" P      %d %d",XX,YY);
	wsprintf(temp4,L"%s",games);

	PaintText(560,0,temp4);
	PaintText(545,16,temp);
	PaintText(556,32,temp3);
	
	PaintText(TargetX,TargetY-15,temp2);	                     // 현 위치 좌표 출력 


}

//////////////////////////////////////////////
//////////////// Unit2ImageDraw() ////////////
//////////////////////////////////////////////

void Unit2ImageDraw()
{
	static int Frame = 0;
	static int Count=0;	
	xx=UNITS[0].Px;	yy=UNITS[0].Py;                        //임시 변수에 현 유닛2의 좌표 대입 

	// 유닛의 이동속도를 50% 저하 시킴. 
	if(temp%2==0)
	{
		if(Astar2->NewPath(UNITS[2].Px,UNITS[2].Py,xx-=(xx%TileSize),yy-=(yy%TileSize)))
		{	Astar2->PathNextNode();			nx=Astar2->NodeGetX();			ny=Astar2->NodeGetY();		}
		if(!Astar2->ReachedGoal() && nx==UNITS[2].Px && ny == UNITS[2].Py)
		{	Astar2->PathNextNode();			nx=Astar2->NodeGetX();			ny=Astar2->NodeGetY();		}
		if(nx > UNITS[2].Px)				UNITS[2].Px++;	
		if(ny < UNITS[2].Py)				UNITS[2].Py--;	
		if(nx < UNITS[2].Px)				UNITS[2].Px--;	
		if(ny > UNITS[2].Py)				UNITS[2].Py++;		
	}
	temp++;
	//유닛 출력 
	
		RECT Srect,Drect;
		Srect.left     =   Frame * 100;
		Srect.top      =   0;
		Srect.right    =   Srect.left + 100;
		Srect.bottom   =   125;
		SetRect(&Drect,UNITS[2].Px,UNITS[2].Py,UNITS[2].Px+TileSize,UNITS[2].Py+TileSize);
		BackScreen->Blt(&Drect,BackImage2,&Srect,DDBLT_KEYSRC|DDBLT_WAIT,NULL);
	
	TCHAR temp[20];	
	int XX=UNITS[2].Px>>SHIFT;
	int YY=UNITS[2].Py>>SHIFT;
	wsprintf(temp,L" U2    %d %d",YY,XX); 
	PaintText(556,64,temp);	     // 현 위치 좌표 출력 
}


