#pragma once
#pragma comment(lib,"TCoreDx_0.lib")
#include "TCoreDx.h"

//#define USED_ONLY_VERTEX
#define MAX_OBJECT 4
#define MAX_TEXTURE 10
struct P_VERTEX
{
	float x, y, z; // 위치
	float u, v;
};
struct VS_CONSTANT_BUFFER
{
	float x, y, z, w;
	float time; // 컬러 // 레지스터 단위 (float*4)
	float a, b, c;
};
struct TObject
{
	
	VS_CONSTANT_BUFFER  m_Data;
	P_VERTEX			m_VertexList[6];
	ID3D11Buffer*		m_pVB;

	DWORD				m_IndexList[6];
	ID3D11Buffer*		m_pIB;

	ID3D11Buffer*		m_pCB;//ConstantBuffer
	VS_CONSTANT_BUFFER	m_ConstantList;

	ID3D11VertexShader* m_pVS;
	ID3D11PixelShader*  m_pPS;
	ID3D11InputLayout*  m_pIL;
	ID3D11ShaderResourceView* m_pTexSRV;
	ID3D11SamplerState*		  m_pTexSS;
};

namespace TInputLayout
{
	static ID3D11InputLayout*     g_pP_IL;
	static ID3D11InputLayout*     g_pPT_IL;
	static ID3D11InputLayout*     g_pPNCT_IL;
	static ID3D11InputLayout*     g_pPC_IL;
	static ID3D11InputLayout*     g_pPNT_IL;
}

class Sample : public TCoreDx
{
public:
	float				m_fEffectTime;
	D3D11_VIEWPORT		 m_vp;

	TObject				m_Object[MAX_OBJECT];
	ID3D11ShaderResourceView* m_pTexArraySRV[10];
	int					m_iIndex;
	ID3D11VertexShader* m_pVS;
	ID3D11PixelShader*  m_pPS;
	ID3D11InputLayout*  m_pIL;
	DWORD				m_IndexList[6];
	ID3D11Buffer*		m_pIB;
	ID3D11Buffer*		m_pCB;//ConstantBuffer
	void    SetRect(RECT& rt, int iObj, float fDepth= 0.5f);
	void    SetRectUV(RECT& rt, int iObj);
public:
	bool	Init();
	bool	Frame();
	bool	Render();
	bool	Release();
public:
	Sample();
	virtual ~Sample();
};

