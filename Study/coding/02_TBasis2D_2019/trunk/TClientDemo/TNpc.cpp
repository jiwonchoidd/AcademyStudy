#include "TNpc.h"
TNpc::TNpc()
{
}
void TNpc::MoveToLocation(const FVector& dest)
{
	//MoveTarget(dest);
}

void TNpc::UpdateHealth(float HealthChange)
{
	m_fHealth += HealthChange;
}

float TNpc::GetHealth()
{
	return m_fHealth;
}


