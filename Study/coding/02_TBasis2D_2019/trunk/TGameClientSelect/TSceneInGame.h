#pragma once
#include "TDemoGameScene.h"
#include "TProtocol.h"
class TSceneInGame : public TDemoGameScene
{
public:
	bool Frame()	override;
	bool Render()	override;
	bool Reset()	override;
	bool CreateScene() override;
	bool SetObject(TObjAttribute& iType, TObject* pObj) override;
	void AddNpc(wstring name, TPoint pos, TPoint dir, int index);
public:
	TSceneInGame();
};

