Texture2D		g_txDiffuse : register(t0);
Texture2D		g_txSpecular : register(t1);
Texture2D		g_txNormal : register(t2);
SamplerState	g_Sample : register(s0);

struct VS_OUTPUT
{
	float4 mPosition : SV_POSITION;
	float3 mNormal : NORMAL;
	float4 mColor : COLOR0;
	float2 mUV : TEXCOORD0;		//uv
	float3 mDiffuse : TEXCOORD1; //디퓨즈
	float3 mViewDir : TEXCOORD2; //방향
	float3 mReflection : TEXCOORD3; //반사
};

float4 PS(VS_OUTPUT Input) : SV_TARGET
{	
	float3 diffuse = saturate(Input.mDiffuse);
	//보간기를 거치는 동안 값이 흐트러질수도 있음 다시 정규화
	float3 reflection = normalize(Input.mReflection);
	float3 viewDir = normalize(Input.mViewDir);
	float3 specular = 0;
	//난사빛(빛이 닿아야)이 있어야 정반사광이 존재함
	if (diffuse.x > 0.0f)
	{
	   specular = saturate(dot(reflection, -viewDir));
	   //20번 거듭제곱
	   specular = pow(specular,20.0f);
	}

	float3 ambient = float3(0.1f, 0.1f, 0.1f);

	return float4(ambient + diffuse + specular,1);
}