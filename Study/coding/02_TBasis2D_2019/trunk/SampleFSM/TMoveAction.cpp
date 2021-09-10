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
	// 상단 왼쪽이 원점(0, 0) 으로 설정하고
	POINT pTarget = ch->getPosition(); // hero
	POINT pPos = m_pOwner->getPosition(); // anemy
	// Y값은 원점에서 아래로 감소하도록 클라이언트 좌표를 직각 좌표로 변환한다.
	pTarget.y *= -1.0f;
	pPos.y *= -1.0f;
	// 이후 이전 프레임의 벡터와 현재의 벡터의 각을 계산한다.
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
