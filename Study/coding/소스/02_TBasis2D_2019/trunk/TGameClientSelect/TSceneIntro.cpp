#include "TSceneIntro.h"
#include <string.h>

bool  TSceneIntro::SetObject(TObjAttribute& info, TObject* pObj)
{
	if (TScene::SetObject(info, pObj) == false)
	{
		if (pObj && !(info.szParentName == L"nullptr" || info.szParentName == L"NULL"))
		{
			pObj->m_pParentObject = FindParent(info.szParentName);
			if (pObj->m_pParentObject != nullptr)
			{
				pObj->m_pParentObject->m_pChildObjects.push_back(pObj);
			}
		}

		switch (info.iObjType)
		{
		case TDemoEnum::T_GAMEPLAYER:
		{
			m_pGamePlayer = (TGameUser*)pObj;
			m_ptInitHeroPos = info.pos;
			pObj->Set(info.pos, info.rtSrc);
			pObj->m_bCollisionEnabled = true;
			return true;
		}break;
		}
	}
	return false;
}
bool  TSceneIntro::Reset()
{
	//TScene::Reset();
	m_bSceneChange = false;
	return true;
}
bool	TSceneIntro::Frame()
{
	TScene::Frame();
	TButton* pLoginButton = (TButton*)FindUI(L"LOGIN_BTN");
	TEdit* pIDEdit	= (TEdit*)FindUI(L"IDEDIT");
	TEdit* pPSEdit	= (TEdit*)FindUI(L"PASSEDIT");
	TEdit* pAccontBtn = (TEdit*)FindUI(L"ACCOUNT_BTN");
	if (pLoginButton->m_bSelect)
	{
		TLogin login;
		ZeroMemory(&login, sizeof(TLogin));
		strcpy_s(login.szID, to_wm(pIDEdit->m_szEdit).c_str());
		strcpy_s(login.szPS, to_wm(pPSEdit->m_szEdit).c_str());
		
		UPACKET sendPacket;
		m_pNetwork->MakePacket(sendPacket, (char*)&login,sizeof(TLogin), PACKET_LOGIN_REQ);
		m_pNetwork->m_sendPacket.push_back(sendPacket);

		/*m_pNetwork->MakePacket(sendPacket, (char*)&login,	sizeof(TLogin), PACKET_CREATE_ACCOUNT_REQ);
		m_pNetwork->m_sendPacket.push_back(sendPacket);*/
	}
	if (pAccontBtn->m_bSelect)
	{
		TLogin login;
		ZeroMemory(&login, sizeof(TLogin));
		strcpy_s(login.szID, to_wm(pIDEdit->m_szEdit).c_str());
		strcpy_s(login.szPS, to_wm(pPSEdit->m_szEdit).c_str());
		UPACKET sendPacket;
		m_pNetwork->MakePacket(sendPacket, (char*)&login,sizeof(TLogin), PACKET_CREATE_ACCOUNT_REQ);
		m_pNetwork->m_sendPacket.push_back(sendPacket);
	}	
	return true;
}
bool  TSceneIntro::RenderUI()
{
	TScene::RenderUI();
	//TEdit* pAccontBtn = (TEdit*)FindUI(L"ACCOUNT_BTN");
	//TDialogBox* pAccontPanel = (TDialogBox*)FindUI(L"ACCOUNT_PANEL");
	//TButton* pLoginBtn = (TButton*)FindUI(L"LOGIN_BTN");
	//TEdit* pIDEdit = (TEdit*)FindUI(L"IDEDIT");
	//TEdit* pPSEdit = (TEdit*)FindUI(L"PASSEDIT");
	//TButton* pExitBtn = (TButton*)FindUI(L"GAME_EXIT");
	//if (pAccontPanel->m_Attribute.iVisible >= 1)
	//{
	//	//pAccontPanel->m_bDrawDebugRect = true;
	//	pAccontPanel->Render();
	//}

	//if (pLoginBtn->m_Attribute.iVisible >= 1)
	//{
	//	pLoginBtn->Render();
	//}
	//if (pIDEdit->m_Attribute.iVisible >= 1)
	//{
	//	pIDEdit->Render();
	//}
	//if (pPSEdit->m_Attribute.iVisible >= 1)
	//{
	//	pPSEdit->Render();
	//}
	//if (pAccontBtn->m_Attribute.iVisible >= 1)
	//{
	//	pAccontBtn->Render();
	//}
	//if (pExitBtn->m_Attribute.iVisible >= 1)
	//{
	//	pExitBtn->Render();
	//}
	
	//TUserInterface* pNumber[4];
	//pNumber[0]= (TEdit*)FindUI(L"Number1000");
	//pNumber[1] = (TEdit*)FindUI(L"Number100");
	//pNumber[2] = (TEdit*)FindUI(L"Number10");
	//pNumber[3] = (TEdit*)FindUI(L"Number1");
	//for (int iNum = 0; iNum < 4; iNum++)
	//{
	//	pNumber[iNum]->Render();
	//}
	return true;
}
TSceneIntro::TSceneIntro()
{
	m_iSceneID = TScene::TSCENE_LOADING;
}
TSceneIntro::~TSceneIntro()
{

}