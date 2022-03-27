cbuffer CBuf
{
	matrix g_matWorld : packoffset(c0);
	matrix g_matView : packoffset(c4);
	matrix g_matProj : packoffset(c8);
	matrix g_matNormal : packoffset(c12);
	float4 g_lightPos : packoffset(c16);	//라이트 방향
	float4 g_lightColor : packoffset(c17);  //라이트 색상
	float4 g_camPos : packoffset(c18);		//카메라 방향
	float4 g_value : packoffset(c19);		//기타 시간 값등
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
	float3 n : NORMAL;
	float4 c : COLOR0; //방향
	float2 t : TEXCOORD0;
	float3 r  : TEXCOORD1;
};
VS_OUTPUT VS(VS_INPUT Input)
{
	VS_OUTPUT pOut = (VS_OUTPUT)0;
	float4 vLocal = float4(Input.p.xyz, 1.0f);// float4(v.p.x, v.p.y, v.p.z, 1.0f);
	float4 vWorld = mul(vLocal, g_matWorld);
	float4 vView = mul(vWorld, g_matView);
	float4 vProj = mul(vView, g_matProj);
	pOut.p = vProj;
	pOut.c = Input.c;
	pOut.r = normalize(vLocal.xyz);
	return pOut;
}
float4 PS(VS_OUTPUT input) : SV_TARGET
{
	float4 vColor = input.c;
	vColor.a = 0.5f;
	return vColor;
}