#pragma once
#include "TDxBuffer.h"
#include "TShader.h"
#include "TTimer.h"
#include "TShape.h"

struct PCT
{
	float x, y, z; // 위치
	float r, g, b, a;
	float u, v;
};
struct VS_CONSTANT_BUFFER
{
	float x, y, z, w;
	float time; // 컬러 // 레지스터 단위 (float*4)
	float a, b, c;
};

class  TSprite : public TPlaneShape
{
public:	
	float   m_fLifeTime;
	float   m_fStepTime;
	float   m_fCurrentTime;
	bool    m_Loop;

	INT_VECTOR			m_iTexList;
	bool				SetTexArray(T_STR_ARRAY	 TexArray);
	TTimer				m_Timer;
	float				m_Color[4];
	INT					m_iTexIndex;
	ID3D11Device*		m_pd3dDevice;
	ID3D11DeviceContext*	m_pImmediateContext;
	//VS_CONSTANT_BUFFER m_Data;
	PCT					m_VertexList[6];
	DWORD				m_IndexList[6];
	VS_CONSTANT_BUFFER	m_ConstantList;
	TDxBuffer			m_VB;
	TDxBuffer			m_IB;
	TDxBuffer			m_CB;
	INT					m_iVS;
	INT					m_iPS;
	TShader*			m_pVS;
	TShader*			m_pPS;
	ID3D11InputLayout*  m_pIL;
public:
	bool	Create(ID3D11Device* pd3dDevice, 
		ID3D11DeviceContext*	pImmediateContext, RECT& rt,
		const TCHAR* pTextureName,
		const TCHAR* pVShaderName,
		const TCHAR* pPShaderName);
	void    SetRect(RECT& rt);
	
	void    SetLifeTime(float fLifeTime, bool bLoop);
	bool	Render();
	bool	Frame();
	bool	Release();
	TSprite();
	~TSprite();
};

