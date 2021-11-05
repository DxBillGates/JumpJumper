cbuffer Model : register(b0)
{
	matrix modelMatrix;
}

cbuffer Material : register(b1)
{
	float4 ambient;
	float4 diffuse;
	float4 specular;
}

cbuffer CameraInfo : register(b2)
{
	matrix viewMatrix;
	matrix projMatrix;
	float4 cameraPos;
	matrix billboard;
}

cbuffer LightInfo : register(b3)
{
	float4 worldLightDir;
	float4 lightColor;
}

cbuffer LightViewInfo : register(b4)
{
	matrix lightViewMatrix;
}

struct VSInput
{
	float4 pos : POSITION;
	float2 uv : TEXCOORD;
	float3 normal : NORMAL;
};

struct VSOutput
{
	float4 svpos : SV_POSITION;
	float4 tpos : TEXCOORD2;
	float2 uv : TEXCOORD;
	float3 normal : NORMAL;
};

struct PSOutput
{
	float4 result : SV_TARGET0;
	float4 shadow : SV_TARGET1;
};