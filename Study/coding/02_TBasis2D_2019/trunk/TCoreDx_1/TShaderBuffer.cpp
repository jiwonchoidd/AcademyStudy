#include "TShaderBuffer.h"

HRESULT    TShaderBuffer::Load(const TCHAR* pLoadFile)
{
	HRESULT hr=S_OK;
	HANDLE hFile = CreateFile(pLoadFile,
		GENERIC_READ, FILE_SHARE_READ,
		NULL, OPEN_EXISTING, FILE_FLAG_SEQUENTIAL_SCAN, NULL);
	if (INVALID_HANDLE_VALUE == hFile)
	{
		return E_FAIL;
	}

	GetFileSizeEx(hFile, &m_FileSize);

	m_pByteData = new BYTE[m_FileSize.LowPart];
	if (m_pByteData == 0)
	{
		return E_OUTOFMEMORY;
	}
	DWORD byteRead;
	if (!ReadFile(hFile, m_pByteData, m_FileSize.LowPart, &byteRead, NULL) )
	{
		return E_FAIL;
	}
	CloseHandle(hFile);
	return hr;
}
bool TShaderBuffer::Release()
{
	delete[] m_pByteData;
	SAFE_RELEASE(m_pDataBlob);
	return true;
}
TShaderBuffer::TShaderBuffer()
{
	m_pByteData = 0;
	SAFE_ZERO(m_pDataBlob);
}
TShaderBuffer::~TShaderBuffer()
{
}
INT    TShaderBufferMgr::Load(const TCHAR* pLoadFile)
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

	for (std::map< INT, TShaderBuffer*>::iterator itor = m_Map.begin();
	itor != m_Map.end(); itor++)
	{
		TShaderBuffer *pPoint = (TShaderBuffer *)(*itor).second;
		if (!_tcsicmp(pPoint->m_szName.c_str(), szFileName))
		{
			return (*itor).first;
		}
	}

	TShaderBuffer* pShaderBuffer = NULL;
	SAFE_NEW(pShaderBuffer, TShaderBuffer);
	if (SUCCEEDED(hr = pShaderBuffer->Load(pLoadFile)))
	{
		pShaderBuffer->m_szName = szFileName;
		m_Map.insert(make_pair(++m_iCurIndex, pShaderBuffer));
	}
	return m_iCurIndex;
}
TShaderBuffer*  TShaderBufferMgr::GetPtr(INT iIndex)
{
	std::map< INT, TShaderBuffer*>::iterator itor = m_Map.find(iIndex);
	if (itor == m_Map.end()) return NULL;

	TShaderBuffer* pShader = (TShaderBuffer*)(*itor).second;
	return pShader;
}
TShaderBufferMgr::TShaderBufferMgr()
{
	m_iCurIndex = -1;
}
TShaderBufferMgr::~TShaderBufferMgr()
{
	for (std::map< INT, TShaderBuffer*>::iterator itor = m_Map.begin();
	itor != m_Map.end(); itor++)
	{
		TShaderBuffer* pShader = (*itor).second;
		pShader->Release();
		SAFE_DEL(pShader);
	}
	m_Map.clear();
}