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

	MakePloydPass();


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

bool TMazeMap::MakePloydPass()
{
	m_dwNumNodes = m_iWidth * m_iHeight;
	// 전체 지형에 선정된 플로이드 정점
	SAFE_NEW_ARRAY(m_pPloydVertex, TPloydVertex, m_dwNumNodes);
	// 전체 플로이드정점의 2차원 배열
	SAFE_NEW_ARRAY(m_pPloydVertexArray, WORD, m_dwNumNodes *  m_dwNumNodes);
	memset(m_pPloydVertexArray, 0, sizeof(WORD) * m_dwNumNodes * m_dwNumNodes);
	// 플로이드 정점의 2차원배열을 사용하여 실제적인 길찾기 정보를 저장한다.
	SAFE_NEW_ARRAY(m_pPloydPathList, TPloydPath, m_dwNumNodes*m_dwNumNodes);

	int iOffSetWidth = m_iScreenWidth / m_iWidth;
	int iOffSetHeight = m_iScreenHeight / m_iHeight;

	int iPloydVertexCnt = 0;
	float x0, x1, y0, y1;
	for (int y = 0; y < m_iHeight; y++)
	{
		for (int x = 0; x < m_iWidth; x++)
		{
			// 각 플로이드 정점의 위치를 저장한다.
			x0 = (float)(x * iOffSetWidth);
			y0 = (float)(y * iOffSetHeight);
			x1 = (float)((x + 1) * iOffSetWidth);
			y1 = (float)((y + 1) * iOffSetHeight);
			m_pPloydVertex[iPloydVertexCnt].m_vVertex.x = (x1 + x0) / 2.0f;
			m_pPloydVertex[iPloydVertexCnt++].m_vVertex.y = (y1 + y0) / 2.0f;

			// 4 방향의 이동 가능 여부를 저장한다.
			int iIndex = y * m_iWidth + x;
			// iIndex에 해당하는 모든 2차원 배열을 체운다.
			int iArrayIndex = iIndex * (m_iWidth*m_iHeight);
			MazeBoxInfo MazeBox = m_pMazeBoxes[iIndex];
			if (x == y)
			{
				m_pPloydVertexArray[iArrayIndex] = 1;
			}
			if (MazeBox.GetTop() == 0)
			{
				// [ iArrayIndex 번 정점] + [4방향 중에 갈수 있는 플로이드 정점인덱스] = 1 
				m_pPloydVertexArray[iArrayIndex + iIndex - m_iWidth] = 1;
			}
			////////////////////////////////////////////////////////////////////////////////////////		
			if (MazeBox.GetBottom() == 0)
			{
				m_pPloydVertexArray[iArrayIndex + iIndex + m_iWidth] = 1;
			}
			///////////////////////////////////////////////////////////////////////////////////////////
			if (MazeBox.getLeft() == 0)
			{
				m_pPloydVertexArray[iArrayIndex + iIndex - 1] = 1;
			}
			/////////////////////////////////////////////////////////////////////////////////////////
			if (MazeBox.GetRight() == 0)
			{
				m_pPloydVertexArray[iArrayIndex + iIndex + 1] = 1;
			}
		}
	}

	ComputePloydPass();
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

bool TMazeMap::ComputePloydPass()
{
	DWORD i, j, k;
	// 이동가능한 경로를 저장한다.
	for (i = 0; i<m_dwNumNodes; i++)
	{
		for (j = 0; j<m_dwNumNodes; j++)
		{
			if (m_pPloydVertexArray[i * m_dwNumNodes + j] == 0)
			{
				m_pPloydPathList[i*m_dwNumNodes + j].m_dwCost = 9999;
			}
			else
			{
				m_pPloydPathList[i*m_dwNumNodes + j].m_dwCost = 1;
				m_pPloydPathList[i*m_dwNumNodes + j].m_VertexList.push_back(j);
			}
		}
	}

	for (k = 0; k<m_dwNumNodes; k++)
	{
		for (i = 0; i<m_dwNumNodes; i++)
		{
			for (j = 0; j<m_dwNumNodes; j++)
			{
				// 빠른 길이 있으면 그 길을 선택한다.
				// 이전 노드와 현재의 합이 누적된 노드의 값보다 작으면 선택한다.
				int iNewCost = m_pPloydPathList[i*m_dwNumNodes + k].m_dwCost + m_pPloydPathList[k*m_dwNumNodes + j].m_dwCost;
				if (iNewCost < m_pPloydPathList[i*m_dwNumNodes + j].m_dwCost)
				{
					m_pPloydPathList[i*m_dwNumNodes + j].m_dwCost = iNewCost;
					// 이동경로가 누적되어 저장된다.          
					m_pPloydPathList[i*m_dwNumNodes + j].m_VertexList.assign(
						m_pPloydPathList[i*m_dwNumNodes + k].m_VertexList.begin(),
						m_pPloydPathList[i*m_dwNumNodes + k].m_VertexList.end());

					m_pPloydPathList[i*m_dwNumNodes + j].m_VertexList.insert(
						m_pPloydPathList[i*m_dwNumNodes + j].m_VertexList.end(),
						m_pPloydPathList[k*m_dwNumNodes + j].m_VertexList.begin(),
						m_pPloydPathList[k*m_dwNumNodes + j].m_VertexList.end());
				}
			}
		}
	}
	return true;
}
TPloydPath*	TMazeMap::GetPloydPathList(DWORD dwStart, DWORD dwEnd)
{
	return &m_pPloydPathList[dwStart*m_dwNumNodes + dwEnd];
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

void TMazeMap::RenderPath()
{
	if (m_PathList.size() <= 0) return;
	HPEN OldPen = (HPEN)SelectObject(g_hOffScreenDC, m_hRedPen);
	TPoint vStart = { m_PathList[0].x, m_PathList[0].y };
	for (int iCnt = 1; iCnt< m_PathList.size(); iCnt++)
	{
		MoveToEx(g_hOffScreenDC, vStart.x, vStart.y, NULL);
		LineTo(g_hOffScreenDC, m_PathList[iCnt].x, m_PathList[iCnt].y);
		vStart = m_PathList[iCnt];
	}
	SelectObject(g_hOffScreenDC, OldPen);
}
bool     TMazeMap::IsRoad(TIndex s)
{
	if (!(s.x >= 0 && s.x < m_iMaxTileWidth && s.y >= 0 && s.y < m_iMaxTileWidth))
	{
		return false;
	}
	if (m_TileData[s.y * m_iMaxTileWidth + s.x].iType == 0)
	{
		return true;
	}
	return false;
}
TIndex   TMazeMap::GetTileIndex(TPoint p)
{
	TIndex ret;
	ret.x = p.x / (g_rtClient.right / m_iMaxTileWidth);
	ret.y = p.y / (g_rtClient.bottom / m_iMaxTileHeight);
	return ret;
}
TIndex   TMazeMap::GetTileIndex(POINT p)
{
	TIndex ret;
	ret.x = p.x / (g_rtClient.right / m_iMaxTileWidth);
	ret.y = p.y / (g_rtClient.bottom / m_iMaxTileHeight);
	return ret;
}
TPoint   TMazeMap::GetTilePos(TIndex p)
{
	TPoint ret;
	float fHalfW = (g_rtClient.right / m_iMaxTileWidth);
	float fHalfH = (g_rtClient.bottom / m_iMaxTileHeight);
	ret.x = p.x * fHalfW + (fHalfW / 2);
	ret.y = p.y * fHalfH + (fHalfH / 2);
	return ret;
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
// 플로이드 영역에서 인덱스를 계산하여 반환한다.
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

