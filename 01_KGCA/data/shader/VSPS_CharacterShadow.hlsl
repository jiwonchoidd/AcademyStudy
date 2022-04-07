cbuffer CBuf : register(b0)
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
cbuffer CBuf_Bone: register(b1)
{
	float4x4 g_matBoneWorld[255]; //65535 / 4
};
cbuffer cbDataShadow: register(b2)
{
	matrix g_matShadow	: packoffset(c0);
};
struct VS_INPUT
{
	float3 p		: POSITION;
	float3 n		: NORMAL;
	float4 c		: COLOR;
	float2 t		: TEXCOORD;

	float3 mTangent	: TANGENT;
	float3 mBinormal: BINORMAL;

	float4 i		: INDEX;
	float4 w		: WEIGHT;
};
struct VS_OUTPUT
{
	float4 p : SV_POSITION;
	float2 t : TEXCOORD0;
	float4 c : COLOR0;
	float3 mLightDir : TEXCOORD1; //����
	float3 mViewDir  : TEXCOORD2; //����
	float4 mShadow	 : TEXCOORD3; //���� �� ������ �߰�
	float3 mT        : TEXCOORD4;
	float3 mB        : TEXCOORD5;
	float3 mN        : TEXCOORD6;
};

VS_OUTPUT VS(VS_INPUT Input)
{
	VS_OUTPUT Output = (VS_OUTPUT)0;

	//�ִϸ��̼�
	uint iIndex = Input.c.w;
	float4 vLocal = float4(Input.p, 1.0f);
	float4 vWorld = mul(vLocal, g_matBoneWorld[iIndex]); //�� ��ġ�� ��ȯ
	vWorld = mul(vWorld, g_matWorld); //�� ��ġ���� ���� ��� ��

	//����Ʈ ���� ���� ��� ����, ���� ���������� ��ġ���� ���⼭ ������ ��ġ�� ����.
	float3 lightDir = vWorld.xyz - g_lightPos.xyz;
	Output.mLightDir = normalize(lightDir);
	//���� ����
	float3 viewDir = vWorld.xyz - g_camPos.xyz;
	Output.mViewDir = normalize(viewDir);
	//������ ��İ�
	Output.mShadow = mul(vWorld, g_matShadow);

	float4 vView = mul(vWorld, g_matView);
	float4 vProj = mul(vView, g_matProj);
	float3 worldTangent = mul(Input.mTangent, (float3x3)g_matWorld);
	float3 worldBinormal = mul(Input.mBinormal, (float3x3)g_matWorld);
	float3 worldNormal = mul(Input.n, (float3x3)g_matWorld);
	Output.p = vProj;
	Output.t = Input.t;
	float depth1 = vProj.z * 1.0f / (1000.0f - 1.0f) + -1.0f / (1000.0f - 1.0f);
	Output.c = float4(depth1, depth1, depth1, 1);
	Output.mT = normalize(worldTangent);
	Output.mB = normalize(worldBinormal);
	Output.mN = normalize(worldNormal);

	return  Output;
}
Texture2D		g_txDiffuse : register(t0);
Texture2D		g_txSpecular : register(t1);
Texture2D		g_txNormal : register(t2);
Texture2D		g_txShadow  : register(t3);
SamplerState	g_Sample : register(s0);
SamplerState	 g_SamplerClamp : register(s1);;
float4 PS(VS_OUTPUT Input) : SV_TARGET
{
	//�ؽ��Ŀ��� �븻, ���� ��ǥ ���ؿ�
   float3 tangentNormal = g_txNormal.Sample(g_Sample, Input.t).xyz;
   tangentNormal = normalize(tangentNormal * 2 - 1);

   float3x3 TBN = float3x3(normalize(Input.mT), normalize(Input.mB),normalize(Input.mN));
   TBN = transpose(TBN);
   float3 worldNormal = mul(TBN, tangentNormal);
   float4 albedo = g_txDiffuse.Sample(g_Sample, Input.t); //�˺��� �⺻ ���� �ؽ���
   //������
   float3 vShadowProj;
   vShadowProj.xy = Input.mShadow.xy / Input.mShadow.w;
   float shadow = g_txShadow.Sample(g_SamplerClamp, vShadowProj.xy);
   float depth = Input.mShadow.z * 1.0f / (1000.0f - 1.0f) + -1.0f / (1000.0f - 1.0f);
   if (shadow + 0.005f <= depth)
   {
	   albedo = albedo * float4(0.5f, 0.5f, 0.5f, 1.0f);
   }
   //��ǻ�� �ؽ���
   float3 lightDir = normalize(Input.mLightDir);
   float3 diffuse = saturate(dot(worldNormal, -lightDir)); // ���帮��� ��ǻ��
   diffuse = g_lightColor.rgb * albedo.rgb * diffuse;

   //����ŧ����
   float3 specular = 0;
   if (diffuse.x > 0.0f)
   {
	  float3 reflection = reflect(lightDir, worldNormal);
	  float3 viewDir = normalize(Input.mViewDir);

	  specular = saturate(dot(reflection, -viewDir));
	  specular = pow(specular,20.0f);

	  //����ŧ�� �ؽ���
	  float4 specularInten = g_txSpecular.Sample(g_Sample, Input.t);
	  specular *= specularInten.rgb * g_lightColor;
   }
   float3 ambient = float3(0.05f, 0.05f, 0.05f) * albedo;
   return float4(ambient + diffuse + specular, 1);
}
float4 PSDepth(VS_OUTPUT Input) : SV_TARGET
{
	return Input.c;
}