//Vertex Shader
// 반환값, 정점세이더 함수명, 시멘틱(의미구조)
// 정점쉐이더-> 정점버퍼에 들어있는 모든 정점당 1회 호출된다.
// 반드시 처리해야하는 역활은 
// float4로 정점의 위치(SV_POSITION)를 반환해야 한다.
// 시멘틱
// 상수 레지스터 c0 ~ 65553
// c0 = x,y,z,w
//      r,g,b,a
cbuffer cb0
{
	float4 Color : packoffset(c0);	// c0
	float  time  : packoffset(c1.x);    // c1
};
struct VS_INPUT
{
	float3 p : POSITION;
	float4 c : COLOR0;
	float2 t : TEXCOORD0;
};

struct VS_OUTPUT
{
	float4 p : SV_POSITION; // 정점 위치 레지스터 사용
	float4 c : COLOR0;      // 정점 컬러 레지스터 사용
	float2 t : TEXCOORD0;   // 정점 텍스쳐 좌표 레지스터 사용
};
VS_OUTPUT VS( VS_INPUT input)
{
	VS_OUTPUT output = (VS_OUTPUT)0;
	float4 pos = float4(input.p, 1.0f);
	output.p.x = pos.x;
	output.p.y = pos.y;
	output.p.z = pos.z;
	output.p.w = 1.0f;
	output.t = input.t;// time*0.5f + 0.5f;
	output.c = input.c;// float4(Color.x, Color.y, Color.z, Color.w);
	return output;
}