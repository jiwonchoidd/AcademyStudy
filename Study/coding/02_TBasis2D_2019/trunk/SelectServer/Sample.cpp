#include "TNetWork.h"
#include "TSelect.h"

TNetwork		m_Network; 

int main()
{
	if (m_Network.Server()==false)
	{
		printf("\n���� �ʱ�ȭ ����");
		return 0;
	}
	// 3)�� ����
	m_Network.SetModel(make_shared<TSelect>(m_Network.m_Sock));

	while (1)
	{
		m_Network.m_pModel->Frame();
		Sleep(1);
	}
	
	m_Network.Release();
	getchar();
	return 0;
}