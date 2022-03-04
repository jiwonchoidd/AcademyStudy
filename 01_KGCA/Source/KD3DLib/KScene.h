#pragma once
#include "KObjectManager.h"
class KScene
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
	BYTE				 m_SceneID;
	ID3D11DeviceContext* m_pContext;	// �ٺ��̽� ���ؽ�Ʈ ��ü
	std::vector<KObject*> m_UIObj;
	std::vector<KObject*> m_MapObj;
	std::vector<KObject*> m_ItemObj;
	std::vector<KObject*> m_CharaterObj;
	using m_iter = std::vector<KObject*>::iterator;
public:
	//���� ���� �Լ���
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

