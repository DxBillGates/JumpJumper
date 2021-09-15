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

			Vertex() : point({}) {}
			Vertex(const Math::Vector3& p) :point(p) {}
		};

		struct Vertex_UV
		{
			GatesEngine::Math::Vector3 point;
			GatesEngine::Math::Vector2 uv;

			Vertex_UV() : point({}), uv({}) {}
			Vertex_UV(const Math::Vector3& p,const Math::Vector2& u) : point(p),uv(u) {}
		};

		struct Vertex_Normal
		{
			GatesEngine::Math::Vector3 point;
			GatesEngine::Math::Vector3 normal;

			Vertex_Normal() : point({}), normal({}) {}
			Vertex_Normal(const Math::Vector3& p, const Math::Vector3& n) : point(p), normal(n) {}
		};

		struct Vertex_UV_Normal
		{
			GatesEngine::Math::Vector3 point;
			GatesEngine::Math::Vector2 uv;
			GatesEngine::Math::Vector3 normal;

			Vertex_UV_Normal() : point({}), uv({}), normal({}) {}
			Vertex_UV_Normal(const Math::Vector3& p, const Math::Vector2& u, const Math::Vector3& n) : point(p),uv(u), normal(n) {}
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