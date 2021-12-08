#include "GaussBlurShader.hlsli"

Texture2D<float4> tex : register(t0);

SamplerState wrapPointSampler : register(s0);
SamplerState clampPointSampler : register(s1);
SamplerState wrapLinearSampler  : register(s2);
SamplerState clampLinearSampler : register(s3);

float4 main(VSOutput input) : SV_TARGET
{
	//テクスチャのサイズを取得
	float w,h,level;
	tex.GetDimensions(0, w, h, level);

	//サイズから1ピクセル分のuv値を取得
	float perPixel = 1;
	float2 pixel = float2(perPixel / w, perPixel / h);

	float totalWeight = 0, sigma = 0.01f, stepWidth = 0.004f;
	float4 samplingColor = float4(0, 0, 0, 0);

	for (float py = -sigma * 2; py <= sigma * 2; py += stepWidth)
	{
		for (float px = -sigma * 2; px <= sigma * 2; px += stepWidth)
		{
			float2 uv = input.uv + float2(px, py);
			float weight = Gaussian(input.uv, uv, sigma);
			samplingColor += tex.Sample(clampPointSampler, uv) * weight;
			totalWeight += weight;
		}
	}

	samplingColor.rgb = samplingColor.rgb / totalWeight;

	return float4(samplingColor.rgb,1);
}