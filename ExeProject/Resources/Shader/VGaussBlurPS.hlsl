#include "GaussBlurShader.hlsli"

Texture2D<float4> tex : register(t0);
SamplerState smp : register(s0);

float4 main(VSOutput input) : SV_TARGET
{
	//�e�N�X�`���̃T�C�Y���擾
	float w,h,level;
	tex.GetDimensions(0, w, h, level);

	//�T�C�Y����1�s�N�Z������uv�l���擾
	float perPixel = 1;
	float2 pixel = float2(perPixel / w, perPixel / h);

	float4 texColor = tex.Sample(smp, input.uv);
	float4 resultColor = float4(0, 0, 0, 0);

	resultColor += blurData[0] * texColor;

	//�c�����Ƀu���[��������
	for (uint i = 0; i < 8; ++i)
	{
		resultColor += blurData[i >> 2][i % 4] * tex.Sample(smp, saturate(input.uv + float2(0,  pixel.x * i)));
		resultColor += blurData[i >> 2][i % 4] * tex.Sample(smp, saturate(input.uv + float2(0, -pixel.y * i)));
	}

	return float4(resultColor.rgb,texColor.a);
}