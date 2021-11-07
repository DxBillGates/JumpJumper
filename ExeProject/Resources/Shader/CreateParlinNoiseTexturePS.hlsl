#include "CreateParlinNoiseTextureShader.hlsli"

float Random(float2 fact)
{
	return frac(sin(dot(fact, float2(12.9898f, 78.233f))) * 43758.5453f);
}

float2 RandomVector2(float2 fact)
{
	float2 angle = float2(dot(fact, float2(127.1f, 311.7f)), dot(fact, float2(269.5f, 183.3f)));
	return frac(sin(angle) * 43758.5453123f) * 2 - 1;
}

float GetRamdomNoise(float2 fact)
{
	return Random(fact);
}

float GetBlockNoise(float2 fact, float size)
{
	float2 uv = floor(fact * size) / size;
	return Random(uv);
}

float GetBlurBlockNoise(float2 fact, float size)
{
	float v00 = Random(floor(fact * size));
	float v01 = Random(floor(fact * size)) + float2(0, 1);
	float v10 = Random(floor(fact * size)) + float2(1, 0);
	float v11 = Random(floor(fact * size)) + float2(1, 1);
	float2 p = frac(fact * size);
	float v0010 = lerp(v00, v10, p.x);
	float v0111 = lerp(v01, v11, p.x);

	float lerpNoise = lerp(v0010, v0111, p.y);
	return lerpNoise;
}

float GetParlinNoise(float2 uv, float size)
{
	float2 uvFloor = floor(uv * size);
	float2 uvFrac = frac(uv * size);

	float2 v00 = RandomVector2(uvFloor + float2(0, 0));
	float2 v01 = RandomVector2(uvFloor + float2(0, 1));
	float2 v10 = RandomVector2(uvFloor + float2(1, 0));
	float2 v11 = RandomVector2(uvFloor + float2(1, 1));

	float c00 = dot(v00, uvFrac - float2(0, 0));
	float c01 = dot(v01, uvFrac - float2(0, 1));
	float c10 = dot(v10, uvFrac - float2(1, 0));
	float c11 = dot(v11, uvFrac - float2(1, 1));

	float2 u = uvFrac * uvFrac * (3 - 2 * uvFrac);

	float v0010 = lerp(c00, c10, u.x);
	float v0111 = lerp(c01, c11, u.x);

	return lerp(v0010, v0111, u.y) / 2 + 0.5f;
}

float GetFractalSumNoise(float2 uv, float size)
{
	float fn = 0;
	int j = 1;
	for (int i = 0; i < 4; ++i)
	{
		fn += GetParlinNoise(uv,size * j) * 1.0f / j;
		j *= 2;
	}
	return fn;
}

float4 main(VSOutput input) : SV_TARGET
{
	float r = Random(input.uv);
	r = GetFractalSumNoise(input.uv, 10);
	return float4(r,r,r,r);
}