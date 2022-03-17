#pragma once
#include "KObject.h"
#include "KObjectManager.h"
class KBoxObj : public KObject
{
public:
	bool Init(std::wstring vsfile, std::wstring psfile, std::wstring tex1=L"", std::wstring tex2 = L"", std::wstring tex3 = L"");
	bool Frame();
	void CreateTangentSpace(KVector3* v1, KVector3* v2, KVector3* v3, KVector2* uv1, KVector2* uv2, KVector2* uv3,KVector3* normal, KVector3* tagent, KVector3* binormal);
	bool Render(ID3D11DeviceContext* pContext);
	bool Release();
public:
	KBoxObj();
	~KBoxObj();
};

