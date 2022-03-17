cbuffer CBuf
{
	matrix g_matWorld : packoffset(c0);
	matrix g_matView : packoffset(c4);
	matrix g_matProj : packoffset(c8);
	matrix g_matNormal : packoffset(c12);
	float4 g_lightPos : packoffset(c16); //라이트 방향
	float4 g_lightColor : packoffset(c17); //라이트 색상
	float4 g_camPos : packoffset(c18); //카메라 방향
	float4 g_value : packoffset(c19); //기타 시간 값등
};
struct VS_INPUT 
{
   float3 mPosition : POSITION;
   float3 mTangent	: TANGENT;
   float3 mBinormal : BINORMAL;
   float3 mNormal    : NORMAL;
   float4 mColor	: COLOR;
   float2 mUV		: TEXTURE;
};
struct VS_OUTPUT
{
	float4 mPosition : SV_POSITION;
	float4 mColor : COLOR0;
	float2 mUV : TEXCOORD0;		//uv
	float3 mLightDir : TEXCOORD1; //방향
	float3 mViewDir : TEXCOORD2; //방향
	float3 mT        : TEXCOORD3;
	float3 mB        : TEXCOORD4;
	float3 mN        : TEXCOORD5;
};

VS_OUTPUT VS(VS_INPUT Input) 
{
	//난반사광의 계산 동일한 계산을 PS VS 둘다 할 수 있음. 
   //하지만 픽셀단위 계산보다 정점단위 계산이 더 연산량이 적음
	VS_OUTPUT Output = (VS_OUTPUT)0;
	Output.mPosition = float4(Input.mPosition, 1.0f);
	Output.mPosition = mul(Output.mPosition, g_matWorld);

	//라이트 방향 월드 행렬 곱함, 월드 공간에서의 위치여서 여기서 광원의 위치를 뺀다.
	float3 lightDir = Output.mPosition.xyz - g_lightPos.xyz;
	Output.mLightDir= normalize(lightDir);
	//보는 방향
	float3 viewDir = Output.mPosition.xyz - g_camPos.xyz;
	Output.mViewDir = normalize(viewDir);
	//

	Output.mPosition = mul(Output.mPosition, g_matView);
	Output.mPosition = mul(Output.mPosition, g_matProj);

	float3 worldTangent = mul(Input.mTangent, (float3x3)g_matWorld);
	Output.mT = normalize(worldTangent);
	float3 worldBinormal = mul(Input.mBinormal, (float3x3)g_matWorld);
	Output.mB = normalize(worldBinormal);
	float3 worldNormal = mul(Input.mNormal, (float3x3)g_matWorld);
	Output.mN = normalize(worldNormal);


	Output.mColor = Input.mColor;
	Output.mUV = Input.mUV;
	return  Output;
}
