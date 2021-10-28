struct VS_OUT
{
	float4 pos : SV_POSITION;
	float4 color : COLOR0;
};
float4 PS(VS_OUT vertex) : SV_TARGET
{
	return vertex.color;
}