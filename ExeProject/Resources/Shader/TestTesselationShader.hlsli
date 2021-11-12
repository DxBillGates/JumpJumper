struct VSInput
{
	float4 pos : POSITION;
	float2 uv : TEXCOORD;
	float3 normal : NORMAL;
};

struct VSOutput
{
	float4 svpos : SV_POSITION;
	float4 wpos : POSITION;
	float2 uv : TEXCOORD;
	float3 normal : NORMAL;
};

struct HS_CONTROL_POINT_OUTPUT
{
	float4 svpos : SV_POSITION;
	float4 wpos : POSITION;
	float2 uv : TEXCOORD;
	float3 normal : NORMAL;
};

struct HS_CONSTANT_DATA_OUTPUT
{
	float EdgeTessFactor[4]			: SV_TessFactor;
	float InsideTessFactor[2] : SV_InsideTessFactor;
};

struct DSOutput
{
	float4 svpos : SV_POSITION;
	float2 uv : TEXCOORD;
	float3 normal : NORMAL;
	float3 color :COLOR;
};

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

cbuffer TesselationInfo : register(b4)
{
	float4 tesselationInfo;
	float4 factor;
}

Texture2D<float4> tex : register(t0);
Texture2D<float4> heightMapTex : register(t1);

SamplerState wrapPointSampler : register(s0);
SamplerState clampPointSampler : register(s1);
SamplerState wrapLinearSampler  : register(s2);
SamplerState clampLinearSampler : register(s3);