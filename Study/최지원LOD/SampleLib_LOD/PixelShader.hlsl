struct VS_OUT
{
	float4 p : SV_POSITION;
	float4 c : COLOR0;
};
float4 PS(VS_OUT v) : SV_TARGET
{
	return v.c;
}