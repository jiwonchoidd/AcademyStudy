#pragma once
#include "TCore.h"
#include "TMazeMap.h"
#include "TCollision.h"
#include <iostream>
//#define TEST
#ifndef TEST

class Sample : public TCore
{
public:
	TMazeMap   m_Map;
	TObject    m_User;
	TPloydPath* m_pTargetPath;
	AStar::Generator generator;
	int m_iCurrentPathIndex;
public:
	bool		Init() {
		m_Map.Create(10, 10, m_rtClient.right, m_rtClient.bottom);
		TIndex start = m_Map.GetTileID(0, 0);
		int iTile = start.y * m_Map.m_iMaxTileWidth + start.x;
		TPoint pos(m_Map.m_TileData[iTile].pos.x, (float)m_Map.m_TileData[iTile].pos.y);
		m_User.m_bDebugRect = false;
		m_User.Load(L"../../data/temp.bmp");
		RECT rt = { 0, 0,m_User.m_pImage->m_bmpInfo.bmWidth,m_User.m_pImage->m_bmpInfo.bmHeight };
		m_User.SetSourceRect(pos.x, pos.y, rt);
		m_User.m_fSpeed = 100.0f;
		generator.setWorldSize({ m_Map.m_iMaxTileWidth, m_Map.m_iMaxTileHeight });
		for (int h = 0; h < m_Map.m_iMaxTileHeight; h++)
		{
			for (int w = 0; w < m_Map.m_iMaxTileWidth; w++)
			{
				int iTileID = h * m_Map.m_iMaxTileWidth + w;
				if (m_Map.m_TileData[iTileID].iType == 1)
				{
					generator.addCollision({ w,h });
				}				
			}
		}
		generator.setHeuristic(AStar::Heuristic::euclidean);
		generator.setDiagonalMovement(true);
		return true;
	}
	bool		Frame() {

		if (g_ActionInput.bLeftClick)
		{
			m_iCurrentPathIndex = 0;
			m_Map.m_PathList.clear();
			// Plyod use
			////// Ployd
			////int iUserTile = m_Map.GetPloydIndex(m_User.m_vPos.x, m_User.m_vPos.y);
			////int iTarget = m_Map.GetPloydIndex(I_Input.m_MousePos.x, I_Input.m_MousePos.y);			
			////m_pTargetPath = m_Map.GetPloydPathList(iUserTile, iTarget);
			////m_Map.PloydListToTileList(iUserTile, iTarget);

			// AStar use
			TIndex start = m_Map.GetTileID(m_User.m_vPos.x, m_User.m_vPos.y);
			TIndex end = m_Map.GetTileID(I_Input.m_MousePos.x, I_Input.m_MousePos.y);
			auto path = Path(start, end);
			for (auto& pos : path)
			{
				int iTile = pos.y * m_Map.m_iMaxTileWidth + pos.x;				
				m_Map.m_PathList.push_back({ (float)m_Map.m_TileData[iTile].pos.x, (float)m_Map.m_TileData[iTile].pos.y});
			}		
		}
		if (m_Map.m_PathList.size() > 0)
		{
			TSphere rt;
			rt.vCenter = m_Map.m_PathList[m_iCurrentPathIndex];
			rt.fRadius = 5;
			if (TCollision::SphereInPoint(rt, m_User.m_vPos))
			{
				m_iCurrentPathIndex++;
				if (m_Map.m_PathList.size() <= m_iCurrentPathIndex)
				{
					m_Map.m_PathList.clear();
					m_iCurrentPathIndex = 0;					
				}
			}
			m_User.Move(rt.vCenter);
		}
		return true;
	}
	bool		Render() 
	{
		m_Map.Reset();
		m_Map.RenderTile();
		m_Map.Render();		
		m_Map.RenderPath();
		m_User.Render();
		return true;
	}
	bool		Release() 
	{
		m_Map.Release();
		m_User.Release();
		return true;
	}
	CoordinateList      Path(TIndex s, TIndex e)
	{
		if (!(m_Map.IsRoad(s) && m_Map.IsRoad(e)))
		{
			std::vector<TIndex> path;
			return path;
		}
		auto path = generator.findPath(s, e);
		std::reverse(path.begin(), path.end());
		return path;
	}	
public:
	Sample() {};
	virtual ~Sample() {};
};
#else

class Sample : public TCore
{
public:
	bool Init()
	{
		AStar::Generator generator;
		// Set 2d g_Map size.
		generator.setWorldSize({ 8, 6 });
		generator.addCollision({ 4,0 });
		generator.addCollision({ 4,1 });
		generator.addCollision({ 4,2 });
		generator.addCollision({ 4,3 });
		//generator.addCollision({ 4,4 });

		generator.addCollision({ 1,1 });
		generator.addCollision({ 1,2 });
		generator.addCollision({ 1,3 });
		//generator.addCollision({ 1,4 });

		generator.addCollision({ 2,3 });
		generator.addCollision({ 3,3 });
		// You can use a few heuristics : manhattan, euclidean or octagonal.
		generator.setHeuristic(AStar::Heuristic::euclidean);
		generator.setDiagonalMovement(true);

		auto path = generator.findPath({ 2,2 }, { 6, 2 });

		for (auto& coordinate : path)
		{
			std::cout << coordinate.x << " " << coordinate.y << "\n";
		}
		return true;
	}
public:
	Sample() {};
	virtual ~Sample() {};
};
#endif

TCORE_RUN(SampleMaze, 800, 600);