#include "TTexture.h"

HRESULT    TTexture::Create(ID3D11Device* pDevice,
	const TCHAR* pLoadFile)
{
	HRESULT hr = D3DX11CreateShaderResourceViewFromFile(
		pDevice, pLoadFile, NULL, NULL, &m_pSRV, NULL );
	return hr;
}
void	   TTexture::Apply(ID3D11DeviceContext* pContext)
{
	pContext->PSSetShaderResources(0, 1, &m_pSRV);
}
bool	   TTexture::Release()
{
	SAFE_RELEASE(m_pSRV);
	return true;
}
TTexture::TTexture()
{
}


TTexture::~TTexture()
{
}

INT    TTextureMgr::Load(ID3D11Device* pDevice,
	const TCHAR* pLoadFile)
{
	HRESULT hr;

	TCHAR szFileName[MAX_PATH];
	TCHAR Drive[MAX_PATH];
	TCHAR Dir[MAX_PATH];
	TCHAR FName[MAX_PATH];
	TCHAR Ext[MAX_PATH];
	if (pLoadFile == NULL) return -1;

	_tsplitpath_s(pLoadFile, Drive, Dir, FName, Ext);
	Ext[4] = 0;
	memset(szFileName, 0, sizeof(TCHAR) * MAX_PATH);
	_stprintf_s(szFileName, _T("%s%s"), FName, Ext);

	for (std::map< INT, TTexture*>::iterator itor = m_Map.begin();
	itor != m_Map.end(); itor++)
	{
		TTexture *pPoint = (TTexture *)(*itor).second;
		if (!_tcsicmp(pPoint->m_szName.c_str(), szFileName))
		{
			return (*itor).first;
		}
	}

	TTexture* pTexture = NULL;
	SAFE_NEW(pTexture, TTexture);
	if (SUCCEEDED(hr = pTexture->Create(pDevice, pLoadFile)))
	{
		pTexture->m_szName = szFileName;
		m_Map.insert(make_pair(++m_iCurIndex, pTexture));
	}
	return m_iCurIndex;
}
TTexture*  TTextureMgr::GetPtr(INT iIndex)
{
	std::map< INT, TTexture*>::iterator itor = m_Map.find(iIndex);
	if (itor == m_Map.end()) return NULL;

	TTexture* pShader = (TTexture*)(*itor).second;
	return pShader;
}
TTextureMgr::TTextureMgr()
{
	m_iCurIndex = -1;
}
TTextureMgr::~TTextureMgr()
{
	for (std::map< INT, TTexture*>::iterator itor = m_Map.begin();
	itor != m_Map.end(); itor++)
	{
		TTexture* pTexture = (*itor).second;
		pTexture->Release();
		SAFE_DEL(pTexture);
	}
	m_Map.clear();
}