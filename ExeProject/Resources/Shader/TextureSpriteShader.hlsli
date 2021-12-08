cbuffer Model : register(b0)
{
	matrix modelMatrix;
}

cbuffer CameraInfo : register(b1)
{
	matrix projMatrix;
}

cbuffer ColorInfo : register(b3)
{
	float4 color;
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