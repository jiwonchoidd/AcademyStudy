#pragma once
#include "TObject.h"
#include "TImageMgr.h"

class TSprite : public TObject
{
public:
	TObjectInfo m_info;
	float		m_fDeltaTime;// = 3.0f / iNumSprite;
	bool		m_bDead;
	int			m_iSpriteID;
	int			m_iCurrentFrame;
	float       m_fTmpTime;
	bool		m_bSceneChange;
public:
	bool  Init();
	virtual bool  Set(TObjectInfo info, int id, int mask);
	bool  Frame();
	bool  Render();
	virtual void  Draw(DWORD dwMode);
	void  Draw(int x, int y, DWORD dwMode);
	void  DrawCenter(DWORD dwMode);
	bool  Release();
public:
	TSprite();
	virtual ~TSprite();
};
