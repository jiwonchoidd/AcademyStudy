#pragma once
#include <d3d11.h>
#include <d3dx11.h>
#include <d3dx10math.h>

class TViewRT
{
public :
	ID3D11Texture2D*    m_pTexture;
	ID3D11ShaderResourceView* m_pSRV;
	ID3D11RenderTargetView*  m_pRTV;
public:
	// 后 咆胶贸 积己
	bool Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext,
		UINT iWidth = 1024,
		UINT iHeight = 1024);
	bool Release();
public:
	TViewRT();
	~TViewRT();
};
class TViewDS
{
public:
	ID3D11Texture2D*    m_pTexture;
	ID3D11DepthStencilView*  m_pDSV;
	ID3D11ShaderResourceView* m_pSRV;
	// 后 咆胶贸 积己
	bool Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext,
		UINT iWidth = 1024,
		UINT iHeight = 1024);
	bool Release();
public:
	TViewDS();
	~TViewDS();
};
class TViewPort
{
public:
	D3D11_VIEWPORT    m_VP;
public:
	void    Set(FLOAT fTopLeftX,
		FLOAT fTopLeftY,
		FLOAT fWidth,
		FLOAT fHeight,
		FLOAT fMinDepth,
		FLOAT fMaxDepth);
	void    Set(D3D11_VIEWPORT);
	void    Apply(ID3D11DeviceContext* pContext);
	bool Release() { return true; }
public:
	TViewPort() {};
	~TViewPort() {};
};
class TRenderTexture
{
public:
	TViewRT    m_RT;
	TViewDS    m_DS;
	TViewPort  m_VP;

	UINT					 m_iRT;
	D3D11_VIEWPORT			 m_OldVP;
	ID3D11DepthStencilView*  m_pOldDSV;
	ID3D11RenderTargetView*  m_pOldRTV;
public:
	bool    Begin(ID3D11DeviceContext* pContext, D3DXVECTOR4 clearColor= D3DXVECTOR4(1,1,1,1));
	bool    End(ID3D11DeviceContext* pContext);
	bool    Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext,
					UINT iWidth=1024, UINT iHeight=1024);
	bool    Release();
public:
	TRenderTexture();
	virtual ~TRenderTexture();
};

