//구조체를 이용해서 위치와 색을 보내줌
//버퍼 구조체 레지스터에 저장
cbuffer bData
{
	matrix g_mtWorld : packoffset(c0);
	matrix g_mtView : packoffset(c4);
	matrix g_mtProj : packoffset(c8);
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
	float4 vWorld = mul(vLocal, g_mtWorld);
	float4 vView = mul(vWorld, g_mtView);
	float4 vProj = mul(vView, g_mtProj);

	output.p = vProj;
	output.c = c;
	return output;
}
