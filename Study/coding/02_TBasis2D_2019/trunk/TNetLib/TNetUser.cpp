#include "TNetUser.h"
TNetUser::TNetUser()
{
	iRecvSize = 0;
	iSendSize = 0;
	ZeroMemory(recvBuf, sizeof(char) * 10000);
}
TNetUser::~TNetUser()
{

}