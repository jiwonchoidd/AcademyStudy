#include "ModelObj.h"
void ModelObj::SetMatrix (KMatrix* pMatWorld, KMatrix* pMatView, KMatrix* pMatProj)
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
bool  ModelObj::LoadObject(std::wstring filename)
{
    FILE* file = nullptr;
    _tfopen_s(&file, filename.c_str(), _T("rt"));
    if (file == NULL)
    {
        return false;
    }
    TCHAR buffer[256] = { 0, };
    int iVersion = 0;
    _fgetts(buffer, 256, file);
    TCHAR tmp[256] = { 0, };
    _stscanf_s(buffer, _T("%s%d"), tmp, _countof(tmp), &iVersion);

    int iNumVertex = 0;
    _fgetts(buffer, 256, file);
    _stscanf_s(buffer, _T("%d"), &iNumVertex);

    int index = 0;
    for (int iLine = 0; iLine < iNumVertex; iLine++)
    {
        SimpleVertex v;
        _fgetts(buffer, 256, file);
        _stscanf_s(buffer, _T("%d %f %f %f %f %f %f %f"),
            &index,
            &v.pos.x, &v.pos.y, &v.pos.z,
            &v.color.x, &v.color.y, &v.color.z, &v.color.w);
        m_VertexList.push_back(v);
    }
    fclose(file);
    return true;
}
ModelObj::ModelObj()
{
    m_pVertexBuffer = nullptr;
    m_pVertexLayout = nullptr;
    m_pVS = nullptr;
    m_pPS = nullptr;
}
HRESULT ModelObj::CreateConstantBuffer()
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
HRESULT ModelObj::CreateVertexBuffer()
{
    HRESULT hr = S_OK;
    // 로칼->월드->뷰->투영좌표계(NDC)->화면
    // 그래픽 카드 메모리로 보내야 한다.
    D3D11_BUFFER_DESC bd;
    ZeroMemory(&bd, sizeof(D3D11_BUFFER_DESC));
    bd.ByteWidth = sizeof(SimpleVertex) * m_VertexList.size();
    bd.Usage = D3D11_USAGE_DEFAULT;
    bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    D3D11_SUBRESOURCE_DATA data;
    ZeroMemory(&data, sizeof(D3D11_SUBRESOURCE_DATA));
    data.pSysMem = &m_VertexList.at(0);
    hr = g_pd3dDevice->CreateBuffer(&bd, &data, &m_pVertexBuffer);
    if (FAILED(hr)) return hr;
    return hr;
}

HRESULT ModelObj::CreateVertexLayout()
{
    HRESULT hr = S_OK;
    D3D11_INPUT_ELEMENT_DESC layout[2];
    ZeroMemory(layout, sizeof(D3D11_INPUT_ELEMENT_DESC) * 2);
    layout[0].SemanticName = "POSITION";
    layout[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
    layout[0].AlignedByteOffset = 0;
    layout[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
    layout[1].SemanticName = "COLOR";
    layout[1].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
    layout[1].AlignedByteOffset = 12;
    layout[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
    hr = g_pd3dDevice->CreateInputLayout(layout, 2,
        m_pVSBlob->GetBufferPointer(),
        m_pVSBlob->GetBufferSize(),
        &m_pVertexLayout);
    if (FAILED(hr)) return hr;

    m_pVSBlob->Release();
    return hr;
}

// 인덱스 버퍼 생성 bindflags와 사이즈를 indexbuffer
HRESULT ModelObj::CreateIndexBuffer()
{
    HRESULT hr = S_OK;
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

HRESULT ModelObj::LoadShader()
{
    HRESULT hr = S_OK;
    ID3DBlob* error = nullptr;
    hr = D3DCompileFromFile(
        L"VertexShader.hlsl",
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
        L"PixelShader.hlsl",
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
bool	ModelObj::CreateVertexData()
{
    if (m_VertexList.size() > 0)
    {
        return true;
    }
    return false;
}
bool	ModelObj::CreateIndexData()
{
    if (m_IndexList.size() > 0)
    {
        return true;
    }
    return false;
}
bool ModelObj::Init()
{
    if (CreateVertexData())
    {
        CreateConstantBuffer();
        CreateVertexBuffer();
        CreateIndexBuffer();
        LoadShader();
        CreateVertexLayout();
    }
    return false;
}

bool ModelObj::Frame()
{
    return true;
}
bool ModelObj::PreRender(ID3D11DeviceContext* pContext)
{
    if (m_VertexList.size() <= 0) return true;

    pContext->UpdateSubresource(
        m_pConstantBuffer, 0, NULL, &m_cbData, 0, 0);
    pContext->VSSetConstantBuffers(
        0, 1, &m_pConstantBuffer);
    pContext->VSSetShader(m_pVS, NULL, 0);
    pContext->PSSetShader(m_pPS, NULL, 0);
    pContext->IASetInputLayout(m_pVertexLayout);
    UINT pStrides = sizeof(SimpleVertex);
    UINT pOffsets = 0;
    pContext->IASetVertexBuffers(0, 1, &m_pVertexBuffer,
        &pStrides, &pOffsets);
    pContext->IASetIndexBuffer(m_pIndexBuffer,
        DXGI_FORMAT_R32_UINT, 0);
    return true;
}
bool ModelObj::Render(ID3D11DeviceContext* pContext)
{
    if (PreRender(pContext) == false) return false;
    if (PostRender(pContext, m_iNumIndex) == false) return false;
    return true;
}
bool ModelObj::PostRender(ID3D11DeviceContext* pContext, UINT iNumIndex)
{
    pContext->IASetPrimitiveTopology(
        D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    pContext->DrawIndexed(iNumIndex, 0, 0);
    return true;
}

bool ModelObj::Release()
{
    m_pVertexBuffer->Release();
    m_pIndexBuffer->Release();
    m_pVertexLayout->Release();
    m_pConstantBuffer->Release();
    m_pVS->Release();
    m_pPS->Release();
    return false;
}
