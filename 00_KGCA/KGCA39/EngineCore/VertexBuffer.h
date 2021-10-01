#pragma once
#include <d3dcompiler.h>
#include "TStd.h"
#include "Matrix.h"
#pragma comment (lib, "D3DCompiler.lib")

struct NormalVertex 
{
	Vector3 pos;
	Vector4 color;
	NormalVertex(Vector3 p, Vector4 c)
	{
		pos = p;
		color = c;
	}
	NormalVertex()
	{
		pos = { 0.5f,0.5f,0.5f };
		color = { 0.5f,0.5f,0.5f,1.0f };
	}
};

struct CBuffer_DATA
{
	Matrix	matWorld;
	Matrix	matView;
	Matrix	matProj;
};

class VertexBuffer 
{
	//버퍼 필수 멤버 변수
	//상수 버퍼
	ID3D11Buffer* m_pConstBuffer;
	ID3D11InputLayout* m_pInputLayout;
	ID3D11VertexShader* m_pVertexShader;
	ID3D11PixelShader* m_pPixelShader;
	ID3D11Buffer* m_pVertexBuffer;
	ID3DBlob* m_pBlob = nullptr;
	CBuffer_DATA	m_ConstBufferData;
	Vector3			m_vCamPos;
	Vector3			m_vCamTarget;
	std::vector<NormalVertex> m_VertexList;
public:
	HRESULT	CreateConstBuffer();
	HRESULT	CreateVertexBuffer();
	HRESULT CreateInputLayout();
	HRESULT LoadShader();
	bool LoadFile(std::wstring filename);
public:
	bool	Init();
	bool	Frame();
	bool	Render();
	bool	Release();
public:
	VertexBuffer();
};

