#include "TNetWork.h"
#include "TSelect.h"

TNetwork		m_Network; 

int main()
{
	if (m_Network.Server()==false)
	{
		printf("\n윈속 초기화 오류");
		return 0;
	}
	// 3)모델 선택
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