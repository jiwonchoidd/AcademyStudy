#pragma once
#include <KCore.h>
#include "KTexture.h"
#pragma comment	(lib, "D3DCompiler.lib")
struct SimpleVertex
{
	KVector3 pos;
	KVector4 color;
	KVector2 uv;
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
class Sample : public KCore
{
	float				m_pSpeed;
	KVector3			m_vCameraPos;
	KVector3			m_vCameraTarget;
	CB_DATA				m_cbData;
	ID3DBlob*			m_pVSBlob = nullptr;
	ID3D11Buffer*		m_pVertexBuffer;
	ID3D11Buffer*		m_pIndexBuffer;
	ID3D11Buffer*		m_pConstantBuffer;
	ID3D11InputLayout*	m_pVertexLayout;
	ID3D11VertexShader* m_pVS;
	ID3D11PixelShader*  m_pPS;
	std::vector< SimpleVertex> m_VertexList;
	std::vector< DWORD> m_IndexList;
public:
	KTexture		m_Texture;
	KMatrix			m_matWorld;
public:
	HRESULT		CreateConstantBuffer();
	HRESULT		CreateVertexBuffer();
	HRESULT		CreateIndexBuffer();
	HRESULT		CreateVertexLayout();
	HRESULT		LoadShader();
	bool		LoadObject(std::wstring filename);

public:
	bool   Init();
	bool   Frame();
	bool   Render();
	bool   Release();
public:
	Sample();
};

