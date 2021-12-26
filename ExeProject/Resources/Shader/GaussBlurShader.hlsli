cbuffer Model : register(b0)
{
	matrix modelMatrix;
}

cbuffer Orthograph : register(b1)
{
	matrix projMatrix;
}

cbuffer GaussFilterData : register(b2)
{
	float4 data[16];
}

struct VSInput
{
	float4 pos : POSITION;
	float2 uv : TEXCOORD;
};

struct VSOutput
{
	float4 svpos : SV_POSITION;
	float2 uv : TEXCOORD;
};

float Gaussian(float2 inputUV, float2 uv, float sigma)
{
	float d = distance(inputUV, uv);
	return exp(-(d * d) / (2 * sigma * sigma));
}