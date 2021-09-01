#include "TDemoGameScene.h"
#include "TGamePlayer.h"
TNetworkClient* TDemoGameScene::m_pNetwork = nullptr;

TObject*  TDemoGameScene::NewObj(TObjAttribute& info)
{
	TObject* pObject = TScene::NewObj(info);
	if (pObject == nullptr)
	{
		if (info.iObjType == TDemoEnum::T_AIRNPC)
		{
			pObject = new TNpcAirObject;
			TNpcAirObject* pNpc = dynamic_cast<TNpcAirObject*>(pObject);
			if (pNpc != nullptr)
			{
				pNpc->FSM();
			}
		}
		if (info.iObjType == TDemoEnum::T_GAMEPLAYER)
		{
			pObject = new TGamePlayer;
		}
	}
	return pObject;
}