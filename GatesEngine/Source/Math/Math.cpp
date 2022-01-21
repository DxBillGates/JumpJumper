#include "..\..\Header\Math\Math.h"
#include <math.h>

float GatesEngine::Math::ConvertToRadian(float value)
{
	if (value == 0)return 0;
	float result = PI / 180.0f * value;
	return result;
}

float GatesEngine::Math::Lerp(float s, float e, float t)
{
	return s * (1.0f - t) + e * t;
}

float GatesEngine::Math::GaussFilter(const GatesEngine::Math::Vector2& pos, float value)
{
	return expf(-(pos.x * pos.x + pos.y * pos.y) / (2.0f * value * value));
}

void GatesEngine::Math::SetGaussFilterData(const GatesEngine::Math::Vector2& size, GatesEngine::Math::Vector2& dir, float gaussValue, GaussFilterData* data, int dataSize)
{
	GatesEngine::Math::Vector2 perPixel = { 1.0f / size.x,1.0f / size.y };

	data->data[0] = GatesEngine::Math::Vector4();
	data->data[0].z = GaussFilter({ 0,0 }, gaussValue);
	float totalWeight = data[0].data->z;

	for (int i = 1; i < 8; ++i)
	{
		data->data[i].x = dir.x * i * perPixel.x;
		data->data[i].y = dir.y * i * perPixel.y;
		data->data[i].z = GaussFilter(dir * (float)i, gaussValue);
		totalWeight += data->data[i].z * 2;
	}

	for (int i = 0; i < 8; ++i)
	{
		data->data[i].z /= totalWeight;
	}

	// 負の方向の値もセット
	for (int i = 8; i < 15; ++i)
	{
		data->data[i].x = -data->data[i - 7].x;
		data->data[i].y = -data->data[i - 7].y;
		data->data[i].z = data->data[i - 7].z;
	}
}
