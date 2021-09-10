#pragma once
#include "TNetworkClient.h"
#include "TCore.h"
class Sample : public TCore
{
	TNetworkClient	m_Network;
	TNetUser			m_User;
	HANDLE			m_hExecuteMutex;
public:
	bool  Init();
	bool  Frame();
	bool  Render();
	bool  Release();
};

