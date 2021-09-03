#include "TShader.h"
//HRESULT WINAPI
//D3DCompile(__in_bcount(SrcDataSize) LPCVOID pSrcData,
//	__in SIZE_T SrcDataSize,
//	__in_opt LPCSTR pSourceName,
//	CONST D3D_SHADER_MACRO* pDefines,
//	__in_opt ID3DInclude* pInclude,
//	__in LPCSTR pEntrypoint,
//	__in LPCSTR pTarget,
//	__in UINT Flags1,
//	__in UINT Flags2,
//	__out ID3DBlob** ppCode,
//	__out_opt ID3DBlob** ppErrorMsgs);
HRESULT    TShader::Create(ID3D11Device* pDevice,
	TShaderBuffer* pBuffer,
	CHAR* pEntry, CHAR* pCompiler,
	SHADER_TYPE type)
{
	HRESULT hr = S_OK;
	DWORD dwFlags = 0;
#if defined(_DEBUG)
	dwFlags |= D3DCOMPILE_DEBUG;
#endif
	ID3DBlob* pErrorBuffer = 0;
	hr = D3DCompile(pBuffer->m_pByteData,
		pBuffer->m_FileSize.LowPart,
		"none",
		NULL, NULL,
		pEntry, pCompiler, dwFlags,
		0, 
		&m_pBlob, &pErrorBuffer);
	if (FAILED(hr))
	{
		OutputDebugStringA((char*)pErrorBuffer->GetBufferPointer());		
	}

	m_szName = pBuffer->m_szName + L"-" + mtw(pEntry);

	switch (type)
	{
		case t_VS: {
			hr = pDevice->CreateVertexShader(
				m_pBlob->GetBufferPointer(),
				m_pBlob->GetBufferSize(),
				NULL,
				&m_pVS);	
		}break;
		case t_GS: {
			hr = pDevice->CreateGeometryShader(
				m_pBlob->GetBufferPointer(),
				m_pBlob->GetBufferSize(),
				NULL,
				&m_pGS);		
		}break;
		case t_PS: {
			hr = pDevice->CreatePixelShader(
				m_pBlob->GetBufferPointer(),
				m_pBlob->GetBufferSize(),
				NULL,
				&m_pPS);		
		}break;
	}
	SAFE_RELEASE(pErrorBuffer);
	return hr;
}
HRESULT    TShader::Create(ID3D11Device* pDevice, 
	const TCHAR* pLoadFile,
					CHAR* pEntry, CHAR* pCompiler,
					SHADER_TYPE type)
{
	HRESULT hr;
	DWORD dwShaderFlags = D3DCOMPILE_SKIP_OPTIMIZATION;
#ifdef _DEBUG
	dwShaderFlags |= D3DCOMPILE_DEBUG;
#endif
	ID3DBlob* pBuffer = 0; // 컴파일한 결과의 스트링
	ID3DBlob* pErrorBuffer = 0;
	hr = D3DX11CompileFromFile(
		pLoadFile,
		NULL, NULL,
		pEntry, pCompiler,
		dwShaderFlags,
		NULL, NULL,
		&pBuffer, &pErrorBuffer, NULL);
	if (FAILED(hr))
	{
		OutputDebugStringA((char*)pErrorBuffer->GetBufferPointer());
	}

	m_szName =  mtw(pEntry);

	switch (type)
	{
		case t_VS: {
			hr = pDevice->CreateVertexShader(
				pBuffer->GetBufferPointer(),
				pBuffer->GetBufferSize(),
				NULL,
				&m_pVS);
			m_pBlob = pBuffer;
		}break;
		case t_GS: {
			hr = pDevice->CreateGeometryShader(
				pBuffer->GetBufferPointer(),
				pBuffer->GetBufferSize(),
				NULL,
				&m_pGS);
			m_pBlob = pBuffer;
		}break;
		case t_PS: {
			hr = pDevice->CreatePixelShader(
				pBuffer->GetBufferPointer(),
				pBuffer->GetBufferSize(),
				NULL,
				&m_pPS);
			SAFE_RELEASE(pBuffer);
		}break;
	}	
	SAFE_RELEASE(pErrorBuffer);
	return hr;
}
void	   TShader::ApplyVS(ID3D11DeviceContext* pContext)
{
	pContext->VSSetShader(m_pVS, NULL, 0);
}
void	   TShader::ApplyPS(ID3D11DeviceContext* pContext)
{
	pContext->PSSetShader(m_pPS, NULL, 0);
}
void	   TShader::ApplyGS(ID3D11DeviceContext* pContext)
{
	pContext->GSSetShader(m_pGS, NULL, 0);
}
bool	   TShader::Release()
{
	SAFE_RELEASE(m_pBlob);
	SAFE_RELEASE(m_pVS);
	SAFE_RELEASE(m_pPS);
	SAFE_RELEASE(m_pGS);
	return true;
}
TShader::TShader()
{
}


TShader::~TShader()
{
}
INT    TShaderMgr::Load(ID3D11Device* pDevice,
	const TCHAR* pLoadFile,
	CHAR* pEntry, CHAR* pCompiler,
	SHADER_TYPE type)
{
	HRESULT hr;
	// 파일에 전체 내용을 버퍼에 저장.
	INT iBufferIndex = I_ShaderBuffer.Load(pLoadFile);
	if (iBufferIndex < 0)
	{
		return -1;
	}
	TShaderBuffer* pBuffer = I_ShaderBuffer.GetPtr(iBufferIndex);

	//wstring entry = mtw(pEntry);
	wstring file = pLoadFile;
	file += mtw(pEntry);
	for (std::map< INT, TShader*>::iterator itor = m_Map.begin(); 
	itor != m_Map.end(); itor++)
	{
		TShader *pPoint = (TShader *)(*itor).second;
		if (pPoint->m_szName == file)
		{
			return (*itor).first;
		}
	}

	TShader* pShader = NULL;
	SAFE_NEW(pShader, TShader);
	/*if (SUCCEEDED(hr=pShader->Create(pDevice, pLoadFile,
					pEntry, pCompiler, type)))
	{
		pShader->m_szName = szFileName;
		m_Map.insert( make_pair(++m_iCurIndex, pShader));
	}*/
	if (SUCCEEDED(hr = pShader->Create(pDevice, 
		pBuffer,
		pEntry, pCompiler, type)))
	{
		m_Map.insert(make_pair(++m_iCurIndex, pShader));
	}
	return m_iCurIndex;
}
TShader*  TShaderMgr::GetPtr(INT iIndex)
{
	std::map< INT, TShader*>::iterator itor = m_Map.find(iIndex);
	if (itor == m_Map.end()) return NULL;

	TShader* pShader = (TShader*)(*itor).second;
	return pShader;
}
TShaderMgr::TShaderMgr()
{
	m_iCurIndex = -1;
}
TShaderMgr::~TShaderMgr()
{
	for (std::map< INT, TShader*>::iterator itor = m_Map.begin();
	itor != m_Map.end(); itor++)
	{
		TShader* pShader = (*itor).second;
		pShader->Release();
		SAFE_DEL(pShader);
	}
	m_Map.clear();
}