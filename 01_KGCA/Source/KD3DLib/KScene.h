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
	std::map<int, KObject*> m_UIobj;
	std::map<int, KObject*> m_Map;
	std::map<int, KObject*> m_Item;
	std::map<int, KObject*> m_Charater;
	using m_iter = std::map<int, KObject*>::iterator;
public:
	//순수 가상 함수들
	virtual bool	Load(std::wstring file) = 0;
public:
	virtual bool	Init(ID3D11DeviceContext* context) = 0;
	virtual bool	Frame() = 0;
	virtual bool	Render() = 0;
	virtual bool	Release() = 0;
public:
	KScene();
	virtual ~KScene();
};

