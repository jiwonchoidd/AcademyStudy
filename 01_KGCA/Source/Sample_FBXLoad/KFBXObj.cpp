#include "KFBXObj.h"

bool KFBXObj::Render(ID3D11DeviceContext* context)
{
	context->IASetInputLayout(m_pVertexLayout.Get());
	UINT pStrides = m_iVertexSize;
	UINT pOffsets = 0;
	//노말맵 추가적인 Binormal Tangent
	context->IASetVertexBuffers(1, 1, m_pVertexBuffer.GetAddressOf(),
		&pStrides, &pOffsets);
	KObject::Render(context);
	return true;
}

bool KFBXObj::CreateVertexData()
{
	return true;
}

bool KFBXObj::CreateIndexData()
{
	return true;
}

KFBXObj::KFBXObj()
{
}

KFBXObj::~KFBXObj()
{
}
