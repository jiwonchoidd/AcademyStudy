#pragma once
#include "TUtils.h"
class TDxBuffer
{
public:
	UINT				m_BindFlags;
	UINT				m_iSize;
	UINT				m_iArrayCount;
	ID3D11Buffer*		m_pBuffer;
public:
	HRESULT    Create(ID3D11Device* pDevice,
		D3D11_USAGE Usage,
		UINT BindFlags,
		UINT iSize, UINT iCounter,
		void *pSysMem= NULL);
	// iShaderBind=0 => VS, 
	// iShaderBind=1 => PS, 	
	void	   Apply(	ID3D11DeviceContext* pContext, 
						int iOffset=0,
						int iShaderBind=0);
	bool	   Update(	ID3D11DeviceContext* pContext, 
						void* pData,  // 상수버퍼 주소
						UINT iSize);  // 상수버퍼 크기
	bool	   Release();
public:
	TDxBuffer();
	virtual ~TDxBuffer();
};

