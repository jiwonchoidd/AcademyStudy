#include "KCommandServer.h"
/*Command Line Interface*/
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
				std::cout << "해당 명령어 없음" << std::endl;
			}
		}
		m_Input = "";
		std::cin.clear();
		Sleep(2);

		if (m_CommandMode != 0)
		{
			std::cout << "okay";

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
