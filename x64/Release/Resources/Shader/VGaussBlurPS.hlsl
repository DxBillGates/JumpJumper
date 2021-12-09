#include "GaussBlurShader.hlsli"

Texture2D<float4> tex : register(t0);

SamplerState wrapPointSampler : register(s0);
SamplerState clampPointSampler : register(s1);
SamplerState wrapLinearSampler  : register(s2);
SamplerState clampLinearSampler : register(s3);

float4 main(VSOutput input) : SV_TARGET
{
	//�e�N�X�`���̃T�C�Y���擾
	float w,h,level;
	tex.GetDimensions(0, w, h, level);

	//�T�C�Y����1�s�N�Z������uv�l���擾
	float perPixel = 1;
	float2 pixel = float2(perPixel / w, perPixel / h);

	float4 texColor = tex.Sample(clampPointSampler, input.uv);
	return texColor;
}