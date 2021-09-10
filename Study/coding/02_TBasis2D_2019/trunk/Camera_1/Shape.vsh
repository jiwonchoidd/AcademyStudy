//Vertex Shader
// 반환값, 정점세이더 함수명, 시멘틱(의미구조)
// 정점쉐이더-> 정점버퍼에 들어있는 모든 정점당 1회 호출된다.
// 반드시 처리해야하는 역활은 
// float4로 정점의 위치(SV_POSITION)를 반환해야 한다.
// 시멘틱
// 상수 레지스터 c0 ~ 65553
// c0 = x,y,z,w
//      r,g,b,a

cbuffer cb0 : register(b0)
{
	matrix g_matWorld : packoffset(c0);
	matrix g_matView : packoffset(c4);
	matrix g_matProj : packoffset(c8);
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
VS_OUTPUT VS(VS_INPUT input)
{
	VS_OUTPUT output = (VS_OUTPUT)0;
	float4 pos = float4(input.p, 1.0f);
	float4 world = mul(pos, g_matWorld);
	float4 view = mul(world, g_matView);
	float4 proj = mul(view, g_matProj);
	output.p = proj;
	output.t = input.t;
	output.c = input.c;
	return output;
}