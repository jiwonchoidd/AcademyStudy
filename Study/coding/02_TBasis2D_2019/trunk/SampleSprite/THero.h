#pragma once
#include "xObject.h"
struct TSpriteRect
{
	float  fSpriteSpeed;
	float  fDirection;
	bool  bStartAnim;
	float  fStartTime;
	bool  bEndAnim;
	int    iFrame;
	DWORD  m_KeyValue[2];
	std::vector<RECT>   rtSprite;
	TSpriteRect()
	{
		fSpriteSpeed = 10.0f;
		bStartAnim = false;
		fStartTime = 0.0f;
		bEndAnim = false;
		iFrame = 0;;
	}
};
class THero : public xObject
{
public:
	RECT          m_rtDeskPos;
	RECT          m_rtSrcPos;
	RECT			m_rtDraw;
public:
	TSpriteRect   m_ExplosionEffect;
	TSpriteRect   m_SparkEffect;
	TSpriteRect   m_FlareEffect;
	TSpriteRect   m_SmokeEffect;
	TSpriteRect   m_LeftState;
	TSpriteRect   m_RightState;
public:
	void           Movement(TSpriteRect&   key, float & pos);
	bool		   Update();
	
public:
	THero();
	virtual ~THero();
};

