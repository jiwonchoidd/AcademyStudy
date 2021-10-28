//구조체를 이용해서 위치와 색을 보내줌
//버퍼 구조체 레지스터에 저장
// 정해진 출력 양식(반드시 정점 위치는 SV_POSITION에 저장해야 한다.)
cbuffer cbData
{
	matrix g_matWorld	: packoffset(c0);
	matrix g_matView	: packoffset(c4);
	matrix g_matProj	: packoffset(c8);
	float  g_fTimer : packoffset(c12.z); // 4비트중에 z축에 저장
};

struct VS_OUT
{
	float4 p : SV_POSITION;
	float4 c : COLOR0;
};
VS_OUT VS(float3 p: POSITION, float4 c : COLOR)
{
	VS_OUT output = (VS_OUT)0;
	float4 vLocal = float4(p, 1.0f);
	float4 vWorld = mul(vLocal, g_matWorld);

	float t = cos(g_fTimer * 0.25f); // -1 ~ +1	
	vWorld.y = cos(t * vWorld.x * 0.1f) + sin(t * vWorld.z * 0.1f);

	float4 vView = mul(vWorld, g_matView);
	float4 vProj = mul(vView, g_matProj);

	output.p = vProj;
	output.c.x = 0.2f;
	output.c.y = 0.2f;
	output.c.z = 0.2f;
	output.c.w = 1.0f;
	return output;
}