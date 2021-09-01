#pragma once
#pragma comment(lib,"TCoreDx_0.lib")
#include "TCoreDx.h"
#include "TDxBuffer.h"
#include "TShader.h"
//#define USED_ONLY_VERTEX
#define MAX_OBJECT 3
struct P_VERTEX
{
	float x, y, z; // 위치
};
struct VS_CONSTANT_BUFFER
{
	float x, y, z, w;
	float time; // 컬러 // 레지스터 단위 (float*4)
	float a, b, c;
};
struct TObject
{
	VS_CONSTANT_BUFFER m_Data;
	P_VERTEX			m_VertexList[6];			
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
};
class Sample : public TCoreDx
{
	TObject				m_Object[MAX_OBJECT];	
	void    SetRect( RECT& rt, int iObj);
public:
	bool	Init();
	bool	Frame();
	bool	Render();
	bool	Release();
public:
	Sample();
	virtual ~Sample();
};

