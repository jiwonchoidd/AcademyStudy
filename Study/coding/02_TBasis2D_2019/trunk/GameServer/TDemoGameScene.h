#pragma once
#include "TScene.h"
#include "TNetObject.h"
#include "TServerObj.h"
struct TDemoEnum : TEnum
{
	enum  TGameType1 
	{
		T_AIRNPC = TEnum::T_NPC + 1,
		T_GAMEPLAYER = TEnum::T_USER + 1,
	};
};
class TDemoGameScene : public TScene, public TServerObj
{
public:
	TObject*		NewObj(TObjAttribute& info);
	virtual bool	Frame() override;
	virtual bool	Render() override;
	virtual bool	Release() override;
	virtual bool	Load(const TCHAR* filename) override;
	virtual void	AddNpc(wstring name, TPoint pos, TPoint dir, int iIndex) override;
};

