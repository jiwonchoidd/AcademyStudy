#pragma once
#include "TPloyd.h"
#include "TObject.h"

struct TTile
{
	int iIndex;
	int iType;
	TPoint pos;
};

class  TMazeMap : public TPloyd
{
public:
	HPEN			m_hBluePen;
	HPEN			m_hRedPen;
	TObject			m_Wall;
	TObject			m_Road;
	bool*			m_pVisited;
	TPoint			m_fTileDistance;
	int				m_iScreenWidth;
	int				m_iScreenHeight;
	int				m_iMaxTileWidth;
	int				m_iMaxTileHeight;
	MazeBoxInfo *	m_pMazeBoxes;
	vector<TTile>	m_TileData;
public:
	bool CanMove(int x, int y);
	bool CanMoveLeft(int x, int y);
	bool CanMoveRight(int x, int y);
	bool CanMoveUp(int x, int y);
	bool CanMoveDown(int x, int y);
	void Visit(int x, int y);
public:	
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
public:
	TMazeMap(int m_iWidth, int m_iHeight);
	TMazeMap();
	virtual ~TMazeMap();
};
