#pragma once
#include "TUtils.h"
#include "TDxBuffer.h"
#include "TShader.h"

struct PCT_VERTEX
{
	D3DXVECTOR3 p; // �ġ
	D3DXVECTOR4 c;
	D3DXVECTOR2 t;
};
struct CB_VS_PER_OBJECT
{
	D3DXMATRIX m_matWorld;
	D3DXMATRIX m_matView;
	D3DXMATRIX m_matProj;
};
class TShape
{
public:
	std::vector<PCT_VERTEX>			m_VertexList;
	std::vector<DWORD>				m_IndexList;	
	CB_VS_PER_OBJECT				m_ConstantList;
	void				SetMatrix(D3DXMATRIX matWorld,
		D3DXMATRIX matView, 
		D3DXMATRIX matPro );
	TDxBuffer			m_VB;
	TDxBuffer			m_IB;	
	TDxBuffer			m_CB;

	INT					m_iVS;
	INT					m_iPS;
	TShader*			m_pVS;
	TShader*			m_pPS;
	ID3D11InputLayout*  m_pIL;
	INT					m_iTexIndex;
	ID3D11Device*		m_pd3dDevice;
	ID3D11DeviceContext*	m_pImmediateContext;
public:
	virtual bool		Init() {return true;};
	virtual bool		Frame() { return true; };
	virtual bool		Render() { return true; };
	virtual bool		Release() { return true; };
	virtual bool		Create(ID3D11Device* pd3dDevice,
		ID3D11DeviceContext*	pImmediateContext,
		const TCHAR* pTextureName,
		const TCHAR* pVShaderName,
		const TCHAR* pPShaderName) { return true; };
public:
	TShape();
	virtual ~TShape();
};
class TPlaneShape : public TShape
{
public:
	bool		Create(ID3D11Device* pd3dDevice,
		ID3D11DeviceContext*	pImmediateContext,
		const TCHAR* pTextureName,
		const TCHAR* pVShaderName,
		const TCHAR* pPShaderName);
	bool		Render();
	bool		Release();
public:
	TPlaneShape();
	virtual ~TPlaneShape();
};
class TBoxShape : public TShape
{
public:
	bool		Create(ID3D11Device* pd3dDevice,
		ID3D11DeviceContext*	pImmediateContext,
		const TCHAR* pTextureName,
		const TCHAR* pVShaderName,
		const TCHAR* pPShaderName);
	bool CreateVertexBuffer(
		D3DXVECTOR3 vMan, D3DXVECTOR3 vMin);
	bool		Render();
	bool		Release();
public:
	TBoxShape();
	virtual ~TBoxShape();
};
class TSphereShape : public TShape
{

};
class TLineShape : public TShape
{
public:
	bool		Create(ID3D11Device* pd3dDevice,
		ID3D11DeviceContext*	pImmediateContext,
		const TCHAR* pTextureName,
		const TCHAR* pVShaderName,
		const TCHAR* pPShaderName);
	bool		Render();
	bool		Release();
public:
	TLineShape();
	virtual ~TLineShape();
};
