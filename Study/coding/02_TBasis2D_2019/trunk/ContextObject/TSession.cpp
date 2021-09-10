#include "TSession.h"
#include "TSessionContext.h"

Session::Session(CompletionPort * port_, std::size_t sendBufSize, std::size_t recvBufSize)
	:socket(new Socket(ConnectType::TCP)), port(port_), sendBuffer(sendBufSize), recvBuffer(recvBufSize), connected(false), refCount(0), mSendLock(0)
{
}

Session::~Session()
{
	delete socket;
}

bool Session::disconnect(Context* context)
{
	// already disconnected
	if (false == connected.exchange(false))
		return true;

	return socket->disconnect(context);
}
bool Session::accept(Socket* listen, Context* context)
{
	return socket->accept(listen, context);
}
bool Session::onDisconnect(int reason)
{
	return true;
}

bool Session::preRecv()
{
	if (!isConnected())
		return false;

	PreRecvContext* context = new PreRecvContext(this);

	context->mWsabuf.len = 0;
	context->mWsabuf.buf = nullptr;

	return socket->recv(context, context->mWsabuf);
}

bool Session::recv()
{
	if (!isConnected())
		return false;

	if (0 == recvBuffer.getFreeSpaceSize())
		return false;

	RecvContext* recvContext = new RecvContext(this);

	DWORD recvbytes = 0;
	DWORD flags = 0;
	recvContext->mWsabuf.len = (ULONG)recvBuffer.getFreeSpaceSize();
	recvContext->mWsabuf.buf = (char*)recvBuffer.getBuffer();

	return socket->recv(recvContext, recvContext->mWsabuf);
}

bool Session::acceptCompletion(Socket* listen)
{
	if (true == connected.exchange(true))
	{
		crashAssert(false);
		return false;
	}

	bool resultOk = true;
	do
	{
		if (!socket->updateAcceptContext(listen))
		{
			resultOk = false;
			break;
		}

		if (!socket->setNodelay(true))
		{
			resultOk = false;
			break;
		}

		if (!socket->setReceiveBufferSize(0))
		{
			resultOk = false;
			break;
		}

		if (!socket->completeTo(port))
		{
			resultOk = false;
			break;
		}
	} while (false);

	if (!resultOk)
	{
		return false;
	}

	if (false == preRecv())
	{
		printf("pre recv failed.\n");
	}

	return onAccept();
}

bool Session::onAccept()
{
	return true;
}

bool Session::onRead()
{
	return true;
}

bool Session::onSend()
{
	return true;
}

bool Session::sendCompletion(DWORD transferred)
{
	sendBuffer.remove(transferred);

	return onSend();
}

bool Session::recvCompletion(DWORD transferred)
{
	recvBuffer.commit(transferred);

	return onRead();
}

bool Session::disconnectCompletion(int reason)
{
	bool res = onDisconnect(reason);

	releaseRef();

	return res;
}

void Session::addRef()
{
	refCount++;
}

void Session::releaseRef()
{
	refCount--;

	crashAssert(refCount >= 0);

	if (refCount == 0)
	{
		onRelease();
	}
}

void Session::onRelease()
{

}

bool Session::send(char * packet, std::size_t len)
{
	if (!isConnected())
		return false;

	Guard<SpinLock> guard(mSendLock, true);

	if (sendBuffer.getFreeSpaceSize() < len)
		return false;

	sendBuffer.write(packet, len);

	FunctionContext* flushSend = new FunctionContext([session = this]()
	{
		return session->flushSend();
	});

	return true;
}

bool Session::isConnected()
{
	return connected;
}

bool Session::flushSend()
{
	if (!isConnected())
	{
		DisconnectContext* context = new DisconnectContext(this, 0/*temp value*/);
		disconnect(context);
		return false;
	}

	Guard<SpinLock> guard(mSendLock, true);

	if (0 == sendBuffer.getContiguiousBytes())
	{
		return false;
	}

	SendContext* context = new SendContext(this);

	DWORD sendbytes = 0;
	DWORD flags = 0;

	context->mWsabuf.len = (ULONG)sendBuffer.getContiguiousBytes();
	context->mWsabuf.buf = (char*)sendBuffer.getBufferStart();

	if (!socket->send(context, context->mWsabuf))
	{
		DisconnectContext* context = new DisconnectContext(this, 0/*temp value*/);
		disconnect(context);
		return true;
	}

	return false;
}
