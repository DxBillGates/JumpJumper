#pragma once
#include "..\MeshData.h"
#include "..\VertexInfo.h"
#include <string>

namespace GatesEngine
{
	namespace MeshCreater
	{
		void CreatePlane(Math::Vector2 size, Math::Vector2 uvMax, MeshData<VertexInfo::Vertex_UV_Normal>& meshData);
		void CreateQuad(Math::Vector2 size, Math::Vector2 uvMax, MeshData<VertexInfo::Vertex_UV_Normal>& meshData, Math::Vector2 center = {0});
		void CreateQuad(Math::Vector2 size, Math::Vector2 uvMax,float divide, MeshData<VertexInfo::Vertex_UV_Normal>& meshData);
		void Create2DQuad(Math::Vector2 size, Math::Vector2 uvMax, MeshData<VertexInfo::Vertex_UV_Normal>& meshData, Math::Vector2 center = {0});
		void CreateGrid(Math::Vector2 size, float spaceInterval, MeshData<VertexInfo::Vertex_Color>& meshData);
		void CreateLine(Math::Vector3 size, const Math::Vector4& color,MeshData<VertexInfo::Vertex_Color>& meshData);
		void CreateLineCube(Math::Vector3 size,const Math::Vector4& color,MeshData<VertexInfo::Vertex_Color>& meshData);
		void CreateLineCircle(Math::Vector3 size, float vertexCount, const Math::Vector4& color, MeshData<VertexInfo::Vertex_Color>& meshData);
		void CreateCube(Math::Vector3 size, MeshData<VertexInfo::Vertex_UV_Normal>& meshData);
		void CreateSphere(Math::Vector3 size,int vTess,int hTess,MeshData<VertexInfo::Vertex_UV_Normal>& meshData);
		void LoadModelData(const std::string& filename,MeshData<VertexInfo::Vertex_UV_Normal>& meshData);
		void LoadGates3DModelData(const std::string& filename,MeshData<VertexInfo::Vertex_UV_Normal>& meshData);
	}
}