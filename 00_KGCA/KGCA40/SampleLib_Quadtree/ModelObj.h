#pragma once
#include "TStd.h"
struct SimpleVertex
{
	KVector3 pos;
	KVector4 color;
	SimpleVertex()
	{
		color.x = 1.0f;
		color.y = 1.0f;
		color.z = 1.0f;
		color.w = 1.0f;
	}
};
struct CB_DATA
{
	KMatrix  matWorld;
	KMatrix  matView;
	KMatrix  matProj;
};
class ModelObj
{
public:
	CB_DATA				m_cbData;
	//인덱스 버퍼
	ID3DBlob* m_pVSBlob = nullptr;
	ID3D11Buffer* m_pVertexBuffer;
	ID3D11Buffer* m_pConstantBuffer;
	ID3D11Buffer* m_pIndexBuffer;
	ID3D11InputLayout* m_pVertexLayout;
	ID3D11VertexShader* m_pVS;
	ID3D11PixelShader* m_pPS;
	//버텍스리스트 인덱스리스트
	std::vector<SimpleVertex> m_VertexList;
	std::vector<DWORD> m_IndexList;
	UINT				m_iNumIndex;
public:
	KMatrix		m_matWorld;
public:
	virtual bool	CreateIndexData();
	virtual bool	CreateVertexData();
	HRESULT		CreateIndexBuffer();
	HRESULT		CreateConstantBuffer();
	HRESULT		CreateVertexBuffer();
	HRESULT		CreateVertexLayout();
	HRESULT		LoadShader();
	bool		LoadObject(std::wstring filename);
	void		SetMatrix(KMatrix* pMatWorld,
		KMatrix* pMatView, KMatrix* pMatProj);
public:
	bool   Init();
	bool   Frame();
	bool   PreRender(ID3D11DeviceContext* pContext);
	bool   Render(ID3D11DeviceContext* pContext);
	bool   PostRender(ID3D11DeviceContext* pContext, UINT iNumIndex);
	bool   Release();
public:
	ModelObj();
};

