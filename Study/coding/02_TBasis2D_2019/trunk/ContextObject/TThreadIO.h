#pragma once
#include "TThreadEx.h"
class CompletionPort;
class IOThread : Thread
{
public:
	explicit IOThread(CompletionPort* port);
	~IOThread() override = default;

	void init() override;
	void run() override;

	CompletionPort* GetPort() const
	{
		return mPort;
	}
private:
	CompletionPort* mPort;
};
