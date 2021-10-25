#include "DefaultSpriteShader.hlsli"

Texture2D<float4> depthTex : register(t0);
Texture2D<float4> tex : register(t1);
Texture2D<float4> lateDrawDepthTex : register(t2);
Texture2D<float4> lateDrawTex : register(t3);
SamplerState smp : register(s0);

float4 main(DefaultSpriteVSOutput input) : SV_TARGET
{
	float4 texColor = tex.Sample(smp, input.uv);
	float4 depthColor = pow(depthTex.Sample(smp, input.uv),512);
	float4 lateDrawColor = lateDrawTex.Sample(smp, input.uv);
	float4 lateDrawDepthColor = pow(lateDrawDepthTex.Sample(smp, input.uv),512);

	//サンプリング回数2以上でちゃんと表示される
	float SAMPLING_VALUE = 5;
	float2 perPixel = float2(1 / 1920.0f, 1 / 1080.0f) * 3;
	float2 offset = input.uv;
	offset.x -= perPixel.x * SAMPLING_VALUE / 2.0f;
	offset.y += perPixel.x * SAMPLING_VALUE / 2.0f;
	float4 samplingColor = float4(0, 0, 0, 0);
	for (int i = 0; i < SAMPLING_VALUE; ++i)
	{
		for (int j = 0; j < SAMPLING_VALUE; ++j)
		{
			float2 uv = offset + perPixel * float2(i, -j);
			samplingColor += pow(depthTex.Sample(smp,uv), 512);
		}
	}
	samplingColor /= SAMPLING_VALUE * SAMPLING_VALUE;

	float c = depthColor.r - samplingColor.r;
	float sub = 0;
	float s = 0.00699f;
	sub = step(c, s);

	float4 resultColor = (depthColor.r < lateDrawDepthColor.r) ? texColor * sub : lateDrawColor * sub;
	resultColor = (sub <= 0) ? color : resultColor;
	//outline
	return resultColor;

	//no outline
	return texColor;
}