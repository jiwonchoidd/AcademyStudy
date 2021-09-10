// Pixel Shader
// ���� ���̴��� ��ȯ���� ���ڰ����� ���޵ȴ�.
// �Ƚ����̴� ��Ȱ : �ݵ�� 1���� �÷�(SV_Target)�� ��ȯ�ؾ� �Ѵ�.
Texture2D		g_txDiffuse;
SamplerState	g_sampler;

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
PS_OUTPUT PS3(PS_INPUT input)
{
	PS_OUTPUT output = (PS_OUTPUT)0;
	float2 tex = input.t;
	tex.x = tex.x;
	tex.y = tex.y;
	float4 fResult = g_txDiffuse.Sample(g_sampler, tex);
	//if (fResult.r < 0.01f && fResult.g < 0.01f && fResult.b < 0.01f)
	//{
	//	discard;
	//}
	output.c = fResult;
	return output;
}