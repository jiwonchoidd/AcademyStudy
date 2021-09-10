#include "TLock.h"

Lock::Lock(Order order)
	:mOrder(order)
{
}

bool Lock::operator<(const Lock & rhs)
{
	return mOrder < rhs.mOrder;
}

bool Lock::operator>(const Lock & rhs)
{
	return mOrder > rhs.mOrder;
}
