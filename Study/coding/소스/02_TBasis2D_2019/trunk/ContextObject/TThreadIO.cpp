#include "TThreadIO.h"
#include "TCompletionPort.h"


IOThread::IOThread(CompletionPort * port)
	: mPort(port)
{
}

void IOThread::init()
{
}

void IOThread::run()
{
	while (true)
	{
		mPort->job();
	}
}
