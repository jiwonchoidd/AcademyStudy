#pragma once
#include <TCore.h>
#include "ShapeObj.h"
#include "MapObj.h"
#include "Camera.h"
class Sample : public TCore
{
public:
	BoxShapeObj		m_BoxObj[2];
	MapObj			m_Map;
	DebugCamera		m_Camera;
public:
	bool	Init();
	bool	Frame();
	bool	Render();
	bool	Release();
};

