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
	float EdgeTessFactor[3]			: SV_TessFactor;
	float InsideTessFactor : SV_InsideTessFactor;
};

struct DSOutput
{
	float4 svpos : SV_POSITION;
	float2 uv : TEXCOORD;
	float3 normal : NORMAL;
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
}