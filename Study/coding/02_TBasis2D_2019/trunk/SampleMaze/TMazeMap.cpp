#include "TMazeMap.h"
#include <Time.h>


bool TMazeMap::CanMove(int x, int y)
{
	return (CanMoveUp(x, y) || CanMoveDown(x, y) ||
		CanMoveLeft(x, y) || CanMoveRight(x, y));
}

bool TMazeMap::CanMoveLeft(int x, int y)
{
	return (x > 0 && !m_pVisited[y*m_iWidth + x - 1]);
}

bool TMazeMap::CanMoveRight(int x, int y)
{
	return (x < (m_iWidth - 1) && !m_pVisited[y*m_iWidth + x + 1]);
}

bool TMazeMap::CanMoveUp(int x, int y)
{
	return (y > 0 && !m_pVisited[(y - 1)*m_iWidth + x]);
}

bool TMazeMap::CanMoveDown(int x, int y)
{
	return (y < (m_iHeight - 1) && !m_pVisited[(y + 1)*m_iWidth + x]);
}
TMazeMap::TMazeMap(int m_iWidth, int m_iHeight)
{
	this->m_iWidth = m_iWidth;
	this->m_iHeight = m_iHeight;
	m_pMazeBoxes = new MazeBoxInfo[m_iWidth*m_iHeight];
	m_pVisited = new bool[m_iWidth*m_iHeight];
	for (int ctr = 0; ctr < m_iWidth*m_iHeight; ctr++) m_pVisited[ctr] = false;

	srand((unsigned int)time(NULL));
	// initialize the maze
	Visit(0, 0);
	delete[] m_pVisited;
}
TMazeMap::TMazeMap()
{
	m_pMazeBoxes = 0;
	m_hBluePen = CreatePen(PS_SOLID, 10, RGB(0, 0, 255));
	m_hRedPen = CreatePen(PS_SOLID, 2, RGB(255, 0, 0));
}

TMazeMap::~TMazeMap()
{
	if (m_pMazeBoxes != NULL) delete[] m_pMazeBoxes;
	m_pMazeBoxes = 0;
}
void TMazeMap::Visit(int x, int y)
{
	m_pVisited[y*m_iWidth + x] = true;

	MazeBoxInfo &b = m_pMazeBoxes[y*m_iWidth + x];

	while (CanMove(x, y))
	{
		int movement = rand() % 4;

		switch (movement)
		{

		case 0:
			if (CanMoveRight(x, y))
			{
				b.setright(0);
				MazeBoxInfo &rightbox = m_pMazeBoxes[y*m_iWidth + x + 1];
				rightbox.SetLeft(0);
				Visit(x + 1, y);
			}
			break;


		case 1:
			if (CanMoveLeft(x, y))
			{
				b.SetLeft(0);
				MazeBoxInfo &leftbox = m_pMazeBoxes[y*m_iWidth + x - 1];
				leftbox.setright(0);
				Visit(x - 1, y);
			}
			break;


		case 2:
			if (CanMoveDown(x, y))
			{
				b.SetBottom(0);
				MazeBoxInfo &bottombox = m_pMazeBoxes[(y + 1)*m_iWidth + x];
				bottombox.SetTop(0);
				Visit(x, y + 1);
			}
			break;


		case 3:
			if (CanMoveUp(x, y))
			{
				b.SetTop(0);
				MazeBoxInfo &topbox = m_pMazeBoxes[(y - 1)*m_iWidth + x];
				topbox.SetBottom(0);
				Visit(x, y - 1);
			}
			break;
		}
	}
}
void TMazeMap::Release()
{
	if (m_pMazeBoxes != NULL) delete[] m_pMazeBoxes;
	m_pMazeBoxes = 0;

	SAFE_DELETE_ARRAY(m_pPloydVertexArray);
	SAFE_DELETE_ARRAY(m_pPloydVertex);
	SAFE_DELETE_ARRAY(m_pPloydPathList);

	m_Wall.Release();
	m_Road.Release();
	m_dwNumNodes = 0;

	DeleteObject(m_hBluePen);
	DeleteObject(m_hRedPen);
}
void TMazeMap::Reset()
{
	int iScreenWidth = g_rtClient.right - g_rtClient.left;
	int iScreenHeight = g_rtClient.bottom - g_rtClient.top;
	if (m_iScreenHeight == iScreenHeight && m_iScreenWidth == iScreenWidth) return;
	m_iScreenWidth = iScreenWidth;
	m_iScreenHeight = iScreenHeight;

	int iOffSetWidth = m_iScreenWidth / m_iMaxTileWidth;
	int iOffSetHeight = m_iScreenHeight / m_iMaxTileHeight;

	int iTileID = 0;
	float fHalf = (iOffSetHeight / 2.0f);
	for (int h = 0; h < m_iMaxTileHeight; h++)
	{
		for (int w = 0; w < m_iMaxTileWidth; w++)
		{
			TPoint pos(w * iOffSetWidth + fHalf, h * iOffSetHeight + fHalf);
			iTileID = h * m_iMaxTileWidth + w;
			m_TileData[iTileID].pos = pos;
		}
	}
}
void TMazeMap::Create(int iWidth, int iHeight, int iScreenWidth, int iScreenHeight)
{
	m_iScreenWidth = iScreenWidth;
	m_iScreenHeight = iScreenHeight;

	m_iWidth = iWidth;
	m_iHeight = iHeight;

	m_pMazeBoxes = new MazeBoxInfo[m_iWidth*m_iHeight];
	m_pVisited = new bool[m_iWidth*m_iHeight];
	for (int ctr = 0; ctr < m_iWidth*m_iHeight; ctr++) m_pVisited[ctr] = false;
	srand((unsigned int)time(NULL));
	Visit(0, 0);
	delete[] m_pVisited;

	MakePloydPass(m_pMazeBoxes, m_iScreenWidth, m_iScreenWidth);


	m_Wall.Init();
	m_Wall.Load(L"../../data/tile4.bmp");
	m_Road.Init();
	m_Road.Load(L"../../data/sand.bmp");

	CreateTile();
}
void TMazeMap::Render()
{
	int iOffSetWidth = m_iScreenWidth / m_iWidth;
	int iOffSetHeight = m_iScreenHeight / m_iHeight;

	//HBRUSH MyBrush = CreateHatchBrush(HS_BDIAGONAL, RGB(255, 255, 0));
	//HBRUSH OldBrush = (HBRUSH)SelectObject(g_hOffScreenDC, MyBrush);
	//HPEN MyPen = CreatePen(PS_SOLID, 10, RGB(0, 0, 255));
	HPEN OldPen = (HPEN)SelectObject(g_hOffScreenDC, m_hBluePen);

	for (int y = 0; y < m_iHeight; y++)
	{
		for (int x = 0; x < m_iWidth; x++)
		{
			MazeBoxInfo b = m_pMazeBoxes[y*m_iWidth + x];


			float x0 = (float)(x * iOffSetWidth);//float(x)/m_iWidth;
			float y0 = (float)(y * iOffSetHeight);//float(y)/m_iHeight;

			float x1 = (float)((x + 1) * iOffSetWidth);//float(x+1)/m_iWidth;
			float y1 = (float)((y + 1) * iOffSetHeight);//float(y+1)/m_iHeight;

														//SetROP2(g_hOffScreenDC, R2_NOTXORPEN);

			if (b.GetTop())
			{
				MoveToEx(g_hOffScreenDC, x0, y0, NULL);
				LineTo(g_hOffScreenDC, x1, y0);
			}
			if (b.GetBottom())
			{
				MoveToEx(g_hOffScreenDC, x0, y1, NULL);
				LineTo(g_hOffScreenDC, x1, y1);
			}
			if (b.getLeft())
			{
				MoveToEx(g_hOffScreenDC, x0, y0, NULL);
				LineTo(g_hOffScreenDC, x0, y1);
			}
			if (b.GetRight())
			{
				MoveToEx(g_hOffScreenDC, x1, y0, NULL);
				LineTo(g_hOffScreenDC, x1, y1);
			}
		}
	}
	//SelectObject(g_hOffScreenDC, OldBrush);
	SelectObject(g_hOffScreenDC, OldPen);
	//DeleteObject(MyBrush);

}
bool TMazeMap::RenderTile()
{
	int iOffSetWidth = g_rtClient.right / m_iMaxTileWidth;
	int iOffSetHeight = g_rtClient.bottom / m_iMaxTileHeight;

	for (int iHeight = 0;
		iHeight < m_iMaxTileHeight; iHeight++)
	{
		for (int iWidth = 0;
			iWidth < m_iMaxTileWidth; iWidth++)
		{
			int x = iWidth * iOffSetWidth;
			int y = iHeight * iOffSetHeight;
			int w = iOffSetWidth;
			int h = iOffSetHeight;
			int iIndex = iHeight * m_iMaxTileWidth + iWidth;
			if (m_TileData[iIndex].iType == 0)
			{
				StretchBlt(g_hOffScreenDC,
					x, y, w, h,
					m_Road.m_pImage->m_hMemDC,
					0, 0, m_Road.m_pImage->m_bmpInfo.bmWidth,
					m_Road.m_pImage->m_bmpInfo.bmHeight, SRCCOPY);
			}
			if (m_TileData[iIndex].iType == 1)
			{
				StretchBlt(g_hOffScreenDC,
					x, y, w, h,
					m_Wall.m_pImage->m_hMemDC,
					0, 0,
					m_Road.m_pImage->m_bmpInfo.bmWidth,
					m_Road.m_pImage->m_bmpInfo.bmHeight, SRCCOPY);
			}
		}
	}
	return true;
}



bool TMazeMap::CreateTile()
{
	m_iMaxTileWidth = m_iWidth * 2 + 1;
	m_iMaxTileHeight = m_iHeight * 2 + 1;
	m_TileData.resize(m_iMaxTileWidth*m_iMaxTileHeight);

	m_fTileDistance.x = g_rtClient.right / m_iMaxTileWidth;
	m_fTileDistance.y = g_rtClient.bottom / m_iMaxTileHeight;

	int iTileID = 0;
	float fHalf = (m_fTileDistance.y / 2.0f);
	for (int h = 0; h < m_iMaxTileHeight; h++)
	{
		for (int w = 0; w < m_iMaxTileWidth; w++)
		{
			TPoint pos(w * m_fTileDistance.x + fHalf, h * m_fTileDistance.y + fHalf);
			iTileID = h * m_iMaxTileWidth + w;
			m_TileData[iTileID].iType = 1;
			m_TileData[iTileID].iIndex = iTileID;
			m_TileData[iTileID].pos = pos;
		}
	}

	for (int h = 0; h < m_iHeight; h++)
	{
		for (int w = 0; w < m_iWidth; w++)
		{
			int iIndex = h * m_iWidth + w;
			int iX = w * 2 + 1;
			int iY = h * 2 + 1;
			int iMapIndex = iY * m_iMaxTileWidth + iX;
			m_TileData[iMapIndex].iType = 0;

			if (m_pMazeBoxes[iIndex].top == 0)
			{
				iMapIndex = (iY - 1) * m_iMaxTileWidth + iX;
				m_TileData[iMapIndex].iType = 0;
			}
			if (m_pMazeBoxes[iIndex].left == 0)
			{
				iMapIndex = iY * m_iMaxTileWidth + iX - 1;
				m_TileData[iMapIndex].iType = 0;
			}
			if (m_pMazeBoxes[iIndex].right == 0)
			{
				iMapIndex = iY * m_iMaxTileWidth + iX + 1;
				m_TileData[iMapIndex].iType = 0;
			}
			if (m_pMazeBoxes[iIndex].bottom == 0)
			{
				iMapIndex = (iY + 1) * m_iMaxTileWidth + iX;
				m_TileData[iMapIndex].iType = 0;
			}
		}
	}
	return true;
}


TIndex TMazeMap::GetTileID(float x, float y)
{
	int iTileIndex = GetPloydIndex(x, y);
	int iX = iTileIndex % m_iHeight;
	int iY = iTileIndex / m_iHeight;
	iX = iX * 2 + 1;
	iY = iY * 2 + 1;
	return { iX, iY };
}
// �÷��̵� �������� �ε����� ����Ͽ� ��ȯ�Ѵ�.
int TMazeMap::GetPloydIndex(float fX, float fY)
{
	int iFindIndex;
	int iOffSetWidth = g_rtClient.right / m_iWidth;
	int iOffSetHeight = g_rtClient.bottom / m_iHeight;
	int w = fX / iOffSetWidth;
	int h = fY / iOffSetHeight;
	iFindIndex = h * m_iWidth + w;
	return iFindIndex;
}
void TMazeMap::RenderPath()
{
	HPEN OldPen = (HPEN)SelectObject(g_hOffScreenDC, m_hRedPen);
	TPoint vStart = { 0,0 };
	for (int iCnt = 0; iCnt< m_PathList.size(); iCnt++)
	{
		MoveToEx(g_hOffScreenDC, vStart.x, vStart.y, NULL);
		LineTo(g_hOffScreenDC, m_PathList[iCnt].x, m_PathList[iCnt].y);
		vStart = m_PathList[iCnt];
	}
	SelectObject(g_hOffScreenDC, OldPen);
}
vector<TPoint>&	TMazeMap::PloydListToTileList(DWORD dwStart, DWORD dwEnd)
{
	m_PathList.clear();
	TPloydPath* pPloydPath = GetPloydPathList(dwStart, dwEnd);
	for (int iPath = 0; iPath < pPloydPath->m_VertexList.size(); iPath++)
	{
		int iNode = pPloydPath->m_VertexList[iPath];

		int iX = iNode % m_iHeight;
		int iY = iNode / m_iHeight;
		iX = iX * 2 + 1;
		iY = iY * 2 + 1;
		m_PathList.push_back(m_TileData[iY*m_iMaxTileWidth + iX].pos);
	}
	return m_PathList;
}