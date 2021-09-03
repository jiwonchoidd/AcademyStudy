Texture2D		g_txDiffuse : register(t0);
Texture2D		g_txRefract : register(t1);
Texture2D		g_txHegiht : register(t2);
Texture2D		g_txReflect : register(t3);
Texture2D		g_txNormal : register(t4);

SamplerState	g_sampler;
cbuffer cb0 : register(b0)
{
	matrix g_matWorld : packoffset(c0);
	matrix g_matView : packoffset(c4);
	matrix g_matProj : packoffset(c8);
};
cbuffer cb1
{
	float4 g_ClipPlane : packoffset(c0);	// c0
	float  time : packoffset(c1.x);    // c1
};
struct PS_INPUT
{
	float4 p : SV_POSITION;
	float4 c : COLOR0;
	float2 t : TEXCOORD0;
	float2 t0 : TEXCOORD1;
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
	float2 t0 : TEXCOORD1;
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
	/*float2 tex;
	tex.x = 0.5f * output.p.x / output.p.w + 0.5f;
	tex.y = -0.5f * output.p.y / output.p.w + 0.5f;
	output.t = tex;*/
	output.c = input.c;
	// y = 0,11,0,1   > 0
	// y = 0,9,0,1    < 0
	float4 clipdata = float4(0,1,0, -g_ClipPlane.w);
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
VS_OUTPUT VS_Reflect(VS_INPUT input)
{
	VS_OUTPUT output = (VS_OUTPUT)0;
	float4 pos = float4(input.p, 1.0f);
	float4 world = mul(pos, g_matWorld);
	float4 view = mul(world, g_matView);
	float4 proj = mul(view, g_matProj);
	output.p = proj;
	output.t = input.t;
	/*float2 tex;
	tex.x = 0.5f * output.p.x / output.p.w + 0.5f;
	tex.y = -0.5f * output.p.y / output.p.w + 0.5f;
	output.t = tex;*/
	output.c = input.c;
	// y = 0,11,0,1   > 0
	// y = 0,9,0,1    < 0
	//float4 clipdata = float4(0, 1, 0, -10);
	output.clip = dot(world, g_ClipPlane);
	return output;
}
PS_OUTPUT PS_Reflect(PS_INPUT input)
{
	PS_OUTPUT output = (PS_OUTPUT)0;
	float4 fResult = g_txDiffuse.Sample(g_sampler, input.t);
	output.c = fResult * input.c;
	return output;
}
VS_OUTPUT VS_Refract(VS_INPUT input)
{
	VS_OUTPUT output = (VS_OUTPUT)0;
	float4 pos = float4(input.p, 1.0f);
	float4 world = mul(pos, g_matWorld);
	float4 view = mul(world, g_matView);
	float4 proj = mul(view, g_matProj);
	output.p = proj;
	output.t = input.t;
	/*float2 tex;
	tex.x = 0.5f * output.p.x / output.p.w + 0.5f;
	tex.y = -0.5f * output.p.y / output.p.w + 0.5f;
	output.t = tex;*/
	output.c = input.c;
	// y = 0,11,0,1   > 0
	// y = 0,9,0,1    < 0
	//float4 clipdata = float4(0, -1, 0, 10);
	output.clip = dot(world, g_ClipPlane);
	return output;
}
PS_OUTPUT PS_Refract(PS_INPUT input)
{
	PS_OUTPUT output = (PS_OUTPUT)0;
	float4 fResult = g_txDiffuse.Sample(g_sampler, input.t);
	output.c = fResult * input.c;
	return output;
}
VS_OUTPUT WaterVS(VS_INPUT input)
{
	VS_OUTPUT output = (VS_OUTPUT)0;
	float4 pos = float4(input.p, 1.0f);
	float4 world = mul(pos, g_matWorld);
	float4 view = mul(world, g_matView);
	float4 proj = mul(view, g_matProj);
	output.p = proj;
	// float4( x,y,z,w) -> x/w,y/w,z/w -> -1 ~ +1, 0 ~1
	output.t = input.t;
	// 투영좌표를 텍스처 좌표로 대체한다.
	float2 tex;
	// x' = x /  w ==> -1 ~ +1
	// y' = y /  w ==> -1 ~ +1
	// x' = x' * 0.5f + 0.5f;
	// y' = y' * 0.5f + 0.5f;
	tex.x = 0.5f * output.p.x / output.p.w + 0.5f;
	tex.y = -0.5f * output.p.y / output.p.w + 0.5f;
	output.t0 = tex;
	output.c = input.c;
	return output;
}
PS_OUTPUT WaterPS(PS_INPUT input)
{
	PS_OUTPUT output = (PS_OUTPUT)0;
	
	float4 cNormal = g_txNormal.Sample(g_sampler, input.t*5);
	

	float2 t0 = input.t0;
	t0.x = t0.x + cNormal.x*0.1f;
	t0.y = t0.y + cNormal.y*0.1f;
	float2 tex = input.t;
	tex.x += time * 0.1f+ cNormal.x*0.1f;
	tex.y += cNormal.y*0.1f;
	float4 cRefract = g_txRefract.Sample(g_sampler, t0);
	float4 cReflect = g_txReflect.Sample(g_sampler, t0);	
	float4 cDefault = g_txDiffuse.Sample(g_sampler, tex);
	
	float fHeight = g_txHegiht.Sample(g_sampler, input.t).r;
	float WaterHeight = ( g_ClipPlane.w - fHeight * 51.0f )
		                   /g_ClipPlane.w;
	
	float4 cFinal = lerp(cDefault, cRefract, WaterHeight);
	cFinal = cFinal * cReflect;
	cFinal = lerp(float4(0.8, 0.8, 0.8, 0.8f), cFinal, WaterHeight);
	output.c = cFinal;
	return output;
}