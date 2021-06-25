cbuffer Model : register(b0)
{
	matrix modelMatrix;
}

cbuffer Color : register(b1)
{
	float4 color;
}

cbuffer ScreenInfo : register(b2)
{
	matrix projMatrix;
}

struct DefaultSpriteVSInput
{
	float4 pos : POSITION;
	float2 uv : TEXCOORD;
};

struct DefaultSpriteVSOutput
{
	float4 svpos : SV_POSITION;
	float2 uv : TEXCOORD;
};