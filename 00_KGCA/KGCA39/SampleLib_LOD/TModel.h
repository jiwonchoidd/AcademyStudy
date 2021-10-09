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
	KVector4 vValue;
};
class TModel 
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
	std::vector< SimpleVertex> m_pVertexList;	
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
	HRESULT		LoadShader();
	bool		LoadObject(std::wstring filename);
	void		SeKMatrix(KMatrix* pMatWorld,
		KMatrix* pMatView, KMatrix* pMatProj);
public:
	bool   Init();	
	bool   Frame();
	bool   PreRender(ID3D11DeviceContext* pContext);
	bool   Render(ID3D11DeviceContext* pContext);
	bool   PostRender(ID3D11DeviceContext* pContext, UINT iNumIndex);
	bool   Release();
public:
	TModel();
};

