
float3 GetFractalNoise(uint2 data)
{
	float3 result = float3(0,0,0);
	result.x = frac(sin(dot(float2(data.x, 0), float2(12.9898, 7.233))) * 43758.5453);
	result.y = frac(sin(dot(float2(0, data.y), float2(12.9898, 7.233))) * 43758.5453);
	result.z = frac(sin(dot(float2(data.x, data.y), float2(12.9898, 7.233))) * 43758.5453);
	return result;
}

RWTexture2D<float4> data : register(u0);

//Create 128 * 128 parlinNoiseTexture
static const float TEXTURESIZE = 128;
[numthreads(1,1,1)]
void main( uint3 DTid : SV_DispatchThreadID )
{
	//data[DTid.xy].xyz = GetFractalNoise(DTid);
	//data[DTid.xy].a = 1;
}