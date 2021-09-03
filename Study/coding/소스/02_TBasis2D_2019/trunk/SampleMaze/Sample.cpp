#pragma once
#include "TCore.h"
#include "TMazeMap.h"


class Sample : public TCore
{
public:
	TMazeMap   m_Map;
	TObject    m_User;
	TPloydPath* m_pTargetPath;
	vector<TPoint>	m_PathList;
public:
	bool		Init() {
		m_Map.Create(10,10, m_rtClient.right, m_rtClient.bottom);
		m_User.Load(L"../../data/sand.bmp");
		RECT rt = { 0, 0,m_User.m_pImage->m_bmpInfo.bmWidth,m_User.m_pImage->m_bmpInfo.bmHeight };
		m_User.SetSourceRect(m_Map.m_TileData[0].pos.x, m_Map.m_TileData[0].pos.y,rt);
		return true;
	}
	bool		Frame() 
	{		
		if (g_ActionInput.bLeftClick == KEY_PUSH )
		{
			int iUserTile = m_Map.GetPloydIndex( m_User.m_vPos.x, m_User.m_vPos.y);
			int iTarget = m_Map.GetPloydIndex(I_Input.m_MousePos.x, I_Input.m_MousePos.y);
			m_pTargetPath = m_Map.GetPloydPathList(iUserTile, iTarget);
			m_PathList = m_Map.PloydListToTileList(iUserTile, iTarget);
		}		
		return true;
	}
	bool		Render() {		
		m_Map.Reset();
		m_Map.RenderTile();
		m_Map.Render();
		//m_User.Render();
		m_Map.RenderPath();
		return true;
	}
	bool		Release() {
		m_Map.Release();
		return true;
	}
public:
	Sample() {};
	virtual ~Sample() {};
};
TCORE_RUN(SampleMaze, 800, 600);