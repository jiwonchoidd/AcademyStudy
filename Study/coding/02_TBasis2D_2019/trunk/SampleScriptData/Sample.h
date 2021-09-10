#pragma once
#include "TCore.h"
#include "TObject.h"
#include "TScriptMgr.h"

class Sample : public TCore
{
public:	
	int  m_iTimerSprite;
	std::vector<TObject>   m_TimerObj;
	std::vector<TObject>   m_EffectObj;
	std::vector<TObject>   m_UIObj;
	std::vector<TObject>   m_SpliteListClick;
public:
	bool LoadScript();
	bool Init();
	bool Frame();
	bool Render();
	bool Release();
public:
	Sample();
	~Sample();
};

