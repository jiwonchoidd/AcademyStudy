#pragma once
#include "TMap.h"
class THeightMap : public TMap
{
public:
	std::vector<float>			m_fVertexList;
	D3D11_TEXTURE2D_DESC		m_Desc;
public:
	bool CreateHeightMap(ID3D11Device* pd3dDevice, 
		ID3D11DeviceContext*	pImmediateContext, T_STR name);
	float		GetHeightOfVertex(UINT iIndex);
public:
	THeightMap();
	virtual ~THeightMap();
};

