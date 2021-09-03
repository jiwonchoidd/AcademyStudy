Texture2D		g_txDiffuse;
SamplerState	g_sampler;
cbuffer cb0 : register(b0)
{
	matrix g_matWorld : packoffset(c0);
	matrix g_matView : packoffset(c4);
	matrix g_matProj : packoffset(c8);
};
struct PS_INPUT
{
	float4 p : SV_POSITION;
	float4 c : COLOR0;
	float2 t : TEXCOORD0;
};
struct PS_OUTPUT
{
	float4 c : SV_TARGET;
};
struct VS_INPUT
{
	float3 p : POSITION;
	float4 c : COLOR0;
	float2 t : TEXCOORD0;
};
struct VS_OUTPUT
{
	float4 p : SV_POSITION; // 정점 위치 레지스터 사용
	float4 c : COLOR0;      // 정점 컬러 레지스터 사용
	float2 t : TEXCOORD0;   // 정점 텍스쳐 좌표 레지스터 사용
	float  clip  : SV_ClipDistance0; // 클립플랜 지정
};
VS_OUTPUT VS(VS_INPUT input)
{
	VS_OUTPUT output = (VS_OUTPUT)0;
	float4 pos = float4(input.p, 1.0f);
	float4 world = mul(pos, g_matWorld);
	float4 view = mul(world, g_matView);
	float4 proj = mul(view, g_matProj);
	output.p = proj;
	output.t = input.t;
	output.c = input.c;
	// y = 0,11,0,1   > 0
	// y = 0,9,0,1    < 0
	float4 clipdata = float4(0,-1,0,10.0f);
	output.clip = dot(world, clipdata);
	return output;
}
PS_OUTPUT PS(PS_INPUT input)
{
	PS_OUTPUT output = (PS_OUTPUT)0;
	float4 fResult = g_txDiffuse.Sample(g_sampler, input.t);
	output.c = fResult * input.c;
	return output;
}