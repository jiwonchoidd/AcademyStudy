#include "KCommandServer.h"
#include "KServer.h"
void KCommandServer::HelpMode()
{
	std::cout << "��ɾ� ����Ʈ" << std::endl;
	for (m_iter_map = m_CommandMap.begin();
		m_iter_map != m_CommandMap.end();)
	{
		std::cout << m_iter_map->first << ", \n";
		m_iter_map++;
	}

}
void KCommandServer::ExitMode()
{
	std::cout << "������ �����մϴ�." << std::endl;
	KServer* pServer = (KServer*)m_pObject;
	::SetEvent(pServer->m_hKillEvent);
	pServer->Release();
}
/*Command Line Interface
 q*/
bool KCommandServer::Run()
{
	while (1)
	{
		std::cin >> m_Input;

		for (m_iter_map = m_CommandMap.begin();
			m_iter_map != m_CommandMap.end();)
		{
			if (m_Input == m_iter_map->first)
			{
				m_CommandMode =m_iter_map->second;
				break;
			}
			else
			{
				m_iter_map++;
				if(m_iter_map== m_CommandMap.end())
				std::cout << "�ش� ��ɾ� ����" << std::endl;
			}
		}

		m_Input = "";
		std::cin.clear();
		Sleep(2);

		if (m_CommandMode != 0)
		{
			switch (m_CommandMode)
			{
			case COMMAND_HELP:
			{
				HelpMode();
			}break;
			case COMMAND_EXIT:
			{
				ExitMode();
			}break;
			default:
				break;
			}
			m_CommandMode = 0;
		}
	}
	return true;
}

KCommandServer::KCommandServer()
{
}

KCommandServer::KCommandServer(LPVOID pValue)
{
}

KCommandServer::~KCommandServer()
{
}
