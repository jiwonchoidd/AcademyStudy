#include "TStandAction.h"
TStandAction::TStandAction(TEnemy* parent): TEnemyState(parent)
{
}

TStandAction::~TStandAction()
{
}

void TStandAction::Process(xObject* ch)
{
	POINT pTarget = ch->getPosition(); // hero
	POINT pPos = m_pOwner->getPosition(); // anemy
	pTarget.y *= -1.0f;
	pPos.y *= -1.0f;

	Vector2 vDir;
	vDir.x = pTarget.x - pPos.x;
	vDir.y = pTarget.y - pPos.y;
	vDir = vDir.Normal();

	float fAngle = vDir.Angle(m_pOwner->m_vLook);

	// 시계 반대방향으로 움직이면 각도를 뺀다.
	Vector2 vCCW;
	bool bAngle = vCCW.CCW(m_pOwner->m_vLook, vDir);
	if (bAngle == false)
	{
		fAngle *= -1.0f;
	}
	m_pOwner->m_vLook = vDir;
	m_pOwner->SetAngle(fAngle);


	RECT rect;
	rect.left = m_pOwner->getPosition().x - m_pOwner->GetRadius() * 10;
	rect.top = m_pOwner->getPosition().y - m_pOwner->GetRadius() * 10;
	rect.right = m_pOwner->getPosition().x + m_pOwner->GetRadius() * 10;
	rect.bottom = m_pOwner->getPosition().y + m_pOwner->GetRadius() * 10;
	
	if (PtInRect( &rect,ch->getPosition()) == TRUE)
	{
		m_pOwner->SetTransition(EVENT_FINDTARGET);

		return;
	}

	if (rand() % 100 < 5)
	{
		m_pOwner->SetTransition(EVENT_PATROL);
		return;
	}
}

