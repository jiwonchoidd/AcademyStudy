#pragma once
#include "KStd.h"
class KDepthStencil
{
	ID3D11DepthStencilView*		m_pDepthStenV;
	ID3D11DepthStencilState*	m_pDepthStenS;
	ID3D11BlendState*			m_pBlendState;
	ID3D11DeviceContext*		m_context;
	ID3D11RenderTargetView*		m_target;
public:
	virtual bool		Init(ID3D11DeviceContext* context, ID3D11RenderTargetView* target);
	virtual bool		Frame();
	virtual bool		PreRender();
	virtual bool		Release();
public:
	HRESULT CreateDepthStenView();
	HRESULT CreateDepthStenState();
	HRESULT CreateBlendState();

public:
	KDepthStencil();
	virtual ~KDepthStencil();
};

