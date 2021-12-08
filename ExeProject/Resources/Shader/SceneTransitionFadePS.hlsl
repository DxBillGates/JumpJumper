#include "DefaultSpriteShader.hlsli"

Texture2D<float4> sceneRenderTexture : register(t0);

SamplerState wrapPointSampler : register(s0);
SamplerState clampPointSampler : register(s1);
SamplerState wrapLinearSampler  : register(s2);
SamplerState clampLinearSampler : register(s3);

float4 main(DefaultSpriteVSOutput input) : SV_TARGET
{
	float4 sceneRenderTextureColor = sceneRenderTexture.Sample(clampPointSampler,input.uv);
	return float4(sceneRenderTextureColor.rgb,1) * color;
}