// 정해진 출력 양식(반드시 정점 위치는 SV_POSITION에 저장해야 한다.)
// 정해진 레지스터에서 정해진 레지스터로 저장한다.
// 레지스터-> x,y,z,w  9.0 -> 65545/4
cbuffer cbData
{
	matrix g_matWorld	: packoffset(c0);
	matrix g_matView	: packoffset(c4);
	matrix g_matProj	: packoffset(c8);
	matrix g_matNormal	: packoffset(c12);
	float  g_fTimer : packoffset(c16.z);
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
	// 라이트에서 바라본 투영 좌표
	float4 vProj = mul(vView, g_matProj);
	// 라이트 뷰 행렬
	float4 vShadowProj = mul(vWorld, g_matNormal);

	output.s = vShadowProj;
	output.p = vProj;
	output.n = n;
	float depth = output.p.z / 500.0f;
	output.c = float4(depth, depth, depth, vShadowProj.z/500.0f); //알파값에 Z
	output.t = t;
	return output;
}

Texture2D g_txDiffuse : register(t0);
Texture2D g_txShadow  : register(t1);
SamplerState g_Sampler : register(s0);
SamplerState g_SamplerClamp : register(s1);
float4 PS(VS_OUT v) : SV_TARGET
{
	float4 shadow = g_txShadow.Sample(g_SamplerClamp,
									v.s.xy / v.s.w);
	float4 color = g_txDiffuse.Sample(g_Sampler, v.t);
	//조건 바이어스를 임의로 적용
	if (shadow.r+0.005f < v.c.a)
	{
		color = color * float4(0.5f,0.5f,0.5f,1);
	}
	return color;
}
float4 PSDepth(VS_OUT v) : SV_TARGET
{
	return v.c;
}