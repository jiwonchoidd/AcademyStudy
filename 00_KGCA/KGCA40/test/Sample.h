#pragma once
#include <TCore.h>
#include "KShape.h"
#include "KMap.h"
#include "KCamera.h"
#include "KQuadtree.h"
class Sample : public TCore
{
public:
	//TBoxShape			m_BoxObj[2];
	KMap				m_Map;
	KDebugCamera		m_Camera;
	KQuadtree			m_Quadtree;
	float m_fYaw =0.0f;
	float m_fPitch=0.0f;
public:
	bool   Init() override;
	bool   Frame()override;
	bool   Render()override;
	bool   Release()override;
	LRESULT MsgProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) override;
public:
	Sample();
	virtual ~Sample();
};

