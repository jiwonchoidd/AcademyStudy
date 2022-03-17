#pragma once
#include "KObject.h"
#include "KObjectManager.h"
class KBoxObj : public KObject
{
public:
	bool Init(std::wstring vsfile, std::wstring psfile, std::wstring tex1=L"", std::wstring tex2 = L"", std::wstring tex3 = L"");
	bool Frame();
	void CreateTangentSpace(PNCT_VERTEX* v1, PNCT_VERTEX* v2, PNCT_VERTEX* v3);
	bool Render(ID3D11DeviceContext* pContext);
	bool Release();
public:
	KBoxObj();
	~KBoxObj();
};

