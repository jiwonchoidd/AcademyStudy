
cbuffer cbData: register(b0)
{
	matrix g_matWorld	: packoffset(c0);
	matrix g_matView	: packoffset(c4);
	matrix g_matProj	: packoffset(c8);
	matrix g_matNormal	: packoffset(c12);
	float4 g_vLightDir : packoffset(c16);
	float  g_fTimer : packoffset(c17.z);
};

cbuffer cbDataShadow: register(b2)
{
	matrix g_matShadow	: packoffset(c0);
};
struct VS_OUT
{
	float4 p : SV_POSITION;
	float3 n : NORMAL;
	float4 c : COLOR0;
	float2 t : TEXCOORD0;
	float4 s : TEXCOORD1;
};
//NULL Sampler maps to default Sampler state
VS_OUT VS(float3 p: POSITION,
	float3 n : NORMAL,
	float4 c : COLOR,
	float2 t : TEXTURE)
{
	VS_OUT output = (VS_OUT)0;
	float4 vLocal = float4(p, 1.0f);
	float4 vWorld = mul(vLocal, g_matWorld);
	float4 vView = mul(vWorld, g_matView);
	float4 vProj = mul(vView, g_matProj);

	output.s = mul(vWorld, g_matShadow);
	output.p = vProj;
	matrix matNormal = transpose(g_matNormal);
	output.n = normalize(mul(n, (float3x3)matNormal));
	float depth1 = vProj.z * 1.0f / (500.0f - 1.0f) + -1.0f / (500.0f - 1.0f);
	output.c = float4(depth1, depth1, depth1, 1);
	output.t = t;
	return output;
}

Texture2D g_txDiffuse : register(t0);
Texture2D g_txShadow  : register(t1);
SamplerState g_Sampler : register(s0);
SamplerState g_SamplerClamp : register(s1);

float4 PS(VS_OUT v) : SV_TARGET
{
	float3 vLight = float3(g_vLightDir.x, g_vLightDir.y, g_vLightDir.z);
	float fDot = max(0, dot(v.n, -vLight));

	float3 vShadowProj;
	vShadowProj.xy = v.s.xy / v.s.w;
	float shadow = g_txShadow.Sample(g_SamplerClamp, vShadowProj.xy);
	float4 color = g_txDiffuse.Sample(g_Sampler, v.t);
	float depth = v.s.z * 1.0f / (500.0f - 1.0f) + -1.0f / (500.0f - 1.0f);
	if (shadow + 0.01f <= depth)
	{
		color = color * float4(0.5f,0.5f,0.5f,1);
	}
	return color * fDot;
}
float4 PSDepth(VS_OUT v) : SV_TARGET
{
	return v.c;
}