#pragma once
#include "TStd.h"
#include "TObject.h"
#include "TAStar.h"
#include <bitset>
using namespace AStar;

struct TPloydPath
{
	DWORD				m_dwCost;
	vector<DWORD>		m_VertexList;
};
struct TPloydVertex
{
	POINT			m_vVertex;
};

struct MazeBoxInfo
{
	unsigned left : 1;
	unsigned right : 1;
	unsigned top : 1;
	unsigned bottom : 1;

	MazeBoxInfo()
	{
		left = 1;
		right = 1;
		top = 1;
		bottom = 1;
	}
	void SetTop(int value) { top = value; }
	void SetBottom(int value) { bottom = value; }
	void SetLeft(int value) { left = value; }
	void setright(int value) { right = value; }
	int GetRight() { return right; }
	int getLeft() { return left; }
	int GetTop() { return top; }
	int GetBottom() { return bottom; }
};
struct TTile
{
	int iIndex;
	int iType;
	TPoint pos;
};
class  TMazeMap
{
public:
	HPEN		m_hBluePen;
	HPEN		m_hRedPen;
	TObject		m_Wall;
	TObject		m_Road;
	MazeBoxInfo * m_pMazeBoxes;
	vector<TTile>	m_TileData;
	bool*			m_pVisited;
	int				m_iWidth;
	int				m_iHeight;
	TPoint			m_fTileDistance;
	int				m_iMaxTileWidth;
	int				m_iMaxTileHeight;
public:
	int				m_iScreenWidth;
	int				m_iScreenHeight;
	DWORD			m_dwNumNodes;
	WORD*			m_pPloydVertexArray;
	TPloydPath*		m_pPloydPathList;
	TPloydVertex*	m_pPloydVertex;
	vector<TPoint>	m_PathList;
public:
	bool CanMove(int x, int y);
	bool CanMoveLeft(int x, int y);
	bool CanMoveRight(int x, int y);
	bool CanMoveUp(int x, int y);
	bool CanMoveDown(int x, int y);
	void Visit(int x, int y);
public:
	bool			MakePloydPass();
	bool			ComputePloydPass();
	TPloydPath*		GetPloydPathList(DWORD dwStart, DWORD dwEnd);
	void			Create(int m_iWidth, int m_iHeight, int iScreenWidth, int iScreenHeight);
	void			Render();
	void			Release();
	void			Reset();
	bool			CreateTile();
	bool			RenderTile();
	void			RenderPath();
	int				GetPloydIndex(float fX, float fY);
	TIndex			GetTileID(float x, float y);
	vector<TPoint>&	PloydListToTileList(DWORD dwStart, DWORD dwEnd);
	TIndex   GetTileIndex(TPoint p);
	TIndex   GetTileIndex(POINT p);
	TPoint   GetTilePos(TIndex p);
	bool     IsRoad(TIndex p);
public:
	TMazeMap(int m_iWidth, int m_iHeight);
	TMazeMap();
	virtual ~TMazeMap();
};
