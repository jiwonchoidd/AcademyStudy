#pragma once
#include "TDemoGameScene.h"
class TSceneLobby : public TDemoGameScene
{
public:
	bool	Frame() override;
	bool	Render() override;
	bool	Reset() override;
public:
	TSceneLobby();
	virtual ~TSceneLobby();
};

