/////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////// AI.CPP /////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////
#include <windows.h>
#include <windowsx.h>
#include <ddraw.h>
#include <stdio.h>
#include "AI.h"
#include "Ddutil.h"
#define  SHIFT 4                 //Ÿ�� ������ 16



///////////////////////////////////////////////////
////////////////// WinMain() //////////////////////
///////////////////////////////////////////////////
int PASCAL WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow )
{
    int index=0;
	MSG msg;
  	if ( !GameMode(hInstance, nCmdShow, 640, 480, 16) ) 
		return FALSE;	
	UnitImage=DDLoadBitmap(DirectOBJ,L"tile2.bmp",0,0);        //����Ÿ�� 
	BackImage=DDLoadBitmap(DirectOBJ, L"tile4.bmp",0,0);        //����Ÿ��  
	BackImage2=DDLoadBitmap(DirectOBJ, L"Scar.bmp",0,0);        //�� ���� 1,2
	Units=DDLoadBitmap(DirectOBJ, L"sand.bmp",0,0);
	UnitImage2=DDLoadBitmap(DirectOBJ, L"PlayerUnit.bmp",0,0);  //�÷��̾� ���� 
	EndImage=DDLoadBitmap(DirectOBJ, L"end.bmp",0,0);
	StartImage=DDLoadBitmap(DirectOBJ, L"start.bmp",0,0);

	DDSetColorKey(UnitImage,RGB(0,0,0));	                  //����� ���� (������)
	DDSetColorKey(UnitImage2,RGB(0,0,0));
	DDSetColorKey(BackImage2,RGB(0,0,0));
	
	Astar=new ASTAR(Tile_Map,COLS,ROWS);
	Astar2=new ASTAR(Tile_Map,COLS,ROWS);
	Astar3=new ASTAR(Tile_Map,COLS,ROWS);
	
	INIT_UNITS_POINT_XY();									//��� ������ �ʱ� ��ǥ ���� 
	InitCreate(0,0,0);	                                    //ó�� �÷��̾��� �̵� ��ǥ �������� ����.
	                                                        //���� �����ϸ� �ʱ� ��ǥ�� �����Ǿ� ������ �̵���.
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
				GameProc();		                                //�� ���� ��ƾ ȣ��.                  		
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
								 
			//A*�� ���� ��ã�� ���� 
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
			//Ű���� �̵� Ÿ�ϻ����� ������ �̵���.
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
	                   // ���� �ֿ��� ����Ÿ�ϸ� �����.  	
	if(!GAME_OVER_CHECK())  //�帷�� ���¸� ���Ͽ� �������Ḧ �Ǵ��Ѵ�.
	{	
		//�浹�˻�(����ǥ���� �ٽ� ����), ��� �� ���� ���� �˻� 
		if(!COLLISION(UNITS[0].Px,UNITS[0].Py,UNITS[2].Px,UNITS[2].Py,UNITS[1].Px,UNITS[1].Py))
		{	
			if(!(attack==1 && HITCOUNT > 0))
			{CollisionSprite();	GAME_AGAIN(0);	}
			GAME_AGAIN(2);
		}
		//�浹�˻�(ó������ �ٽ� ����),  ���  �� ���� ���� �˻�   
		if(!COLLISION(UNITS[0].Px,UNITS[0].Py,UNITS[3].Px,UNITS[3].Py,UNITS[4].Px,UNITS[4].Py))
		{	
			if(!(attack==1 && HITCOUNT > 0))
			{CollisionSprite();	GAME_AGAIN(1);	}
			GAME_AGAIN(2);
			HITCOUNT=50;
		}
		//�÷��̾� ������ �ֺ����� �˻� => ���� �������� ������ ȸ���ϸ鼭 �̵���ǥ�� ��ȯ,��ǥ�̵�  
		//[tmp : 1] �� �� 1���� ,[tmp : 2]�� �� 2 ���� 
		int tmp=DISTANCE_COLLSION(UNITS[0].Px,UNITS[0].Py,UNITS[2].Px,UNITS[2].Py,UNITS[1].Px,UNITS[1].Py) ;  
		if(tmp==1 && attack == 0)
		{			
			int x1=UNITS[2].Px>>SHIFT;int y1=UNITS[2].Py>>SHIFT;
			//NewPath2()�� x1,y1�� �������� �ڸ��� �����ִ� Ÿ�Ϸ� ������.
			if(Astar->NewPath2(UNITS[0].Px,UNITS[0].Py,MouseX-=MouseX%TileSize,MouseY-=MouseY%TileSize ,x1,y1) )
			{
				Astar->PathNextNode();                  //�̵��� ������� �о��.
				nextdestx=Astar->NodeGetX();            //�̵��� ��������� x ��ǥ 
				nextdesty=Astar->NodeGetY();            //�̵��� ��������� y ��ǥ 
				chk2=1;                                 //�÷��� ������ �밢�� �����̵��� �����ϱ� ���� (chk2:1) ���� 
			}
		}
		if(tmp==2 && attack==0)
		{			
			int x1=UNITS[1].Px>>SHIFT;int y1=UNITS[1].Py>>SHIFT;  //�� ��ǥ�� �ʻ��� Ÿ�Ϲ�ȣ�� ���ϱ� ���ؼ� Ÿ�ϻ������ ������.
			if(Astar->NewPath2(UNITS[0].Px,UNITS[0].Py,MouseX-=MouseX%TileSize,MouseY-=MouseY%TileSize ,x1,y1) )
			{
				Astar->PathNextNode();
				nextdestx=Astar->NodeGetX();
				nextdesty=Astar->NodeGetY();
				chk2=1;
			}
		}

		//�÷��� ������ �밢�� �����̵��� �����ϱ� ���� (chk2:1) ���� 
		//���� ó������ �ٽ� �����ϴ� �浹�߻��� ������ �̵��� �����ϱ� ���� ���� 
		if(chk2==1)
		{	
			//���� ������忡 �����Ͽ��ٸ� ������带 �ٽ� �о��.
			if(!Astar->ReachedGoal() && nextdestx==UNITS[0].Px && nextdesty == UNITS[0].Py)
			{	Astar->PathNextNode();		nextdestx=Astar->NodeGetX();	nextdesty=Astar->NodeGetY();	}
						
			if(!Astar->ReachedGoal())                                                //0�� �����ϸ� �ʵǱ� ������ �����Ͽ���.
			{
				if(nextdesty > UNITS[0].Py)				UNITS[0].Py++;               //������ ��� ��ġ����  ���� ������ 
				if(nextdesty < UNITS[0].Py)				UNITS[0].Py--;		         //������                �Ʒ��� ������
				if(nextdestx > UNITS[0].Px)				UNITS[0].Px++;	             //������                ������ ������ 
				if(nextdestx < UNITS[0].Px)				UNITS[0].Px--;               //������                ������ ������  
				//�̵������ ���� ���� �񱳰�� ���� ���� 
				CHK=1;                                                               //��忡 �������� ������ ��� 1�� �����ϱ� ���� �÷���. 
				                                                                     
			}
			else if(CHK==1)                                                          //�̵������ ���� ����.
			{
				if(nextdesty > UNITS[0].Py)				UNITS[0].Py++;
				if(nextdesty < UNITS[0].Py)				UNITS[0].Py--;		
				if(nextdestx > UNITS[0].Px)				UNITS[0].Px++;	
				if(nextdestx < UNITS[0].Px)				UNITS[0].Px--;
			}
		}
		BackTileDraw(0);		//����ȭ�� ��� 
		Unit345Draw();			//�ΰ����� ����
		Unit2ImageDraw();		//�������� ��� 
		Unit1ImageDraw();		//�������� ���  
		 
		BackTileDraw(1);	    //���� �帷 
		UnitImageDraw();		//�÷��̾� ��� 
		
	}
	else
	{	GAMEOVER();	}	
	RealScreen -> Flip( NULL, DDFLIP_WAIT );
}

////////////////////////////////////////
//////////////// Unit1ImageDraw() //////
////////////////////////////////////////       // ���� ���� ��� 
void Unit1ImageDraw()
{
	static int Frame = 0;
	static int Count=0;	
	static int rr;
	static int tt;
	static int temp2;
	rr=UNITS[0].Px;	tt=UNITS[0].Py;           //�ӽú����� �� ������ ��ǥ ���� 
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
	
	//���� 2 ��� 
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
	PaintText(556,48,temp);	     // �� ��ġ ��ǥ ���, ȭ��� �� ��ġ��ǥ ��� 
}

////////////////////////////////////////
//////////////// Unit345Draw() /////////
//////////////////////////////////////// // �����̵� ���� ��� 
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
		if(Count < TileSize*3)          // y��ǥ�� 3��(Ÿ�ϻ����� * 3 )Ÿ�� ��� �̵�. 
		{	
			Count++;
		    RECT Srect;
			SetRect(&Srect,0,0,16,16);
			BackScreen->BltFast(UNITS[3].Px,UNITS[3].Py++,Units,&Srect,DDBLTFAST_WAIT);
			SetRect(&Srect,0,0,16,16);
			BackScreen->BltFast(UNITS[4].Px,UNITS[4].Py--,Units,&Srect,DDBLTFAST_WAIT);
		}
		else
		{			CHK=0;			Count=0;		}     //���� Count�� Ÿ�ϻ�����*3���� ũ�ٸ� �ٽ� �պ���Ű�� ���� �ʱ�ȭ ����.
	}
	
}

/////////////////////////////////////////////
///////////// UnitImageDraw() ///////////////
/////////////////////////////////////////////

void UnitImageDraw()
{
	static int Frame;
	static int Count;	
	TargetX=UNITS[0].Px;  //�ӽ� ������ ���� 
	TargetY=UNITS[0].Py;  //�Խ� ������ ���� 
	
	//chk �� 1 �̸� �÷��̾��� ��������Ʈ ���� 
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
			BackScreen->Blt(&Drect,UnitImage2,&Srect,DDBLT_KEYSRC|DDBLT_WAIT,NULL);        // �̹��� ��� ��� 
	}
	
	//�÷��̾� �ֺ� 3��  Ÿ���� �帷�� �ŵд�.
	for(int c=UNITS[0].Py-TileSize*3 ; c<=UNITS[0].Py+TileSize*3 ; c++)
	for(int r=UNITS[0].Px-TileSize*3 ; r<=UNITS[0].Px+TileSize*3 ; r++)
	{
		int x1=c>>SHIFT;
		int y1=r>>SHIFT;
		if(x1 >= 0 && x1 < COLS && y1 >=0 && y1 < ROWS) 
			MapSecond[x1][y1]=0;                              // �帷�� ���� Ÿ�� ���� 
	}
	MapSecond[MOUSEY][MOUSEX]=0;			                   // ����Ÿ�ϸ� ���� �ش�.
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
	
	PaintText(TargetX,TargetY-15,temp2);	                     // �� ��ġ ��ǥ ��� 


}

//////////////////////////////////////////////
//////////////// Unit2ImageDraw() ////////////
//////////////////////////////////////////////

void Unit2ImageDraw()
{
	static int Frame = 0;
	static int Count=0;	
	xx=UNITS[0].Px;	yy=UNITS[0].Py;                        //�ӽ� ������ �� ����2�� ��ǥ ���� 

	// ������ �̵��ӵ��� 50% ���� ��Ŵ. 
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
	//���� ��� 
	
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
	PaintText(556,64,temp);	     // �� ��ġ ��ǥ ��� 
}


