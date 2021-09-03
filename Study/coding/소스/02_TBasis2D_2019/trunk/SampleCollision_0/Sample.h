#pragma once
#include "TCore.h"
#include "xObject.h"

const int g_iMaxNPC = 30;

struct TProjectile
{
	TPoint pos;
	RECT   rtCollision;
	TSphere m_Sphere;
	float fLifeTime;
	RECT   rtDesk;
	void  SetPos(TPoint p)
	{
		SetPos(p.x, p.y);
	}
	void  SetPos(float x, float y)
	{
		pos.x = x;
		pos.y = y;
		rtCollision.left = pos.x;
		rtCollision.top = pos.y;
		rtCollision.right =
			rtCollision.left + rtDesk.right;
		rtCollision.bottom =
			rtCollision.top + rtDesk.bottom;

		m_Sphere.vCenter.x = (rtCollision.left + rtCollision.right) / 2;
		m_Sphere.vCenter.y = (rtCollision.top + rtCollision.bottom) / 2;
		float fA = rtCollision.right - m_Sphere.vCenter.x;
		float fB = m_Sphere.vCenter.y - rtCollision.top;
		m_Sphere.fRadius = sqrt(fA*fA + fB * fB);
	}
};
class Sample : public TCore
{
public:
	xObject		m_Ground;
	xObject		m_Hero;
	xObject		m_npc[g_iMaxNPC];
	xObject		m_Projectile;
	list<TProjectile>	m_ProjectileList;
	bool		m_bnpc[g_iMaxNPC];
	bool		m_bGameEnding;
public:
	bool		Init();
	bool		Frame();
	bool		Render();
	bool		Release();
	bool		RectInPt(RECT rt, POINT pt);
	bool		RectInRect(RECT rtDesk, RECT rtSrc);
	bool		SphereInSphere(RECT rtDesk, RECT rtSrc);
public:
	Sample();
	virtual ~Sample();
};
//TCORE_RUN(SampleLib_0, 1024, 768);
TCORE_START;
// 윈도우를 생성한다.
if (sample.SetWindow(hInstance,
	L"SampleBitmap_0", 800, 600 ))//, WS_POPUPWINDOW))
{
	// 생성된 윈도우를 가동한다.
	sample.Run();
}
TCORE_END;
