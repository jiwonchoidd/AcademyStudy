//Vertex Shader
// ��ȯ��, �������̴� �Լ���, �ø�ƽ(�ṉ̀���)
// �������̴�-> �������ۿ� ����ִ� ��� ������ 1ȸ ȣ��ȴ�.
// �ݵ�� ó���ؾ��ϴ� ��Ȱ�� 
// float4�� ������ ��ġ(SV_POSITION)�� ��ȯ�ؾ� �Ѵ�.
// �ø�ƽ
// ��� �������� c0 ~ 65553
// c0 = x,y,z,w
//      r,g,b,a

cbuffer cb1 : register( b1 )
{
	matrix g_matWorld : packoffset(c0);
	matrix g_matView : packoffset(c4);
	matrix g_matProj : packoffset(c8);
};

cbuffer cb0: register(b0)
{
	float4 Color : packoffset(c0);	// c0
	float  time : packoffset(c1.x);    // c1
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
};
VS_OUTPUT VS(VS_INPUT input)
{
	VS_OUTPUT output = (VS_OUTPUT)0;
	float4 pos = float4(input.p, 1.0f);
	float4 world = mul(pos, g_matWorld);
	float4 view = mul(world, g_matView);
	//matrix matProj = transpose(g_matProj);
	float4 proj = mul(view, g_matProj);
	output.p = proj;
	output.t = input.t;// time*0.5f + 0.5f;
	output.c = float4(Color.x, Color.y, Color.z, Color.w);
	return output;
}