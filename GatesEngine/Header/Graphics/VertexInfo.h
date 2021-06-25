#pragma once
#include <vector>
#include "..\..\Header\Math\Math.h"

namespace GatesEngine
{
	namespace VertexInfo
	{
		struct Vertex
		{
			GatesEngine::Math::Vector3 point;
		};

		struct Vertex_UV
		{
			GatesEngine::Math::Vector3 point;
			GatesEngine::Math::Vector2 uv;
		};

		struct Vertex_Normal
		{
			GatesEngine::Math::Vector3 point;
			GatesEngine::Math::Vector3 normal;
		};

		struct Vertex_UV_Normal
		{
			GatesEngine::Math::Vector3 point;
			GatesEngine::Math::Vector2 uv;
			GatesEngine::Math::Vector3 normal;
		};

		struct Vertex_UV_Normal_Color
		{
			GatesEngine::Math::Vector3 point;
			GatesEngine::Math::Vector2 uv;
			GatesEngine::Math::Vector3 normal;
			GatesEngine::Math::Vector4 color;
		};

		struct Vertex_Color
		{
			GatesEngine::Math::Vector3 pos;
			GatesEngine::Math::Vector4 color;
		};
	}
}