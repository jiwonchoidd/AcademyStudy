#pragma once
#include "TCamera.h"
class TDebugCamera : public TCamera
{
public:
	void Left();
	void Right();
	void Up();
	void Down();
	void Front();
	void Back();
	void Update(D3DXVECTOR4& vDir);
public:
	TDebugCamera();
	virtual ~TDebugCamera();
};

