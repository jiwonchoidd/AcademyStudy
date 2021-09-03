#pragma once
#include "TDemoGameScene.h"
#include "TInput.h"
class TSceneIntro : public TDemoGameScene
{
public:
	T_STR  m_szUserID;
	T_STR  m_szUserPS;
public:
	bool    SetObject(TObjAttribute& iType, TObject* pObj) override;
	bool	Frame() override;
	virtual bool  RenderUI() override;
	bool	Reset() override;
public:
	TSceneIntro();
	virtual ~TSceneIntro();
};

