#pragma once
#include "KObject.h"
#include "KObjectManager.h"
class KBoxObj : public KObject
{
public:
	bool Init(std::wstring vsfile, std::wstring psfile,
		std::wstring texturefile=nullptr, std::wstring normalfile = nullptr);
	bool Frame();
	bool Render(ID3D11DeviceContext* pContext);
	bool Release();
public:
	KBoxObj();
	~KBoxObj();
};

