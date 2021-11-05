#include "CreateParlinNoiseTextureShader.hlsli"

float Circle(float2 p, float r)
{
    return length(p) < r ? 1 : 0;
}

float4 main(VSOutput input) : SV_TARGET
{
    float2 res = float2(1.0f/1920.0f,1.0f/1080.0f);
    float pos = (input.uv * 2 - res) / min(res.x, res.y);
    float a = Circle(pos,0.5f);
    return float4(a,a,a, 1.0f);
}