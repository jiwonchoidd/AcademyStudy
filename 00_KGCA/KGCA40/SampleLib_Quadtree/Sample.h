#pragma once
#include <TCore.h>
#include "ShapeObj.h"
#include "MapObj.h"
#include "Camera.h"
#include "KQuadtree.h"
class Sample : public TCore
{
public:
	//BoxShapeObj		m_BoxObj[2];
	MapObj			m_Map;
	DebugCamera		m_Camera;
	KQuadtree		m_Quadtree;
public:
	float m_fYaw = 0.0f;
	float m_fPitch = 0.0f;
public:
	bool	Init() override;
	bool	Frame() override;
	bool	Render() override;
	bool	Release() override;
	//마우스 등 윈도우 메세지 프로시저 
	LRESULT MsgProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) override;
public:
	Sample();
	virtual ~Sample();
};

