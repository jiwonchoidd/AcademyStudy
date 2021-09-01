#include "Sample.h"
#include "TObjectMgr.h"
bool  Sample::Init()
{
	//m_hExecuteMutex = CreateMutex(NULL, FALSE,
	//	L"OnceMutex");
	//if (GetLastError() == ERROR_ALREADY_EXISTS)
	//{
	//	CloseHandle(m_hExecuteMutex);
	//	return;
	//}
	if (m_Network.InitNetwork("112.216.123.139", 10000) == false)
	{
		return false;
	}
	return true;
}
bool  Sample::Frame()
{
	EnterCriticalSection(&m_Network.m_cs);
		m_Network.RecvData(m_User);
	LeaveCriticalSection(&m_Network.m_cs);
	return true;
}
bool  Sample::Release()
{
	m_Network.DeleteNetwork();
	//CloseHandle(m_hExecuteMutex);
	return true;
}
bool  Sample::Render()
{
	m_Network.PacketProcess();

	//TChatMsg chatmsg;
	//memset(&chatmsg, 0, sizeof(chatmsg));
	////fgets(chatmsg.buffer, 128, stdin);
	//strcpy_s(chatmsg.szName, "ȫ�浿");
	//strcpy_s(chatmsg.buffer, "kgca");
	//chatmsg.iCnt = 1;
	//if (m_Network.SendMsg(m_Network.m_Sock, (char*)&chatmsg,
	//	sizeof(TChatMsg),
	//	PACKET_CHAT_MSG) < 0)
	//{
	//	return false;
	//}
	return true;
}
GAMERUN;