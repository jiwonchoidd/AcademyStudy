//����ü�� �̿��ؼ� ��ġ�� ���� ������
//���� ����ü �������Ϳ� ����
// ������ ��� ���(�ݵ�� ���� ��ġ�� SV_POSITION�� �����ؾ� �Ѵ�.)
cbuffer cbData
{
	matrix g_matWorld	: packoffset(c0);
	matrix g_matView	: packoffset(c4);
	matrix g_matProj	: packoffset(c8);
	float  g_fTimer : packoffset(c12.z); // 4��Ʈ�߿� z�࿡ ����
};

struct VS_OUT
{
	float4 p : SV_POSITION;
	float4 c : COLOR0;
};
VS_OUT VS(float3 p: POSITION, float4 c : COLOR)
{
	VS_OUT output = (VS_OUT)0;
	float4 vLocal = float4(p, 1.0f);
	float4 vWorld = mul(vLocal, g_matWorld);

	float t = cos(g_fTimer * 0.25f); // -1 ~ +1	
	vWorld.y = cos(t * vWorld.x * 0.1f) + sin(t * vWorld.z * 0.1f);

	float4 vView = mul(vWorld, g_matView);
	float4 vProj = mul(vView, g_matProj);

	output.p = vProj;
	output.c.x = 0.2f;
	output.c.y = 0.2f;
	output.c.z = 0.2f;
	output.c.w = 1.0f;
	return output;
}