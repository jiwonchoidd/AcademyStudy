#pragma once
#include "KScene_Intro.h" // intro
#include "KSoundManager.h"
//�� ��� ����
class KSceneManager : public Singleton <KSceneManager>
{
public:
	enum SceneID
	{
		S_INTRO,
		S_GAME_0,
		S_GAME_1,
		S_COMBAT,
	};
public:
	friend class Singleton<KSceneManager>;
public:
	KScene*						m_pCurrentScene;
	BYTE						m_CurrentScene_Index;
	ID3D11DeviceContext*		m_pContext;	// ����̽� ���ؽ�Ʈ ��ü
public:
	KSound*						m_BGM;
public:
	KScene* GetScene();
	bool	SetScene(BYTE index);
public:
	bool	Init(ID3D11DeviceContext* context);
	bool	Frame();
	bool	Render();
	bool	Release();
public:
	KSceneManager();
};

#define g_SceneManager KSceneManager::Get()