
struct VS_OUTPUT
{
	float4 mPosition : SV_POSITION;
	float3 mNormal : NORMAL;
	float4 mColor : COLOR0;
	float2 mUV : TEXCOORD0;			//uv
	float3 mLightDir : TEXCOORD1;	//빛 방향
	float3 mViewDir : TEXCOORD2;	//카메라 방향
	float3 mT        : TEXCOORD3;
	float3 mB        : TEXCOORD4;
	float3 mN        : TEXCOORD5;
};
Texture2D		g_txDiffuse : register(t0);
Texture2D		g_txSpecular : register(t1);
Texture2D		g_txNormal : register(t2);
SamplerState	g_Sample : register(s0);
float4 PS(VS_OUTPUT Input) : SV_TARGET
{
	//텍스쳐에서 노말, 법선 좌표 구해옴
	float3 tangentNormal = g_txNormal.Sample(g_Sample, Input.mUV).xyz;
    tangentNormal = normalize(tangentNormal * 2 - 1);

   float3x3 TBN = float3x3(normalize(Input.mT), normalize(Input.mB),normalize(Input.mN));
   TBN = transpose(TBN);
   float3 worldNormal = mul(TBN, tangentNormal);
   //디튜즈 텍스쳐
   float4 albedo = g_txDiffuse.Sample(g_Sample, Input.mUV);
   float3 lightDir = normalize(Input.mLightDir);
   float3 diffuse = saturate(dot(worldNormal, -lightDir));
   diffuse =  albedo.rgb * diffuse;

   float3 specular = 0;

   if (diffuse.x > 0.0f)
   {
	  float3 reflection = reflect(lightDir, worldNormal);
	  float3 viewDir = normalize(Input.mViewDir);

	  specular = saturate(dot(reflection, -viewDir));
	  specular = pow(specular,20.0f);

	  //스페큘러 텍스쳐
	  float4 specularInten = g_txSpecular.Sample(g_Sample, Input.mUV);
	  specular *= specularInten.rgb;
   }

   float3 ambient = float3(0.1f, 0.1f, 0.1f) * albedo;

   return float4(ambient + diffuse + specular,1);
}