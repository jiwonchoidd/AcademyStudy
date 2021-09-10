#pragma once
#include "TScene.h"
#include "TNpcAirObject.h"
#include "TNetworkClient.h"

struct TDemoEnum : TEnum
{
	enum  TGameType1 
	{
		T_AIRNPC = TEnum::T_NPC + 1,
		T_GAMEPLAYER = TEnum::T_USER + 1,
	};
};
class TDemoGameScene : public TScene
{
public:
	TObject*  NewObj(TObjAttribute& info);
	static TNetworkClient* m_pNetwork;
};

