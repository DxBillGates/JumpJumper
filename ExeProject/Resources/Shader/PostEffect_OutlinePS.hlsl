#include "DefaultSpriteShader.hlsli"

Texture2D<float4> depthTex : register(t0);
Texture2D<float4> tex : register(t1);
Texture2D<float4> lateDrawDepthTex : register(t2);
Texture2D<float4> lateDrawTex : register(t3);
Texture2D<float4> shadowTex : register(t4);
SamplerState smp : register(s0);

float4 main(DefaultSpriteVSOutput input) : SV_TARGET
{
	float4 texColor = tex.Sample(smp, input.uv);
	float4 depthColor = pow(depthTex.Sample(smp, input.uv),512);
	float4 lateDrawColor = lateDrawTex.Sample(smp, input.uv);
	float4 lateDrawDepthColor = pow(lateDrawDepthTex.Sample(smp, input.uv),512);

	//サンプリング回数2以上でちゃんと表示される
	//アウトライン工程
	float SAMPLING_VALUE = 5;
	float2 perPixel = float2(1 / 1920.0f, 1 / 1080.0f);
	float2 offset = input.uv;
	float4 samplingColor = float4(0, 0, 0, 0);
	offset.x -= perPixel.x * SAMPLING_VALUE / 2.0f;
	offset.y += perPixel.x * SAMPLING_VALUE / 2.0f;
	for (int i = 0; i < SAMPLING_VALUE; ++i)
	{
		for (int j = 0; j < SAMPLING_VALUE; ++j)
		{
			float2 uv = offset + perPixel * float2(i, -j);
			samplingColor += pow(depthTex.Sample(smp,uv), 512);
		}
	}

	////円状に隣接ピクセル参照(微妙)
	//for (int i = 6; i > 1; --i)
	//{
	//	for (int j = 0; j < 6; ++j)
	//	{
	//		float2 uv = float2(0, 0);
	//		uv.x = i * perPixel.x * sin((2.0f * 3.1415f / 6.0f) * j) / 2 + offset.x;
	//		uv.y = -i * perPixel.y * cos((2.0f * 3.1415f / 6.0f) * j) / 2 + offset.y;
	//		samplingColor += pow(depthTex.Sample(smp, uv), 512);
	//	}
	//}
	samplingColor /= SAMPLING_VALUE * SAMPLING_VALUE;

	float c = depthColor.r - samplingColor.r;
	float sub = 0;
	float s = 0.00199f;
	sub = step(c, s);


	//影に対してブラー
	float BLUR_SAMPLING_VALUE = 5;
	float4 blurShadowTex = float4(0, 0, 0, 0);
	for (int k = 0; k < BLUR_SAMPLING_VALUE; ++k)
	{
		for (int j = 0; j < BLUR_SAMPLING_VALUE; ++j)
		{
			float2 uv = offset + perPixel * 2 * float2(k, -j);
			blurShadowTex += shadowTex.Sample(smp, uv);
		}
	}
	blurShadowTex /= BLUR_SAMPLING_VALUE * BLUR_SAMPLING_VALUE;


	// 3Dモデルなどの描画結果と2Dやコライダーの描画結果を深度を比較してどっちを描画するか決定
	float4 resultColor = (depthColor.r < lateDrawDepthColor.r) ? texColor * sub * blurShadowTex : lateDrawColor;
	resultColor = (sub <= 0) ? color : resultColor;
	//outline
	return resultColor;

	//no outline
	return texColor;
}