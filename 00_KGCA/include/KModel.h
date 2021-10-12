#pragma once
#include "TStd.h"

struct PC_VERTEX
{
	KVector3 pos;
	KVector4 color;
	PC_VERTEX()
	{
		color.x = 1.0f;
		color.y = 1.0f;
		color.z = 1.0f;
		color.w = 1.0f;
	}
};
struct PCT_VERTEX
{
	KVector3 pos;
	KVector4 color;
	KVector2 tex;
	PCT_VERTEX()
	{
		color.x = 1.0f;
		color.y = 1.0f;
		color.z = 1.0f;
		color.w = 1.0f;
	}
};
struct PNCT_VERTEX
{
	KVector3 pos;
	KVector3 normal;
	KVector4 color;
	KVector2 tex;
	PNCT_VERTEX()
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
	KVector4 vValue;
};
class KModel 
{
public:
	UINT				m_iNumIndex;
	CB_DATA				m_cbData;
	ID3DBlob* m_pVSBlob = nullptr;
	ID3D11Buffer* m_pVertexBuffer;
	ID3D11Buffer* m_pIndexBuffer;
	ID3D11Buffer* m_pConstantBuffer;
	ID3D11InputLayout* m_pVertexLayout;
	ID3D11VertexShader* m_pVS;
	ID3D11PixelShader* m_pPS;
	std::vector< PC_VERTEX> m_pVertexList;
	std::vector< DWORD> m_IndexList;
public:
	KMatrix		m_matWorld;
public:
	virtual bool	CreateVertexData();
	virtual bool	CreateIndexData();
	HRESULT		CreateConstantBuffer();
	HRESULT		CreateVertexBuffer();
	HRESULT		CreateIndexBuffer();
	HRESULT		CreateVertexLayout();
	HRESULT		LoadShader(std::wstring vsFile, std::wstring psFile);
	bool		LoadObject(std::wstring filename);
	void		SetMatrix(KMatrix* pMatWorld,
		KMatrix* pMatView, KMatrix* pMatProj);
public:
	bool		CreateModel(std::wstring vsFile, std::wstring psFile);
	bool   Init();	
	bool   Frame();
	bool   PreRender(ID3D11DeviceContext* pContext);
	bool   Render(ID3D11DeviceContext* pContext);
	bool   PostRender(ID3D11DeviceContext* pContext, UINT iNumIndex);
	bool   Release();
public:
	KModel();
};

