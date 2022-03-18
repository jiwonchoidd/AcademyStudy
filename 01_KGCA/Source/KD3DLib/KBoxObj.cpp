#include "KBoxObj.h"

bool KBoxObj::Init(std::wstring vsfile, std::wstring psfile, std::wstring texturefile,
	std::wstring specularfile, std::wstring normalfile)
{
	const KVector3 vertices[] =
	{
		{-1.0f,-1.0f,-1.0f},
		{ 1.0f,-1.0f,-1.0f},
		{-1.0f,1.0f,-1.0f},
		{ 1.0f,1.0f,-1.0f},//Á¤¸é3
		{1.0f,-1.0f,1.0f},
		{ -1.0f,-1.0f,1.0f},
		{1.0f,1.0f,1.0f},
		{-1.0f,1.0f,1.0f},//µÞ¸é7
		{1.0f,-1.0f,-1.0f},
		{ 1.0f,-1.0f,1.0f},
		{1.0f,1.0f,-1.0f},
		{ 1.0f,1.0f,1.0f},//¿À¸¥ÂÊ¸é11
		{-1.0f,-1.0f,1.0f},
		{ -1.0f,-1.0f,-1.0f},
		{-1.0f,1.0f,1.0f},
		{ -1.0f,1.0f,-1.0f},//¿ÞÂÊ¸é15
		{-1.0f,1.0f,-1.0f},
		{ 1.0f,1.0f,-1.0f},
		{-1.0f,1.0f,1.0f},
		{ 1.0f,1.0f,1.0f},//À­ÂÊ¸é19
		{-1.0f,-1.0f,1.0f},
		{ 1.0f,-1.0f,1.0f},
		{-1.0f,-1.0f,-1.0f},
		{ 1.0f,-1.0f,-1.0f},//¾Æ·§¸é23
	};
	const KVector3 normal[] =
	{
		{0.0f,0.0f,-1.0f},
		{ 0.0f,0.0f,-1.0f},
		{0.0f,0.0f,-1.0f},
		{ 0.0f,0.0f,-1.0f},//Á¤¸é3
		{0.0f,0.0f,1.0f},
		{ 0.0f,0.0f,1.0f},
		{0.0f,0.0f,1.0f},
		{0.0f,0.0f,1.0f},//µÞ¸é7
		{1.0f,0.0f,0.0f},
		{ 1.0f,0.0f,0.0f},
		{1.0f,0.0f,0.0f},
		{ 1.0f,0.0f,0.0f},//¿À¸¥ÂÊ¸é11
		{-1.0f,0.0f,0.0f},
		{ -1.0f,0.0f,0.0f},
		{-1.0f,0.0f,0.0f},
		{ -1.0f,0.0f,0.0f},//¿ÞÂÊ¸é15
		{0.0f,1.0f,0.0f},
		{ 0.0f,1.0f,0.0f},
		{0.0f,1.0f,0.0f},
		{ 0.0f,1.0f,0.0f},//À­ÂÊ¸é19
		{0.0f,-1.0f,0.0f},
		{ 0.0f,-1.0f,0.0f},
		{0.0f,-1.0f,0.0f},
		{ 0.0f,-1.0f,0.0f},//¾Æ·§¸é23
	};
	const KVector2 uv[] =
	{
		{0.0f,1.0f},
		{1.0f,1.0f},
		{0.0f,0.0f},
		{1.0f,0.0f},//Á¤¸é
		{0.0f,1.0f},
		{1.0f,1.0f},
		{0.0f,0.0f},
		{1.0f,0.0f},//µÞ¸é
		{0.0f,1.0f},
		{1.0f,1.0f},
		{0.0f,0.0f},
		{1.0f,0.0f},//¿À¸¥¸é
		{0.0f,1.0f},
		{1.0f,1.0f},
		{0.0f,0.0f},
		{1.0f,0.0f},//¿Þ¸é
		{0.0f,1.0f},
		{1.0f,1.0f},
		{0.0f,0.0f},
		{1.0f,0.0f},//À­¸é
		{0.0f,1.0f},
		{1.0f,1.0f},
		{0.0f,0.0f},
		{1.0f,0.0f},//¾Æ·§¸é
	};
	const unsigned short indices[] =
	{
		2,1,0,	 2,3,1,//Á¤¸é
		6,5,4,	 6,7,5,//µÞ¸é
		10,9,8,  10,11,9,//¿À¸¥ÂÊ¸é
		14,13,12, 14,15,13,//¿ÞÂÊ¸é
		18,17,16, 18,19,17, //À­ÂÊ¸é
		22,21,20, 22,23,21,//¾Æ·§¸é
		//0,2,1, 2,3,1,
		//1,3,5, 3,7,5,
		//2,6,3, 3,6,7,
		//4,5,7, 4,7,6,
		//0,4,2, 2,4,6,
		//0,1,4, 1,5,4,
	};
	for (int num = 0; num < sizeof(vertices)/sizeof(KVector3); num+=4)
	{
		PNCT_VERTEX pnct1;
		PNCT_VERTEX pnct2;
		PNCT_VERTEX pnct3;
		PNCT_VERTEX pnct4;
		pnct1.pos = vertices[num];
		pnct1.tex = uv[num];
		pnct1.color = { 1.0f,1.0f,1.0f,1.0f };

		pnct2.pos = vertices[num+1];
		pnct2.tex = uv[num+1];
		pnct2.color = { 1.0f,1.0f,1.0f,1.0f };

		pnct3.pos = vertices[num+2];
		pnct3.tex = uv[num+2];
		pnct3.color = { 1.0f,1.0f,1.0f,1.0f };

		pnct4.pos = vertices[num + 3];
		pnct4.tex = uv[num + 3];
		pnct4.color = { 1.0f,1.0f,1.0f,1.0f };
		KVector3 t, b, n;
		CreateTangentSpace(&pnct1.pos, &pnct2.pos, &pnct3.pos, 
			&pnct1.tex, &pnct2.tex, &pnct3.tex,&n, &t, &b);
		pnct1.tangent = t;
		pnct1.binormal = b;
		pnct1.normal = n;
		CreateTangentSpace(&pnct2.pos, &pnct3.pos, &pnct1.pos,
			&pnct2.tex, &pnct3.tex, &pnct1.tex, &n, &t, &b);
		pnct2.tangent = t;
		pnct2.binormal = b;
		pnct2.normal = n;
		CreateTangentSpace(&pnct3.pos, &pnct1.pos, &pnct2.pos,
			&pnct3.tex, &pnct1.tex, &pnct2.tex, &n, &t, &b);
		pnct3.tangent = t;
		pnct3.binormal = b;
		pnct3.normal = n;
		CreateTangentSpace(&pnct4.pos, &pnct3.pos, &pnct2.pos,
			&pnct4.tex, &pnct3.tex, &pnct2.tex, &n, &t, &b);
		pnct4.tangent = t;
		pnct4.binormal = b;
		pnct4.normal = n;
		m_VertexList.push_back(pnct1);
		m_VertexList.push_back(pnct2);
		m_VertexList.push_back(pnct3);
		m_VertexList.push_back(pnct4);
	}

	for (int num = 0; num < sizeof(indices)/ sizeof(short); num++)
	{ 
		m_IndexList.push_back(indices[num]);
	}

	if (CreateObject(vsfile,psfile,texturefile, specularfile, normalfile))
	{
		return true;
	}

	return false;
}

bool KBoxObj::Frame()
{
	return true;
}

void KBoxObj::CreateTangentSpace(KVector3* v1, KVector3* v2, KVector3* v3, KVector2* uv1, 
	KVector2* uv2, KVector2* uv3, KVector3* normal, KVector3* tangent, KVector3* binormal)
{
	KVector3 vEdge1 = *v2 - *v1;
	KVector3 vEdge2 = *v3 - *v1;

	float vEdge1_U = uv2->x - uv1->x;
	float vEdge1_V = uv2->y - uv1->y;
	float vEdge2_U = uv3->x- uv1->x;
	float vEdge2_V = uv3->y - uv1->y;


	float fDenominator = vEdge1_U * vEdge2_V - vEdge2_U * vEdge1_V;

	if (fDenominator < 0.0001f && fDenominator>-0.0001f)
	{
		*tangent =KVector3(1.0f, 0.0f, 0.0f);
		*binormal= KVector3(0.0f, 1.0f, 0.0f);
		*normal= KVector3(0.0f, 0.0f, 1.0f);
	}
	else
	{
		//°è»ê
		float fScale = 1.0f / fDenominator;

		KVector3 T;
		KVector3 B;
		KVector3 N;
		T = KVector3((vEdge2_V * vEdge1.x - vEdge1_V * vEdge2.x) * fScale,
			(vEdge2_V * vEdge1.y - vEdge1_V * vEdge2.y) * fScale,
			(vEdge2_V * vEdge1.z - vEdge1_V * vEdge2.z) * fScale);
		B = KVector3((-vEdge2_U * vEdge1.x + vEdge1_U * vEdge2.x) * fScale,
			(-vEdge2_U * vEdge1.y + vEdge1_U * vEdge2.y) * fScale,
			(-vEdge2_U * vEdge1.z + vEdge1_U * vEdge2.z) * fScale);
		// The normal N is calculated as the cross product between T and B 
		D3DXVec3Cross( &N, &T, &B );
		// Calculate and cache the reciprocal value
		float fScale2 = 1.0f / ((T.x * B.y * N.z - T.z * B.y * N.x) +
								(B.x * N.y * T.z - B.z * N.y * T.x) + 
								(N.x * T.y * B.z - N.z * T.y * B.x));
		KVector3 vTemp = {0,0,0};
		(*tangent).x = D3DXVec3Cross(&vTemp, &B, &N)->x * fScale2; 
		(*tangent).y = -(D3DXVec3Cross(&vTemp, &N, &T)->x * fScale2);
		(*tangent).z = D3DXVec3Cross(&vTemp, &T, &B)->x * fScale2;
		D3DXVec3Normalize(&(*tangent), &(*tangent));
		(*binormal).x = -(D3DXVec3Cross(&vTemp, &B, &N)->y * fScale2); 
		(*binormal).y = D3DXVec3Cross(&vTemp, &N, &T)->y * fScale2;
		(*binormal).z = -(D3DXVec3Cross(&vTemp, &T, &B)->y * fScale2);
		D3DXVec3Normalize(&(*binormal), &(*binormal));
		(*normal).x = D3DXVec3Cross(&vTemp, &B, &N)->z * fScale2; 
		(*normal).y = -(D3DXVec3Cross(&vTemp, &N, &T)->z * fScale2);
		(*normal).z = D3DXVec3Cross(&vTemp, &T, &B)->z * fScale2;
		D3DXVec3Normalize(&(*normal), &(*normal));
	}

}

bool KBoxObj::Render(ID3D11DeviceContext* pContext)
{
	pContext->IASetInputLayout(m_pVertexLayout.Get());
	UINT pStrides = m_iVertexSize;
	UINT pOffsets = 0;
	pContext->IASetVertexBuffers(1, 1, m_pVertexBuffer.GetAddressOf(),
		&pStrides, &pOffsets);
	KObject::Render(pContext);
	return true;
}

bool KBoxObj::Release()
{
	return true;
}

KBoxObj::KBoxObj()
{
}

KBoxObj::~KBoxObj()
{
}
