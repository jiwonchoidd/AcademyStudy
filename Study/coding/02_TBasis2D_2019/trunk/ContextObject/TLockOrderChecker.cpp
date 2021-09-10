#include "TLockOrderChecker.h"
#include "TException.h"

LockOrderChecker::LockOrderChecker()
	:mStackTopPos(0)
{
	memset(mLockStack, 0, sizeof(mLockStack));
}

void LockOrderChecker::push(Lock* lock)
{
	//depth check
	crashAssert(mStackTopPos < MAX_LOCK_DEPTH);

	if (mStackTopPos > 0)
	{
		//order check
		crashAssert((*mLockStack[mStackTopPos - 1]) < (*lock));
	}

	mLockStack[mStackTopPos++] = lock;
}

void LockOrderChecker::pop(Lock* lock)
{
	//depth check
	crashAssert(mStackTopPos > 0);

	//order check
	crashAssert(mLockStack[mStackTopPos - 1] == lock);

	mLockStack[--mStackTopPos] = nullptr;
}
