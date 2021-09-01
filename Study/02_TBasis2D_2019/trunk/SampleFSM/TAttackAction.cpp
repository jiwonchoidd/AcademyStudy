#include "TAttackAction.h"
TAttackAction::TAttackAction(TEnemy* parent): TEnemyState(parent)
{
}

TAttackAction::~TAttackAction()
{
}
void TAttackAction::Process(xObject* ch)
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
	//// �ð� �ݴ�������� �����̸� ������ ����.
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

	if (PtInRect(&rect, ch->getPosition()) == FALSE)
	{
		m_pOwner->SetTransition(EVENT_LOSTTARGET);
		return;
	}
	m_pOwner->Move(ch->getPosition().x, ch->getPosition().y);
}
