#pragma once
#include "TCore.h"
#include "TBitmap.h"
#include "AI.h"

class Sample : public TCore
{
public:
	TBitmap		m_Bitmap;
	tPOINT		m_ObjPos;
	RECT		m_rtObj;
	HBRUSH		m_hbrBlack;
public:
	bool		Init()
	{
		//int index = 0;
		//MSG msg;
		//if (!GameMode()) return false;
		//UnitImage = DDLoadBitmap(DirectOBJ, "tile2.bmp", 0, 0);        //����Ÿ�� 
		//BackImage = DDLoadBitmap(DirectOBJ, "tile4.bmp", 0, 0);        //����Ÿ��  
		//BackImage2 = DDLoadBitmap(DirectOBJ, "Scar.bmp", 0, 0);        //�� ���� 1,2
		//Units = DDLoadBitmap(DirectOBJ, "sand.bmp", 0, 0);
		//UnitImage2 = DDLoadBitmap(DirectOBJ, "PlayerUnit.bmp", 0, 0);  //�÷��̾� ���� 
		//EndImage = DDLoadBitmap(DirectOBJ, "end.bmp", 0, 0);
		//StartImage = DDLoadBitmap(DirectOBJ, "start.bmp", 0, 0);

		//DDSetColorKey(UnitImage, RGB(0, 0, 0));	                  //����� ���� (������)
		//DDSetColorKey(UnitImage2, RGB(0, 0, 0));
		//DDSetColorKey(BackImage2, RGB(0, 0, 0));

		//Astar = new ASTAR(Tile_Map, COLS, ROWS);
		//Astar2 = new ASTAR(Tile_Map, COLS, ROWS);
		//Astar3 = new ASTAR(Tile_Map, COLS, ROWS);

		//INIT_UNITS_POINT_XY();									//��� ������ �ʱ� ��ǥ ���� 
		//InitCreate(0, 0, 0);	                                    //ó�� �÷��̾��� �̵� ��ǥ �������� ����.
		//															//���� �����ϸ� �ʱ� ��ǥ�� �����Ǿ� ������ �̵���.
		//RECT BackRect = { 0,0,640,480 };
		//BackScreen->BltFast(0, 0, StartImage, &BackRect, DDBLTFAST_WAIT | DDBLTFAST_NOCOLORKEY);
		//RealScreen->Flip(NULL, DDFLIP_WAIT);
	};
	bool		Frame()
	{
		if (I_Input.m_dwMouseState[0] == KEY_PUSH)
		{
			MapSecond[MOUSEY][MOUSEX] = 1;
			MouseX = I_Input.m_MousePos.x; 
			MouseY = I_Input.m_MousePos.y;
			chk2 = 1;
			MOUSEX = MouseX >> SHIFT;
			MOUSEY = MouseY >> SHIFT;

			//A*�� ���� ��ã�� ���� 
			if (Astar->NewPath(UNITS[0].Px, UNITS[0].Py, MouseX -= MouseX % TileSize, MouseY -= MouseY % TileSize))
			{
				Astar->PathNextNode();
				nextdestx = Astar->NodeGetX();
				nextdesty = Astar->NodeGetY();
			}
		}
		
		if (I_Input.m_dwMouseState[1] == KEY_PUSH)
		{
			startindex = 1;
		}
		
		chk2 = 0;
		UNITS[0].Px -= (UNITS[0].Px%TileSize);
		UNITS[0].Py -= (UNITS[0].Py%TileSize);

		if (I_Input.KeyCheck(VK_SPACE) == KEY_PUSH)
		{
			attack = 1;
			HITCOUNT--;
		}
		if (I_Input.KeyCheck(VK_END) == KEY_PUSH)
		{
			UNITS[0].Py -= TileSize;	if (!CheckMap(UNITS[0].Px, UNITS[0].Py)) { UNITS[0].Py += TileSize; }
		}
		if (I_Input.KeyCheck(VK_HOME) == KEY_PUSH)
		{
			UNITS[0].Py -= TileSize;	if (!CheckMap(UNITS[0].Px, UNITS[0].Py)) { UNITS[0].Py += TileSize; }
		}
		if (I_Input.KeyCheck(VK_UP) == KEY_PUSH)
		{
			UNITS[0].Py -= TileSize;	if (!CheckMap(UNITS[0].Px, UNITS[0].Py)) { UNITS[0].Py += TileSize; }
		}
		if (I_Input.KeyCheck(VK_LEFT) == KEY_PUSH)
		{
			UNITS[0].Px -= TileSize;	if (!CheckMap(UNITS[0].Px, UNITS[0].Py)) { UNITS[0].Px += TileSize; }
		}
		if (I_Input.KeyCheck(VK_RIGHT) == KEY_PUSH)
		{
			UNITS[0].Px += TileSize;	if (!CheckMap(UNITS[0].Px, UNITS[0].Py)) { UNITS[0].Px -= TileSize; }
		}
		if (I_Input.KeyCheck(VK_DOWN) == KEY_PUSH)
		{
			UNITS[0].Py += TileSize;	if (!CheckMap(UNITS[0].Px, UNITS[0].Py)) { UNITS[0].Py -= TileSize; }
		}	
	
	};
	bool		Render() { return true; };
	bool		Release()
	{
		ReleaseAll();
		return true;
	};
public:
	Sample();
	virtual ~Sample();
};
TCORE_RUN(SampleAStarDemo, 800, 600);
