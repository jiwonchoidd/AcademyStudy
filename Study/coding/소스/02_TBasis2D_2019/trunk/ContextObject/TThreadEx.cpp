#include "TThreadEx.h"
#include "TLockOrderChecker.h"

std::atomic<int> Thread::mIdMaker = 0;
thread_local Thread* thisThread = nullptr;

void threadFunc(Thread* thread)
{
	thisThread = thread;

	thread->init();

	thread->run();
}

Thread::Thread()
	: mId(mIdMaker++), mThread(threadFunc, this), mLockOrderChecker(new LockOrderChecker())
{
}

void Thread::init()
{
}

void Thread::run()
{
}

int Thread::wait()
{
	mThread.join();

	return 0;
}
