Texture2D		g_txDiffuse;
SamplerState	g_sampler;
struct PS_INPUT
{
	float4 p : SV_POSITION;
	float4 c : COLOR0;
	float2 t : TEXCOORD0;
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
	float4 p : SV_POSITION; // ���� ��ġ �������� ���
	float4 c : COLOR0;      // ���� �÷� �������� ���
	float2 t : TEXCOORD0;   // ���� �ؽ��� ��ǥ �������� ���
	float  clip : SV_ClipDistance0; // Ŭ���÷� ����
};
VS_OUTPUT VS(VS_INPUT input)
{
	VS_OUTPUT output = (VS_OUTPUT)0;
	output.p = float4(input.p,1.0f);
	output.t = input.t;
	output.c = input.c;
	return output;
}
PS_OUTPUT PS(PS_INPUT input)
{
	PS_OUTPUT output = (PS_OUTPUT)0;
	float4 fResult = g_txDiffuse.Sample(g_sampler, input.t);
	output.c = fResult;
	return output;
}