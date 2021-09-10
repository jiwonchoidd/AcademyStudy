struct VS_OUTPUT
{
	float4 p : SV_POSITION; // 정점 위치 레지스터 사용
	float4 c : COLOR0;      // 정점 컬러 레지스터 사용
	float2 t : TEXCOORD0;   // 정점 텍스쳐 좌표 레지스터 사용
};
// Geometry Shader
//error X3514 : 'GS' : stream output parameter 'triStream' must be an inout parameter
[maxvertexcount(9)]
void GS(in triangle VS_OUTPUT input[3],
	inout TriangleStream<VS_OUTPUT> triStream)
{
	VS_OUTPUT output;
	float3 centerPos = (input[0].p.xyz +
		input[1].p.xyz +
		input[2].p.xyz) / 3.0f;
	float4 centerColor = (input[0].c +
		input[1].c +
		input[2].c) / 3.0f;
	float2 centerTex = (input[0].t +
		input[1].t +
		input[2].t) / 3.0f;
	for (int i = 0; i < 3; i++)
	{
		output.p = input[i].p;
		output.c = input[i].c;
		output.t = input[i].t;
		triStream.Append(output);
		int iNext = (i + 1) % 3;
		output.p = input[iNext].p;
		output.c = input[iNext].c;
		output.t = input[iNext].t;
		triStream.Append(output);

		output.p = float4(centerPos, 1);
		output.c = input[iNext].c;
		output.t = input[iNext].c;
		triStream.Append(output);
	}
	triStream.RestartStrip();
}