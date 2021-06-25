cbuffer Model : register(b0)
{
	matrix modelMatrix;
}

cbuffer Orthograph : register(b1)
{
	matrix projMatrix;
}

cbuffer GaussBlurData : register(b2)
{
	float4 blurData[2];
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