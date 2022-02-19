#pragma once
#include "KScene_Intro.h"
#include "KScene_Game_0.h"
//씬 헤더 포함
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
	KScene*				 m_pCurrentScene;
	BYTE				 m_CurrentScene_Index;
	ID3D11DeviceContext* m_pContext;	// 디바이스 컨텍스트 객체
public:
	bool	m_bChangeScene;
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