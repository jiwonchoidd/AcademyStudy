cbuffer CBuf
{
	matrix g_matWorld : packoffset(c0);
	matrix g_matView : packoffset(c4);
	matrix g_matProj : packoffset(c8);
	matrix g_matNormal : packoffset(c12);
	float4 g_lightPos : packoffset(c16);	//����Ʈ ����
	float4 g_lightColor : packoffset(c17);  //����Ʈ ����
	float4 g_camPos : packoffset(c18);		//ī�޶� ����
	float4 g_value : packoffset(c19);		//��Ÿ �ð� ����
};
struct VS_INPUT
{
	float3 p : POSITION;
	float3 n : NORMAL;
	float4 c	: COLOR;
	float2 t	: TEXCOORD;

	float3 mTangent	: TANGENT;
	float3 mBinormal : BINORMAL;
};
struct VS_OUTPUT
{
	float4 p : SV_POSITION;
	float4 c : COLOR0; //����
	float2 t : TEXCOORD0;
	float3 r  : TEXCOORD1;
};
VS_OUTPUT VS(VS_INPUT Input)
{
	VS_OUTPUT Output = (VS_OUTPUT)0;
	float4 vLocal = float4(Input.p, 1.0f);
	float4 vWorld = mul(vLocal, g_matWorld);
	float4 vView = mul(vWorld, g_matView);
	float4 vProj = mul(vView, g_matProj);
	Output.p = vProj;
	Output.c = Input.c;
	Output.t = Input.t;
	Output.r = normalize(vLocal.xyz);
	return Output;
}

Texture2D		g_txDiffuse : register(t0);
Texture2D		g_txSpecular : register(t1);
Texture2D		g_txNormal : register(t2);
SamplerState	g_Sample : register(s0);
float4 PS(VS_OUTPUT Input) : SV_TARGET
{
	float4 vColor = Input.c;
	vColor.a = 0.5f;
	return vColor;
}