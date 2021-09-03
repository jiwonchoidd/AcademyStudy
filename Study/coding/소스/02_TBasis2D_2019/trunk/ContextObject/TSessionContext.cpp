#include "TSessionContext.h"
#include "TSession.h"

bool AcceptContext::onComplete(int transferred, int key)
{
	return mSession->acceptCompletion(mListen);
}

bool PreRecvContext::onComplete(int transferred, int key)
{
	return mSession->recv();
}


bool RecvContext::onComplete(int transferred, int key)
{
	if (transferred == 0)
	{
		DisconnectContext* context = new DisconnectContext(mSession, 0/* temp value*/);
		mSession->disconnect(context);
	}

	return mSession->recvCompletion(transferred);
}

bool SendContext::onComplete(int transferred, int key)
{
	if (transferred == 0)
	{
		DisconnectContext* context = new DisconnectContext(mSession, 0/* temp value*/);
		mSession->disconnect(context);
	}

	return mSession->sendCompletion(transferred);
}


bool DisconnectContext::onComplete(int transferred, int key)
{
	return mSession->disconnectCompletion(mReason);
}

void RecvContext::onFailure()
{
	DisconnectContext* context = new DisconnectContext(mSession, 0/* temp value*/);

	mSession->disconnect(context);
}

void SendContext::onFailure()
{
	DisconnectContext* context = new DisconnectContext(mSession, 0/* temp value*/);

	mSession->disconnect(context);
}

UdpContext::UdpContext()
{
	memset(&mAddress, 0, sizeof(SOCKADDR_IN));
}

UdpContext::UdpContext(const std::string& addr, std::uint16_t port)
{
	mAddress.sin_family = AF_INET;
	mAddress.sin_port = htons(port);
	mAddress.sin_addr.s_addr = inet_addr(addr.c_str());
}

UdpContext::UdpContext(SOCKADDR_IN address)
	:mAddress(address)
{

}

bool FunctionContext::onComplete(int transferred, int key)
{
	return mFunc();
}

void FunctionContext::onFailure()
{
	if (mFailFunc == nullptr)
		return;
	mFailFunc();
}