#define _CRT_SECURE_NO_WARNINGS
#include "Sample.h"
//Winmain ��ũ��
WinMain_OPT(���ؽ� ����,800,600);

	//���� ���� ����
HRESULT Sample::CreateVertexBuffer()
{
	HRESULT hr = S_OK;
	D3D11_BUFFER_DESC bufferDesc;
    ZeroMemory(&bufferDesc, sizeof(D3D11_BUFFER_DESC));
    bufferDesc.ByteWidth = sizeof(NormalVertex) * m_VertexList.size();
    bufferDesc.Usage = D3D11_USAGE_DEFAULT;
    bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    D3D11_SUBRESOURCE_DATA data;
    ZeroMemory(&data, sizeof(D3D11_SUBRESOURCE_DATA));
    data.pSysMem = &m_VertexList.at(0);
    hr = m_pd3dDevice->CreateBuffer(&bufferDesc, &data, &m_pVertexBuffer);
    if (FAILED(hr)) return hr;
    return hr;
}

HRESULT Sample::CreateInputLayout()
{
    HRESULT hr = S_OK;
    D3D11_INPUT_ELEMENT_DESC layout[] =
    {
        { "POSITION",  0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,  D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "COLOR",  0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12,  D3D11_INPUT_PER_VERTEX_DATA, 0 }
    };
    UINT numLayout = sizeof(layout) / sizeof(layout[0]);
    hr = m_pd3dDevice->CreateInputLayout(layout, numLayout,
        m_pBlob->GetBufferPointer(),
        m_pBlob->GetBufferSize(),
        &m_pInputLayout);
    if (FAILED(hr)) return hr;

    m_pBlob->Release();
    return hr;
}
//������ 
HRESULT Sample::LoadShader()
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
        &m_pBlob,
        &error);
    //���̴��� �����ڵ� ������
    if (FAILED(hr))
    {
        MessageBoxA(NULL,(char*)error->GetBufferPointer(),"error", MB_OK);
        return hr;
    }
    //������ ����̽�
    //���ؽ����̴� ���� (�����ּ�, ������,pClassLinkage, ���ؽ����̴� ���������� )
    hr =m_pd3dDevice->CreateVertexShader(
        m_pBlob->GetBufferPointer(),
        m_pBlob->GetBufferSize(),
        NULL, &m_pVertexShader);
    if (FAILED(hr)) return hr;
    //�ȼ� ���̴��� �ӽ� ����
    ID3DBlob* psTempBlob = nullptr;
    hr = D3DCompileFromFile(
        L"PixelShader.hlsl",
        nullptr,
        nullptr,
        "PS",
        "ps_5_0",
        0,
        0,
        &psTempBlob,
        &error);
    if (FAILED(hr))
    {
        MessageBoxA(NULL,
            (char*)error->GetBufferPointer(),
            "error", MB_OK);
        return hr;
    }
    hr = m_pd3dDevice->CreatePixelShader(
        psTempBlob->GetBufferPointer(),
        psTempBlob->GetBufferSize(),
        NULL, &m_pPixelShader);
    if (FAILED(hr)) return hr;
    //����
    psTempBlob->Release();
    return hr;
}

bool Sample::LoadFile(std::wstring filename)
{
    FILE* file = nullptr;
    //FILE_Stream,_FileName,_Mode ���� ���� ��
    _tfopen_s(&file, filename.c_str(), _T("rt"));
    if (file == NULL)
    {
        return false;
    }
    //���� �ʱ�ȭ
    TCHAR buffer[256] = { 0, };
    //_Buffer,_BufferCount,����
    _fgetts(buffer, 256, file);
    TCHAR tmp[256] = { 0, };
    int iVersion = 0;
    //���� ��ĵ �Լ�, ��Ʈ���� ��
    _stscanf_s(buffer, _T("%s% d"), tmp, _countof(tmp), &iVersion);

    int iVertexIndex = 0;
    _fgetts(buffer, 256, file);
    _stscanf_s(buffer, _T("%d"), &iVertexIndex);

    int index = 0;
    for (int iLine = 0; iLine < iVertexIndex; iLine++)
    {
        NormalVertex nv;
        _fgetts(buffer, 256, file);
        _stscanf_s(buffer, _T("%d %f %f %f %f %f %f %f"),
            &index,
            &nv.pos.x, &nv.pos.y, &nv.pos.z,
            &nv.color.x, &nv.color.y, &nv.color.z, &nv.color.w);
        m_VertexList.push_back(nv);
    }
    fclose(file);
    return true;
}

bool	Sample::Init()
{
    LoadFile(L"ObjectData.txt");
    CreateVertexBuffer();
    LoadShader();
    CreateInputLayout();
    return false;
}
bool	Sample::Frame()
{
    return true;
}
bool	Sample::Render()
{
    m_pImmediateContext->VSSetShader(m_pVertexShader, NULL, 0);
    m_pImmediateContext->PSSetShader(m_pPixelShader, NULL, 0);
    m_pImmediateContext->IASetInputLayout(m_pInputLayout);
    UINT pStrides = sizeof(NormalVertex);
    UINT pOffsets = 0;
    m_pImmediateContext->IASetVertexBuffers(0, 1, &m_pVertexBuffer,
        &pStrides, &pOffsets);
    m_pImmediateContext->IASetPrimitiveTopology(
        D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    m_pImmediateContext->Draw(m_VertexList.size(), 0);
    return true;
}
bool	Sample::Release()
{
    m_pVertexBuffer->Release();
    m_pInputLayout->Release();
    m_pPixelShader->Release();
    m_pVertexShader->Release();
    return true;
}

Sample::Sample()
{
    m_pVertexBuffer = nullptr;
    m_pInputLayout= nullptr;
    m_pVertexShader = nullptr;
    m_pPixelShader = nullptr;
}
