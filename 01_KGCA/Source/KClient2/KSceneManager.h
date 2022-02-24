#pragma once
#include "KScene_Intro.h"
#include "KScene_Game_0.h"
//�� ��� ����
class KSceneManager : public Singleton <KSceneManager>
{
public:
	enum SceneID
	{
		S_LOAD,
		S_MENU,
		S_GAME,
		S_RESULT,
	};
public:
	friend class Singleton<KSceneManager>;
public:
	KScene*						m_pCurrentScene;
	BYTE						m_CurrentScene_Index;
	ID3D11DeviceContext*		m_pContext;	// ����̽� ���ؽ�Ʈ ��ü
public:
	bool	m_bChangeScene;
	float	m_Timer;
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