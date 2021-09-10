#include "TSceneLobby.h"
bool  TSceneLobby::Reset()
{
	//TScene::Reset();
	m_bSceneChange = false;
	return true;
}
bool	TSceneLobby::Frame()
{
	TScene::Frame();
	
	TButton* pButtonGo = (TButton*)FindUI(L"go");
	TButton* pButtonStart = (TButton*)FindUI(L"BUTTON1");
	if (pButtonStart->m_bSelect)
	{
		UPACKET sendPacket;
		m_pNetwork->MakePacket(sendPacket,nullptr, 0, PACKET_ZONE_ENTRY_REQ);
		m_pNetwork->m_sendPacket.push_back(sendPacket);			
	}	
	return true;
}
bool	TSceneLobby::Render()
{
	return TScene::Render();
}
TSceneLobby::TSceneLobby()
{
	m_iSceneID = TScene::TSCENE_LOBBY;
}
TSceneLobby::~TSceneLobby()
{

}