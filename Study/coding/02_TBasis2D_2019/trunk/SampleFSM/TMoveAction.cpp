#include "TMoveAction.h"

TMoveAction::TMoveAction(TEnemy* parent): TEnemyState(parent)
{
	m_vTargetPos.x = rand() % g_rtClient.right;
	m_vTargetPos.y = rand() % g_rtClient.bottom;
}

TMoveAction::~TMoveAction()
{
}

void TMoveAction::Process(xObject* ch)
{
	// ��� ������ ����(0, 0) ���� �����ϰ�
	POINT pTarget = ch->getPosition(); // hero
	POINT pPos = m_pOwner->getPosition(); // anemy
	// Y���� �������� �Ʒ��� �����ϵ��� Ŭ���̾�Ʈ ��ǥ�� ���� ��ǥ�� ��ȯ�Ѵ�.
	pTarget.y *= -1.0f;
	pPos.y *= -1.0f;
	// ���� ���� �������� ���Ϳ� ������ ������ ���� ����Ѵ�.
	Vector2 vDir;
	vDir.x = pTarget.x - pPos.x;
	vDir.y = pTarget.y - pPos.y;	
	vDir = vDir.Normal();	
	float fAngle = vDir.Angle(m_pOwner->m_vLook);

	// �ð� �ݴ�������� �����̸� ������ ����.
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
	rect.bottom=m_pOwner->getPosition().y + m_pOwner->GetRadius() * 10;

	if (PtInRect(&rect, ch->getPosition()) == TRUE)
	{
		m_pOwner->SetTransition(EVENT_FINDTARGET);
		return;
	}

	rect.left = m_pOwner->getPosition().x - 5;
	rect.top = m_pOwner->getPosition().y - 5;
	rect.right = m_pOwner->getPosition().x + 5;
	rect.bottom=m_pOwner->getPosition().y + 5;


	if (PtInRect(&rect, m_vTargetPos) == TRUE)
	{
		m_vTargetPos.x = rand() % g_rtClient.right;
		m_vTargetPos.y = rand() % g_rtClient.bottom;
		m_pOwner->SetTransition(EVENT_STOPWALK);
		return;
	}
	m_pOwner->Move(m_vTargetPos.x, m_vTargetPos.y);
}
