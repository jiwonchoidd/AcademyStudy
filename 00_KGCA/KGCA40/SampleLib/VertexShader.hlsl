//구조체를 이용해서 위치와 색을 보내줌
struct VS_OUT
{
	float4 p : SV_POSITION;
	float4 c : COLOR0;
};
VS_OUT VS(float3 p: POSITION, float4 c : COLOR)
{
	VS_OUT output = (VS_OUT)0;
	output.p = float4(p, 1.0f);
	output.c = c;
	return output;
}
