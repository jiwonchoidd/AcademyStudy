Texture2D		g_txDiffuse;
SamplerState	g_sampler;
cbuffer cb0 : register(b0)
{
	matrix g_matWorld : packoffset(c0);
	matrix g_matView : packoffset(c4);
	matrix g_matProj : packoffset(c8);
};
cbuffer cb1
{
	float4 g_vLight : packoffset(c0);	// c0
	float4 g_vLightColor : packoffset(c1);	// c0
	float4 g_vMaterialColor : packoffset(c2);	// c0
	float4 g_vAmbientColor : packoffset(c3);	// c0
	float  time : packoffset(c4.x);    // c1
};
struct PS_INPUT
{
	float4 p : SV_POSITION;
	float3 n : NORMAL;
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
	float3 n : NORMAL;
	float4 c : COLOR0;
	float2 t : TEXCOORD0;
};
struct VS_OUTPUT
{
	float4 p : SV_POSITION; // 정점 위치 레지스터 사용
	float3 n : NORMAL;
	float4 c : COLOR0;      // 정점 컬러 레지스터 사용
	float2 t : TEXCOORD0;   // 정점 텍스쳐 좌표 레지스터 사용
};
VS_OUTPUT VS(VS_INPUT input)
{
	VS_OUTPUT output = (VS_OUTPUT)0;
	float4 pos = float4(input.p, 1.0f);
	float4 world = mul(pos, g_matWorld);
	float4 view = mul(world, g_matView);
	float4 proj = mul(view, g_matProj);
	output.n = input.n;
	output.p = proj;
	output.t = input.t;
	float fDot = max(dot(g_vLight.xyz, input.n), 0.3f);
	output.c = float4(fDot, fDot, fDot, 1.0f) * 
				g_vLightColor * g_vMaterialColor + g_vAmbientColor;
	return output;
}
PS_OUTPUT PS(PS_INPUT input)
{
	PS_OUTPUT output = (PS_OUTPUT)0;
	float4 fResult = g_txDiffuse.Sample(g_sampler, input.t);
	output.c = fResult * input.c;
	return output;
}