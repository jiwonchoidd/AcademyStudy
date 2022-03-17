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
	for (int num = 0; num < 6; num+=4)
	{
		PNCT_VERTEX pnct1;
		PNCT_VERTEX pnct2;
		PNCT_VERTEX pnct3;
		PNCT_VERTEX pnct4;
		pnct1.pos = vertices[num];
		pnct1.color = { 1.0f,1.0f,1.0f,1.0f };
		pnct1.tex = uv[num];

		pnct2.pos = vertices[num+1];
		pnct2.color = { 1.0f,1.0f,1.0f,1.0f };
		pnct2.tex = uv[num+1];

		pnct3.pos = vertices[num+2];
		pnct3.color = { 1.0f,1.0f,1.0f,1.0f };
		pnct3.tex = uv[num+2];

		pnct4.pos = vertices[num + 3];
		pnct4.color = { 1.0f,1.0f,1.0f,1.0f };
		pnct4.tex = uv[num + 3];

		CreateTangentSpace(&pnct1, &pnct2, &pnct3);
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

void KBoxObj::CreateTangentSpace(PNCT_VERTEX* v1, PNCT_VERTEX* v2, PNCT_VERTEX* v3)
{
	KVector3 vEdge1 = v2->pos - v1->pos;
	KVector3 vEdge2 = v3->pos - v1->pos;

	KVector2 vEdge1_UV = v2->tex - v1->tex;
	KVector2 vEdge2_UV = v3->tex - v1->tex;


	float fDenominator = vEdge1_UV.x * vEdge2_UV.y - vEdge2_UV.x * vEdge1_UV.y;

	if (fDenominator < 0.0001f && fDenominator>-0.0001f)
	{
		v1->tangent=KVector3(1.0f, 0.0f, 0.0f);
		v1->binormal= KVector3(0.0f, 1.0f, 0.0f);
		v1->normal= KVector3(0.0f, 0.0f, 1.0f);

		v2->tangent = KVector3(1.0f, 0.0f, 0.0f);
		v2->binormal = KVector3(0.0f, 1.0f, 0.0f);
		v2->normal = KVector3(0.0f, 0.0f, 1.0f);
		
		v3->tangent = KVector3(1.0f, 0.0f, 0.0f);
		v3->binormal = KVector3(0.0f, 1.0f, 0.0f);
		v3->normal = KVector3(0.0f, 0.0f, 1.0f);
	}
	else
	{
		//°è»ê
		float fScale = 1.0f / fDenominator;

		KVector3 T;
		KVector3 B;
		KVector3 N;
		T = KVector3((vEdge2_UV.y * vEdge1.x - vEdge1_UV.y * vEdge2.x) * fScale, (vEdge2_UV.y * vEdge1.y - vEdge1_UV.y * vEdge2.y) * fScale,
			(vEdge2_UV.y * vEdge1.z - vEdge1_UV.y * vEdge2.z) * fScale);
		B = KVector3((-vEdge2_UV.x * vEdge1.x + vEdge1_UV.x * vEdge2.x) * fScale, (-vEdge2_UV.x * vEdge1.y + vEdge1_UV.x * vEdge2.y) * fScale,
			(-vEdge2_UV.x * vEdge1.z + vEdge1_UV.x * vEdge2.z) * fScale);
		// The normal N is calculated as the cross product between T and B 
		D3DXVec3Cross( &N, &T, &B );
		// Calculate and cache the reciprocal value
		float fScale2 = 1.0f / ((T.x * B.y * N.z - T.z * B.y * N.x) +
			(B.x * N.y * T.z - B.z * N.y * T.x) + (N.x * T.y * B.z - N.z * T.y * B.x));
		KVector3 vTemp;
		(v1->tangent).x = D3DXVec3Cross(&vTemp, &B, &N)->x * fScale2; 
		(v1->tangent).y = -(D3DXVec3Cross(&vTemp, &N, &T)->x * fScale2);
		(v1->tangent).z = D3DXVec3Cross(&vTemp, &T, &B)->x * fScale2;
		D3DXVec3Normalize(&(v1->tangent), &(v1->tangent));
		(v1->binormal).x = -(D3DXVec3Cross(&vTemp, &B, &N)->y * fScale2); 
		(v1->binormal).y = D3DXVec3Cross(&vTemp, &N, &T)->y * fScale2;
		(v1->binormal).z = -(D3DXVec3Cross(&vTemp, &T, &B)->y * fScale2);
		D3DXVec3Normalize(&(v1->binormal), &(v1->binormal));
		(v1->normal).x = D3DXVec3Cross(&vTemp, &B, &N)->z * fScale2; 
		(v1->normal).y = -(D3DXVec3Cross(&vTemp, &N, &T)->z * fScale2);
		(v1->normal).z = D3DXVec3Cross(&vTemp, &T, &B)->z * fScale2;
		D3DXVec3Normalize(&(v1->normal), &(v1->normal));

		(v2->tangent).x = D3DXVec3Cross(&vTemp, &B, &N)->x * fScale2;
		(v2->tangent).y = -(D3DXVec3Cross(&vTemp, &N, &T)->x * fScale2);
		(v2->tangent).z = D3DXVec3Cross(&vTemp, &T, &B)->x * fScale2;
		D3DXVec3Normalize(&(v2->tangent), &(v2->tangent));
		(v2->binormal).x = -(D3DXVec3Cross(&vTemp, &B, &N)->y * fScale2);
		(v2->binormal).y = D3DXVec3Cross(&vTemp, &N, &T)->y * fScale2;
		(v2->binormal).z = -(D3DXVec3Cross(&vTemp, &T, &B)->y * fScale2);
		D3DXVec3Normalize(&(v2->binormal), &(v2->binormal));
		(v2->normal).x = D3DXVec3Cross(&vTemp, &B, &N)->z * fScale2;
		(v2->normal).y = -(D3DXVec3Cross(&vTemp, &N, &T)->z * fScale2);
		(v2->normal).z = D3DXVec3Cross(&vTemp, &T, &B)->z * fScale2;
		D3DXVec3Normalize(&(v2->normal), &(v2->normal));

		(v3->tangent).x = D3DXVec3Cross(&vTemp, &B, &N)->x * fScale2;
		(v3->tangent).y = -(D3DXVec3Cross(&vTemp, &N, &T)->x * fScale2);
		(v3->tangent).z = D3DXVec3Cross(&vTemp, &T, &B)->x * fScale2;
		D3DXVec3Normalize(&(v3->tangent), &(v3->tangent));
		(v3->binormal).x = -(D3DXVec3Cross(&vTemp, &B, &N)->y * fScale2);
		(v3->binormal).y = D3DXVec3Cross(&vTemp, &N, &T)->y * fScale2;
		(v3->binormal).z = -(D3DXVec3Cross(&vTemp, &T, &B)->y * fScale2);
		D3DXVec3Normalize(&(v3->binormal), &(v3->binormal));
		(v3->normal).x = D3DXVec3Cross(&vTemp, &B, &N)->z * fScale2;
		(v3->normal).y = -(D3DXVec3Cross(&vTemp, &N, &T)->z * fScale2);
		(v3->normal).z = D3DXVec3Cross(&vTemp, &T, &B)->z * fScale2;
		D3DXVec3Normalize(&(v3->normal), &(v3->normal));
	}

}

bool KBoxObj::Render(ID3D11DeviceContext* pContext)
{
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
