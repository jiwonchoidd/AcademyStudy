#include "KModel.h"
void		KModel::SetMatrix(
    KMatrix* pMatWorld,
    KMatrix* pMatView, KMatrix* pMatProj)
{
    if (pMatWorld != nullptr)
    {
        m_cbData.matWorld = pMatWorld->Transpose();
    }
    if (pMatView != nullptr)
    {
        m_cbData.matView = pMatView->Transpose();
    }
    if (pMatProj != nullptr)
    {
        m_cbData.matProj = pMatProj->Transpose();
    }
}
bool  KModel::LoadObject(std::wstring filename)
{
    FILE* fp = nullptr;
    _tfopen_s(&fp, filename.c_str(), _T("rt"));
    if (fp == NULL)
    {
        return false;
    }
    TCHAR buffer[256] = { 0, };
    int iVersion = 0;
    _fgetts(buffer, 256, fp);
    TCHAR tmp[256] = { 0, };
    _stscanf_s(buffer, _T("%s%d"), tmp, _countof(tmp), &iVersion);

    int iNumVertex = 0;
    _fgetts(buffer, 256, fp);
    _stscanf_s(buffer, _T("%d"), &iNumVertex);

    int index = 0;
    for (int iLine = 0; iLine < iNumVertex; iLine++)
    {
        PNCT_VERTEX v;
        _fgetts(buffer, 256, fp);
        _stscanf_s(buffer, _T("%d %f %f %f %f %f %f %f"),
            &index,
            &v.pos.x, &v.pos.y, &v.pos.z,
            &v.color.x, &v.color.y, &v.color.z, &v.color.w);
        m_pVertexList.push_back(v);
    }
    fclose(fp);
    return true;
}
KModel::KModel()
{
    m_pVertexBuffer = nullptr;
    m_pVertexLayout = nullptr;
    m_pIndexBuffer = nullptr;
    m_pConstantBuffer = nullptr;
    m_pVS = nullptr;
    m_pPS = nullptr;
    m_iNumIndex = 0;
}
HRESULT KModel::CreateConstantBuffer()
{
    HRESULT hr = S_OK;
    D3D11_BUFFER_DESC bd;
    ZeroMemory(&bd, sizeof(D3D11_BUFFER_DESC));
    bd.ByteWidth = sizeof(CB_DATA);
    bd.Usage = D3D11_USAGE_DEFAULT;
    bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    D3D11_SUBRESOURCE_DATA data;
    ZeroMemory(&data, sizeof(D3D11_SUBRESOURCE_DATA));
    data.pSysMem = &m_cbData;
    hr = g_pd3dDevice->CreateBuffer(&bd, &data, &m_pConstantBuffer);
    if (FAILED(hr)) return hr;
    return hr;
}
HRESULT KModel::CreateVertexBuffer()
{
    HRESULT hr = S_OK;
    if (m_pVertexList.size() <= 0) return hr;
    D3D11_BUFFER_DESC bd;
    ZeroMemory(&bd, sizeof(D3D11_BUFFER_DESC));
    bd.ByteWidth = sizeof(PNCT_VERTEX) * m_pVertexList.size();
    bd.Usage = D3D11_USAGE_DEFAULT;
    bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    D3D11_SUBRESOURCE_DATA data;
    ZeroMemory(&data, sizeof(D3D11_SUBRESOURCE_DATA));
    data.pSysMem = &m_pVertexList.at(0);
    hr = g_pd3dDevice->CreateBuffer(&bd, &data, &m_pVertexBuffer);
    if (FAILED(hr)) return hr;
    return hr;
}

HRESULT KModel::CreateIndexBuffer()
{
    HRESULT hr = S_OK;
    if (m_IndexList.size() <= 0) return hr;
    D3D11_BUFFER_DESC bd;
    ZeroMemory(&bd, sizeof(D3D11_BUFFER_DESC));
    bd.ByteWidth = sizeof(DWORD) * m_IndexList.size();
    bd.Usage = D3D11_USAGE_DEFAULT;
    bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
    D3D11_SUBRESOURCE_DATA data;
    ZeroMemory(&data, sizeof(D3D11_SUBRESOURCE_DATA));
    data.pSysMem = &m_IndexList.at(0);
    hr = g_pd3dDevice->CreateBuffer(&bd, &data, &m_pIndexBuffer);
    if (FAILED(hr)) return hr;

    m_iNumIndex = m_IndexList.size();
    return hr;
}
HRESULT KModel::CreateVertexLayout()
{
    HRESULT hr = S_OK;
    D3D11_INPUT_ELEMENT_DESC layout[] =
    {
        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,  0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "COLOR",    0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "TEXTURE", 0, DXGI_FORMAT_R32G32_FLOAT,    0, 40, D3D11_INPUT_PER_VERTEX_DATA, 0 },
    };
    hr = g_pd3dDevice->CreateInputLayout(layout, _countof(layout),
        m_pVSBlob->GetBufferPointer(),
        m_pVSBlob->GetBufferSize(),
        &m_pVertexLayout);
    if (FAILED(hr)) return hr;

    m_pVSBlob->Release();
    return hr;
}

HRESULT KModel::LoadShader(std::wstring vsFp, std::wstring psFp)
{
    HRESULT hr = S_OK;
    ID3DBlob* error = nullptr;
    hr = D3DCompileFromFile(
        vsFp.c_str(),
        nullptr,
        nullptr,
        "VS",
        "vs_5_0",
        0,
        0,
        &m_pVSBlob,
        &error);
    if (FAILED(hr))
    {
        MessageBoxA(NULL,
            (char*)error->GetBufferPointer(),
            "error", MB_OK);
        return hr;
    }

    hr = g_pd3dDevice->CreateVertexShader(
        m_pVSBlob->GetBufferPointer(),
        m_pVSBlob->GetBufferSize(),
        NULL, &m_pVS);
    if (FAILED(hr)) return hr;

    ID3DBlob* PSBlob = nullptr;
    hr = D3DCompileFromFile(
        psFp.c_str(),
        nullptr,
        nullptr,
        "PS",
        "ps_5_0",
        0,
        0,
        &PSBlob,
        nullptr);
    if (FAILED(hr)) return hr;
    hr = g_pd3dDevice->CreatePixelShader(
        PSBlob->GetBufferPointer(),
        PSBlob->GetBufferSize(),
        NULL, &m_pPS);
    if (FAILED(hr)) return hr;
    PSBlob->Release();
    return hr;
}
bool	KModel::CreateVertexData()
{
    if (m_pVertexList.size() > 0)
    {
        return true;
    }
    return false;
}
bool	KModel::CreateIndexData()
{
    if (m_IndexList.size() > 0)
    {
        return true;
    }
    return false;
}
bool KModel::Init()
{    
    return true;
}

bool KModel::CreateModel(std::wstring vsFile, std::wstring psFile)
{
    //���� ����
    CreateConstantBuffer();

    if (CreateVertexData())
    {
        CreateVertexBuffer();
    }
    if (CreateIndexData())
    {
        CreateIndexBuffer();
    }
    if (SUCCEEDED(LoadShader(vsFile, psFile)))
    {
        if (SUCCEEDED(CreateVertexLayout()))
        {
            return true;
        }
    }
    return false;
}

bool KModel::Frame()
{     
    m_cbData.vValue.z = g_fTimer;
    return true;
}

bool KModel::Render(ID3D11DeviceContext* pContext)
{
    if (PreRender(pContext) ==false) return false;
    if (PostRender(pContext, m_iNumIndex) == false) return false;
    return true;
}
bool KModel::PreRender(ID3D11DeviceContext* pContext)
{
    if (m_pVertexList.size() <= 0) return true;

    pContext->UpdateSubresource(
        m_pConstantBuffer, 0, NULL, &m_cbData, 0, 0);
    pContext->VSSetConstantBuffers(
        0, 1, &m_pConstantBuffer);
    pContext->VSSetShader(m_pVS, NULL, 0);
    pContext->PSSetShader(m_pPS, NULL, 0);
    pContext->IASetInputLayout(m_pVertexLayout);
    UINT pStrides = sizeof(PNCT_VERTEX);
    UINT pOffsets = 0;
    pContext->IASetVertexBuffers(0, 1, &m_pVertexBuffer,
        &pStrides, &pOffsets);
    pContext->IASetIndexBuffer(m_pIndexBuffer,
        DXGI_FORMAT_R32_UINT, 0);
    return true;
}
bool KModel::PostRender(ID3D11DeviceContext* pContext, UINT iNumIndex)
{
    if (m_IndexList.size() > 0)
    {
        pContext->DrawIndexed(iNumIndex, 0, 0);
    }
    else
    {
        pContext->Draw(m_pVertexList.size(), 0);
    }
    return false;
}
bool KModel::Release()
{
    if (m_pVertexBuffer)m_pVertexBuffer->Release();
    if (m_pIndexBuffer)m_pIndexBuffer->Release();
    if (m_pVertexLayout)m_pVertexLayout->Release();
    if (m_pConstantBuffer)m_pConstantBuffer->Release();
    if (m_pVS)m_pVS->Release();
    if (m_pPS)m_pPS->Release();
    m_pIndexBuffer = nullptr;
    m_pVertexLayout = nullptr;
    m_pConstantBuffer = nullptr;
    m_pVS = nullptr;
    m_pPS = nullptr;
    return false;
}
