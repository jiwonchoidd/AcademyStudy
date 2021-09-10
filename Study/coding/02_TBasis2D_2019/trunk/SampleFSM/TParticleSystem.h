#pragma once
#include "TParticle.h"

class TParticleSystem
{
public:
	HDC			m_hScreenDC;
	xSprite*				m_pSprite;
	TParticle*				m_pParticles;	
	int						m_iStartCnt;
	int						m_iEndCnt;	
	float					m_fTime;
	float					m_fRadius;
	int						m_iMaxCnt;
	float					m_fAddTime;
	float					m_fAddRadius;
	POINT				m_vScale;

public:
	bool		Init();
	bool		Create(HDC hdc, int iMaxCounter, float fAddTime, float fAddRadius, POINT vScale);
	bool		Frame( float fGlobalTime, float fElapsedTime);
	bool		Render();
	bool		Release();
public:
	// 최대 파티클 갯수를 지정하고 할당한다
	void		SetMaxCounter( int iMaxNum);
	int			GetMaxCounter()					{ return m_iMaxCnt; }
	void		ResetCounter();
public:
	void		SetRadius( float fRadius = 1.0f){ m_fRadius = fRadius; }	
	void		SetAddTime( float fTime )		{ m_fAddTime = fTime; };
	void		SetAddRadius( float fTime )		{ m_fAddRadius = fTime; };
	void		SetScale( float fX, float fY ) { m_vScale.x = fX; m_vScale.y = fY;  };	
public:
	tPOINT		 Circle( float fRadius, float fRadian);
public:
	TParticleSystem(void);
	~TParticleSystem(void);
};
