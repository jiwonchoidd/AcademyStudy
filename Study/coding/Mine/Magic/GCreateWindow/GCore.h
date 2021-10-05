#pragma once
#include "GWindow.h"
class GCore : public GWindow
{
private:
	bool GameInit() override;
	bool GameRun() override;
	bool GameFrame();
	bool GameRender();
	bool GameRelease() override;

public:
	virtual bool Init();
	virtual bool Frame();
	virtual bool Render();
	virtual bool Release();

	};

