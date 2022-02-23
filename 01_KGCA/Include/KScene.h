#pragma once
#include "KObjectManager.h"
class KScene
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
	BYTE				 m_SceneID;
	ID3D11DeviceContext* m_pContext;	// 다비이스 컨텍스트 객체
	std::map<int, KObject*> m_UIObj;
	std::map<int, KObject*> m_MapObj;
	std::map<int, KObject*> m_ItemObj;
	std::map<int, KObject*> m_CharaterObj;
	using m_iter = std::map<int, KObject*>::iterator;
public:
	//순수 가상 함수들
	virtual bool	Load(std::wstring file) = 0;
public:
	virtual bool	Init(ID3D11DeviceContext* context);
	virtual bool	Frame();
	virtual bool	Render();
	virtual bool	Release();
public:
	KScene();
	virtual ~KScene();
};

