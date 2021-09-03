#pragma once
#include "TMazeBoxInfo.h"

struct TPloydPath
{
	DWORD				m_dwCost;
	vector<DWORD>		m_VertexList;
};

struct TPloydVertex
{
	POINT			m_vVertex;
};

struct TNodeVertex
{
	int      iCost;	
	std::vector<TIndex> path;
};
using TPloydArray = std::vector<TNodeVertex>;

class TPloyd
{
public:
	int				m_iWidth;
	int				m_iHeight;
	DWORD			m_dwNumNodes;	
public:
	WORD*			m_pPloydVertexArray;
	TPloydPath*		m_pPloydPathList;
	TPloydVertex*	m_pPloydVertex;
	vector<TPoint>	m_PathList;
public:
	bool			MakePloydPass(MazeBoxInfo* pMazeBoxes, int iScreenWidth, int iScreenHeight);
	bool			ComputePloydPass();
	TPloydPath*		GetPloydPathList(DWORD dwStart, DWORD dwEnd);	
public:
	std::vector< TPloydArray >	m_vertexArray;
	bool						TilePolydPath();
	bool						IsRoad(TIndex s);
public:
	TPloyd();
	virtual ~TPloyd();
};

