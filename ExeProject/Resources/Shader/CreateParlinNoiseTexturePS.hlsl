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

static const float cloudscale = 1.0;
static const float speed = 0.03;
static const float clouddark = 0.5;
static const float cloudlight = 0.3;
static const float cloudcover = 0.2;
static const float cloudalpha = -1.0f;
static const float skytint = 10.0;
static const float3 skycolour1 = float3(0.2, 0.4, 0.6);
static const float3 skycolour2 = float3(0.4, 0.7, 1.0);
static const float2x2 m = float2x2(1.6, 1.2, -1.2, 1.6);

float2 hash(float2 p)
{
	p = float2(dot(p, float2(127.1, 311.7)), dot(p, float2(269.5, 183.3)));
	return -1.0 + 2.0 * frac(sin(p) * 43758.5453123);
}

float noise(float2 p) 
{
	const float K1 = 0.366025404;
	const float K2 = 0.211324865;
	float2 i = floor(p + (p.x + p.y) * K1);
	float2 a = p - i + (i.x + i.y) * K2;
	float2 o = (a.x > a.y) ? float2(1.0, 0.0) : float2(0.0, 1.0);
	float2 b = a - o + K2;
	float2 c = a - 1.0 + 2.0 * K2;
	float3 h = max(0.5 - float3(dot(a, a), dot(b, b), dot(c, c)), 0.0);
	float3 n = h * h * h * h * float3(dot(a, hash(i + 0.0)), dot(b, hash(i + o)), dot(c, hash(i + 1.0)));
	return dot(n, float3(70.0,70.0,70.0));
}

float fbm(float2 n) {
	float total = 0.0, amplitude = 0.1;
	for (int i = 0; i < 7; i++) {
		total += noise(n) * amplitude;
		n = mul(m,n);
		amplitude *= 0.4;
	}
	return total;
}

float circle(float2 p, float r)
{
	return length(p) - r;
}

float4 main(VSOutput input) : SV_TARGET
{
	const int SAMPLING_VALUE = 6;
	//float r = Random(input.uv);
	//r = GetFractalSumNoise(input.uv, 10);
	////float a = r < 0.8f ? 0.1f : 1;
	//r = noise(input.uv);
	float2 uv = input.uv;
	float2 iTime = time * speed;
	float q = fbm(input.uv * cloudscale * 0.5f);

	float r = 0;
	uv *= cloudscale;
	uv -= q - iTime.x;

	float weight = 0.8f;
	for (int i = 0; i < SAMPLING_VALUE; ++i)
	{
		r += abs(weight * noise(uv));
		uv = mul(m, uv) + iTime.x;
		weight *= 0.7f;
	}

	float f = 0;
	uv = input.uv;
	uv *= cloudscale;
	uv -= q - iTime.x;
	weight = 0.7f;
	for (int i = 0; i < SAMPLING_VALUE; ++i)
	{
		f += abs(weight * noise(uv));
		uv = mul(m, uv) + iTime.x;
		weight *= 0.6f;
	}

	f *= r + f;

	float c = 0;
	iTime = time.x * speed * 2.0f;
	uv = input.uv;
	uv *= cloudscale * 2.0f;
	uv -= q - iTime.x;
	weight = 0.4f;
	for (int i = 0; i < SAMPLING_VALUE; ++i)
	{
		c += abs(weight * noise(uv));
		uv = mul(m, uv) + iTime.x;
		weight *= 0.6f;
	}

	float c1 = 0;
	iTime = time.x * speed * 3.0f;
	uv = input.uv;
	uv *= cloudscale * 3.0f;
	uv -= q - iTime.x;
	weight = 0.4f;
	for (int i = 0; i < SAMPLING_VALUE; ++i)
	{
		c1 += abs(weight * noise(uv));
		uv = mul(m, uv) + iTime.x;
		weight *= 0.6f;
	}

	c += c1;
	//float a = r * f * c * c1;

	float a = length(float2(0.5,0.5) - input.uv);
	float3 skycolor1 = lerp(skycolour1 * a, skycolour2 * a, input.uv.x);
	float3 skycolor2 = lerp(skycolour1 * a, skycolour2 * a, input.uv.y);
	float3 skycolor = lerp(skycolor1, skycolor2, 0.5f);
	float3 cloudcolor = float3(1.1f, 1.1f, 0.9f) * clamp((clouddark + cloudlight * c), 0.0f, 1.0f);
	f = cloudcover + cloudalpha * f * r;

	float3 result = lerp(skycolor,clamp(skytint * skycolor + cloudcolor, 0.0f, 1.0f), clamp(f + c, 0.0f, 1.0f));
	//float3 cloudcolor = float3(1.1f, 1.1f, 0.9f) * clamp(skytint * skycolor + cloudcolor, 0.0f, 1.0f), clamp(f + c, 0.0f, 1.0f));


	return float4(result,1);
}