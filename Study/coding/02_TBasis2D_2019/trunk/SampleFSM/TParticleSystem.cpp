#include "TParticleSystem.h"
#include "TTimer.h"

bool	 TParticleSystem::Init()
{
	m_hScreenDC	= NULL;
	m_pSprite		= NULL;
	m_pParticles	= NULL;
	m_fRadius		= 1.0f;
	m_iStartCnt		= 0;
	m_iEndCnt		= 1;	
	m_fTime			= 0.0f;
	m_iMaxCnt		= MAX_PARTICLE;
	m_fAddTime		= 0.1f;
	m_fAddRadius	= 30.0f;
	m_vScale.x		= 1.0f;
	m_vScale.y		= 1.0f;
	return true;
}
// 버텍스버퍼 하나를 사용하여 다수의 파티클을 그린다.
bool	 TParticleSystem::Create(HDC hdc,  int iMaxCounter, float fAddTime, float fAddRadius, POINT vScale )
{	
	m_hScreenDC = hdc;
	SetMaxCounter( iMaxCounter );
	SetAddTime( fAddTime  / iMaxCounter );
	SetAddRadius( fAddRadius );
	SetScale( vScale.x, vScale.y );	
	return true;
}	
void	TParticleSystem::SetMaxCounter( int iMaxNum ) 
{ 	
	m_iMaxCnt = iMaxNum; 
	SAFE_DELETE_ARRAY( m_pParticles );
	SAFE_NEW_ARRAY( m_pParticles, TParticle, m_iMaxCnt );
}
	
tPOINT TParticleSystem::Circle( float fRadius, float fRadian)
{
	tPOINT vFinally;		
	vFinally.x = fRadius * cosf(fRadian);
	vFinally.y = fRadius * sinf(fRadian);
	return vFinally;
}
void	TParticleSystem::ResetCounter()
{
	m_iStartCnt = 0;
	m_iEndCnt	= 1;
	m_fTime		= 0.0f;
}
bool	TParticleSystem::Frame( float fGlobalTime, float fElapsedTime)
{		
	tPOINT vReturnValue = Circle( m_fRadius, fGlobalTime );
	vReturnValue.x *= m_fAddRadius;
	vReturnValue.y *= m_fAddRadius;
	vReturnValue.x += g_rtClient.right/2;
	vReturnValue.y += g_rtClient.bottom/2;
	if( ( fGlobalTime-m_fTime ) > m_fAddTime)
	{		
		if( m_iEndCnt+1 > m_iMaxCnt)
		{
			m_iEndCnt = m_iMaxCnt;			
			m_iStartCnt++;			
			if( m_iStartCnt >= m_iEndCnt) 
			{
				m_iStartCnt = 0, m_iEndCnt = 1;
			}
		}
		else
		{
			m_pParticles[m_iEndCnt].m_vPos.x = vReturnValue.x;
			m_pParticles[m_iEndCnt].m_vPos.y = vReturnValue.y;
			m_iEndCnt+=1;				
		}		
		m_fTime = fGlobalTime;
	}	
	//m_pSprite->Frame( fGlobalTime, fElapsedTime );
	return true;
}

bool 	TParticleSystem::Render()
{		
	tPOINT matTrans;
	//for(int iParticle=0; iParticle < m_iMaxCnt; iParticle++)
	for (int iParticle = m_iStartCnt; iParticle < m_iEndCnt; iParticle++)
	{		
		matTrans.x = m_pParticles[iParticle].m_vPos.x;
		matTrans.y = m_pParticles[iParticle].m_vPos.y;

		m_pSprite =  I_Sprite.GetPtr(m_pParticles[iParticle].m_iSpriteIndex);

		m_pSprite->Updata(	matTrans,
							m_pParticles[iParticle].m_fTimer,
							m_pParticles[iParticle].m_iApplyIndex, 
							g_fAccumulation,
							g_fSecPerFrame );

		m_pSprite->Render();

		m_pParticles[iParticle].m_vPos.x = matTrans.x;
		m_pParticles[iParticle].m_vPos.y = matTrans.y;
	}
	return true;
}

bool 	TParticleSystem::Release()
{
	SAFE_DELETE_ARRAY( m_pParticles );
	return true;
}
	
TParticleSystem::TParticleSystem(void)
{
	Init();
}

TParticleSystem::~TParticleSystem(void)
{
}