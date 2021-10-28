#pragma once
#include <KCore.h>
#include "KTexture.h"
#include "KShape.h"
#include "KMap.h"
#include "KCamera.h"
#include "KQuadtree.h"
class Sample : public KCore
{
public:
	//TBoxShape			m_BoxObj[2];
	KMap				m_Map;
	KQuadtree			m_Quadtree;
	KTexture			m_KTexture;
public:
	bool   Init() override;
	bool   Frame()override;
	bool   Render()override;
	bool   Release()override;
public:
	Sample();
	virtual ~Sample();
};

