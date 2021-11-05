#include "CreateParlinNoiseTextureShader.hlsli"

float4 main(VSOutput input) : SV_TARGET
{
    return float4(input.uv.xy,1, 1.0f);
}