#include "KShaderManager.h"
HRESULT	KShader::Load(std::wstring vs, std::wstring ps)
{
	HRESULT hr = S_OK;
	ID3DBlob* error = nullptr;
	m_pVSCodeResult = LoadShaderBlob(vs, "VS", "vs_5_0");
	if (m_pVSCodeResult.Get() == nullptr)
	{
		return hr;
	}
	hr = g_pd3dDevice->CreateVertexShader(
		m_pVSCodeResult.Get()->GetBufferPointer(),
		m_pVSCodeResult.Get()->GetBufferSize(),
		NULL, m_pVertexShader.GetAddressOf());
	if (FAILED(hr)) return hr;

	m_pPSCodeResult = LoadShaderBlob(ps, "PS", "ps_5_0");
	if (m_pPSCodeResult.Get() == nullptr)
	{
		return hr;
	}
	hr = g_pd3dDevice->CreatePixelShader(
		m_pPSCodeResult.Get()->GetBufferPointer(),
		m_pPSCodeResult.Get()->GetBufferSize(),
		NULL, m_pPixelShader.GetAddressOf());
	if (FAILED(hr)) return hr;

	return hr;
}

ID3DBlob* KShader::LoadShaderBlob(std::wstring vs, std::string function, std::string version)
{
	HRESULT hr = S_OK;
	ID3DBlob* ret = nullptr;
	ID3DBlob* error = nullptr;
	hr = D3DCompileFromFile(
		vs.c_str(),
		nullptr,
		nullptr,
		function.c_str(),
		version.c_str(),
		0,
		0,
		&ret,
		&error);
	if (FAILED(hr))
	{
		MessageBoxA(NULL,
			(char*)error->GetBufferPointer(),
			"error", MB_OK);
		return ret;
	}
	return ret;
}

bool	KShader::Init()
{
	return true;
}
bool	KShader::Frame()
{
	return true;
}
bool	KShader::Render()
{
	return true;
}
bool	KShader::Release()
{
	if (m_pVertexShader) m_pVertexShader.Reset();
	if (m_pPixelShader) m_pPixelShader.Reset();
	m_pVertexShader = nullptr;
	m_pPixelShader = nullptr;
	return true;
}
bool KShader::CreateVertexShader(std::wstring filename, std::string entry)
{
	m_pVSCodeResult = LoadShaderBlob(filename, entry, "vs_5_0");
	if (m_pVSCodeResult.Get() == nullptr)
	{
		return false;
	}
	HRESULT hr = g_pd3dDevice->CreateVertexShader(
		m_pVSCodeResult.Get()->GetBufferPointer(),
		m_pVSCodeResult.Get()->GetBufferSize(),
		NULL, m_pVertexShader.GetAddressOf());

	if (FAILED(hr))
	{
		return false;
	}
	return true;
}
bool KShader::CreatePixelShader(std::wstring filename, std::string entry)
{
	m_pPSCodeResult = LoadShaderBlob(filename, entry, "ps_5_0");
	if (m_pPSCodeResult.Get() == nullptr)
	{
		return false;
	}
	HRESULT hr = g_pd3dDevice->CreatePixelShader(
		m_pPSCodeResult.Get()->GetBufferPointer(),
		m_pPSCodeResult.Get()->GetBufferSize(),
		NULL, m_pPixelShader.GetAddressOf());

	if (FAILED(hr))
	{
		return false;
	}
	return true;
}
KShader::KShader()
{
	m_iIndex = 0;
	m_szBuffer[256] = { 0, };
}
KShader::~KShader()
{
	if (m_pVertexShader) m_pVertexShader.Reset();
	if (m_pPixelShader) m_pPixelShader.Reset();
	m_pVertexShader = nullptr;
	m_pPixelShader = nullptr;
}

KShader* KShaderManager::CreateVertexShader(std::wstring filename, std::string entry)
{
	std::wstring name = Splitpath(filename, to_mw(entry));
	//�ߺ� ����
	KShader* pData = GetPtr(name);
	if (pData != nullptr)
	{
		return pData;
	}
	pData = new KShader;
	if (!pData->CreateVertexShader(filename, entry))
	{
		delete pData;
		return nullptr;
	}
	pData->m_csName = name;
	m_list.insert(make_pair(pData->m_csName, pData));
	m_iIndex++;
	return pData;
}
KShader* KShaderManager::CreatePixelShader(std::wstring filename, std::string entry)
{
	std::wstring name = Splitpath(filename, to_mw(entry));
	KShader* pData = GetPtr(name);
	if (pData != nullptr)
	{
		return pData;
	}
	pData = new KShader;
	if (!pData->CreatePixelShader(filename, entry))
	{
		delete pData;
		return nullptr;
	}
	pData->m_csName = name;
	m_list.insert(make_pair(pData->m_csName, pData));
	m_iIndex++;
	return pData;
}
KShaderManager::KShaderManager()
{
	m_iIndex = 0;
}
KShaderManager::~KShaderManager()
{
	Release();
}