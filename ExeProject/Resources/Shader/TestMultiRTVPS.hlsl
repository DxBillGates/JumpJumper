#include "TestMultiRTVShader.hlsli"

Texture2D<float4> tex : register(t0);
SamplerState smp : register(s0);

PSOutput main(VSOutput input)
{
	float4 texColor = tex.Sample(smp, saturate(input.uv));
	PSOutput output;
	//色反転
	output.rtv0 = float4(1,1,1,1) - texColor.rgba;
	output.rtv0.a = 1;

	//平均ぼかし
	float2 pixel = float2(1 / 1920.0f, 1 / 1080.0f);
	float4 addNearPixelColor = float4(0, 0, 0, 0);

	//サンプリング回数
	float sampleCount = 10;
	float2 offset = input.uv + float2(-pixel.x, -pixel.y) * sampleCount;
	float count = 0;
	for (int j = 0; j < sampleCount * 2; ++j)
	{
		for (int k = 0; k < sampleCount * 2; ++k)
		{
			addNearPixelColor += tex.Sample(smp, saturate(offset + float2(pixel.x * j, pixel.y * k)));
			++count;
		}
	}
	float4 r = addNearPixelColor / count;

	output.rtv1 = saturate(r);
	return output;
}