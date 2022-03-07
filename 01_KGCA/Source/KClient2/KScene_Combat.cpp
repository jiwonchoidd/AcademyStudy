#include "KScene_Combat.h"
#include "KSceneManager.h"
#include "KUI.h"
#include "KWrite.h"
bool KScene_Combat::Load(std::wstring file)
{
#pragma region 사용할 모델 생성
	// 배경
	std::shared_ptr<KImage> combat_background(new KImage);
	combat_background->m_Name = L"combat_background";
	combat_background->SetRectDraw({ 0,0,g_rtClient.right, g_rtClient.bottom});
	combat_background->SetPosition(KVector2(g_rtClient.right/2, g_rtClient.bottom/2));
	combat_background->SetCollisionType(KCollisionType::Ignore, KSelectType::Select_Ignore);
	combat_background->m_rtOffset = { 0, 0, 0, 0 };
	if (!combat_background->Init(m_pContext,
		L"../../data/shader/VS_UI_Move.txt",
		L"../../data/shader/PS_UI_0.txt",
		L"../../data/texture/combat_background.png",
		L""))
	{
		return false;
	}
	//대화상자
	combat_background->m_bMoveImg = true;
	m_UIObj.push_back(std::shared_ptr<KObject>(combat_background));

	std::shared_ptr<KImage> dialog_background(new KImage);
	dialog_background->m_Name = L"dialog_background";
	dialog_background->SetRectDraw({ 0,0,g_rtClient.right, g_rtClient.bottom /4});
	dialog_background->SetPosition(KVector2(g_rtClient.right / 2, g_rtClient.bottom-70));
	dialog_background->SetCollisionType(KCollisionType::Ignore, KSelectType::Select_Ignore);
	dialog_background->m_rtOffset = { 10, 10, 10, 10 };
	if (!dialog_background->Init(m_pContext,
		L"../../data/shader/VS_UI_0.txt",
		L"../../data/shader/PS_UI_0.txt",
		L"../../data/texture/DS DSi - Pokemon Diamond Pearl - Text Boxes.png",
		L""))
	{
		return false;
	}
	m_UIObj.push_back(std::shared_ptr<KObject>(dialog_background));
	// 발판
	std::shared_ptr<KImage> combat_stepimg(new KImage);
	combat_stepimg->m_Name = L"combat_step";
	combat_stepimg->SetRectDraw({ 0,0,128, 40});
	combat_stepimg->SetPosition(KVector2(g_rtClient.right / 2, g_rtClient.bottom / 2));
	combat_stepimg->SetCollisionType(KCollisionType::Ignore, KSelectType::Select_Ignore);
	combat_stepimg->m_rtOffset = { 0, 0, 0, 0 };
	if (!combat_stepimg->Init(m_pContext,
		L"../../data/shader/VS_UI_0.txt",
		L"../../data/shader/PS_UI_0.txt",
		L"../../data/texture/combat_foot.png",
		L""))
	{
		return false;
	}
	g_UIModelManager.m_list.insert(std::make_pair(L"combat_step", combat_stepimg));
	// 
	//플레이어
	//대화창
	
#pragma endregion

	m_BGM = g_SoundManager.LoadSound(L"../../data/sound/bgm/Twinleaf Town (Day).wav");
	m_BGM->SoundPlay(true);

	return true;
}

bool KScene_Combat::Init(ID3D11DeviceContext* context)
{
	//상속된 씬 초기화
	KScene::Init(context);
	//현재 씬 열거형 타입 지정
	m_SceneID = S_COMBAT;

	return true;
}

bool KScene_Combat::Frame()
{
	m_BGM->Frame();
	//플레이어 이동

	KScene::Frame();
	return true;
}

bool KScene_Combat::Render()
{

	KScene::Render();
	RECT  rt = { 0, 0, g_rtClient.right, g_rtClient.bottom };
	g_Write->RenderText(rt, L"test",
		D2D1::ColorF(0, 0, 0, 1));
	return true;
}

bool KScene_Combat::Release()
{
	m_BGM->SoundStop();
	KScene::Release();
	return true;
}
