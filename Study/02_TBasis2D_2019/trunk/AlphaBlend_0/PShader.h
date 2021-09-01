// Pixel Shader
// 이전 쉐이더의 반환값이 인자값으로 전달된다.
// 픽쉘세이더 역활 : 반드시 1개의 컬러(SV_Target)를 반환해야 한다.
static const int   g_iMaxArray =10;
Texture2D		g_txDiffuse : register(t0);
//Texture2DArray	g_txBlendA : register(t10);
Texture2D		g_txBlendB[g_iMaxArray] : register(t1);

SamplerState	g_samplerA : register(s0);
//SamplerState	g_samplerB : register(s1);
//SamplerState	g_samplerC : register(s10);

cbuffer cb1
{
	//float4 Color : packoffset(c0);	// c0
	float   index : packoffset(c0.x);    // c1
};
struct PS_INPUT
{
	float4 p : SV_POSITION;
	float4 c : COLOR0;
	float2 t : TEXCOORD;
};
struct PS_OUTPUT
{
	float4 c : SV_TARGET;
};
float4 PS(in float4 p : SV_POSITION) : SV_TARGET
{
	return float4(1,0,1,1);
}
float4 PS2(PS_INPUT input) : SV_TARGET
{
	return float4(1,0,1,1);
}
float4 GetPixel(int iTexture, float2 tex)
{
	float4 cColor = float4(1,1,1,1);
	switch (iTexture)
	{
	case 0: cColor =  g_txBlendB[0].Sample(g_samplerA, tex); break;
	case 1: cColor = g_txBlendB[1].Sample(g_samplerA, tex); break;
	case 2: cColor = g_txBlendB[2].Sample(g_samplerA, tex); break;
	case 3: cColor = g_txBlendB[3].Sample(g_samplerA, tex); break;
	case 4: cColor = g_txBlendB[4].Sample(g_samplerA, tex); break;
	case 5: cColor = g_txBlendB[5].Sample(g_samplerA, tex); break;
	case 6: cColor = g_txBlendB[6].Sample(g_samplerA, tex); break;
	case 7: cColor = g_txBlendB[7].Sample(g_samplerA, tex); break;
	case 8: cColor = g_txBlendB[8].Sample(g_samplerA, tex); break;
	case 9: cColor = g_txBlendB[9].Sample(g_samplerA, tex); break;
	}
	return cColor;
}
PS_OUTPUT PS3(PS_INPUT input)
{
	PS_OUTPUT output = (PS_OUTPUT)0;
	float4 cDest = g_txDiffuse.Sample(g_samplerA, input.t);
	float4 cSrc = GetPixel(index, input.t);
	if (cSrc.r <= 0.1f && cSrc.g <= 0.1f && cSrc.b <= 0.1f )
	{
		//discard;
		cSrc.a = 0.0f;
	}
	//float4 cBlend = lerp(cDest, cSrc, input.c.a);
	output.c = cDest + cSrc;// +input.c;
	return output;
}