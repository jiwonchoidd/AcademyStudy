#include "TParticle.h"

TParticle::TParticle()
{	
	m_iSpriteIndex = (rand() % I_Sprite.TMap.size()) + 1;
	m_vPos.x = rand() % (g_rtClient.right - 400) + 200;
	m_vPos.y = rand() % (g_rtClient.bottom - 200) + 100;
	m_fLifeTime = (float)(rand() % 5);
	m_fTimer	= 0.0f;
	m_vAdd.x = cosf(rand());
	m_vAdd.y = sinf(rand());
	m_iApplyIndex = 0;
}
TParticle::~TParticle()
{
}
