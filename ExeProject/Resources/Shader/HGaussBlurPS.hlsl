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

	float4 texColor = tex.Sample(clampPointSampler, input.uv);
	float4 resultColor = float4(0, 0, 0, 0);

	resultColor += blurData[0] * texColor;

	//横方向にブラーをかける
	for (uint i = 0; i < 8; ++i)
	{
		resultColor += blurData[i >> 2][i % 4] * tex.Sample(clampPointSampler, saturate(input.uv + float2( pixel.x * i,0)));
		resultColor += blurData[i >> 2][i % 4] * tex.Sample(clampPointSampler, saturate(input.uv + float2(-pixel.y * i,0)));
	}

	return float4(resultColor.rgb,texColor.a);
}