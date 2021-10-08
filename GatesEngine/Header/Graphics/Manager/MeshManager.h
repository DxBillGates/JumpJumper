#pragma once
#include <map>
#include <vector>
#include <string>

namespace GatesEngine
{
	class Mesh;
	class GraphicsDevice;

	struct MeshInfo
	{
		std::string name;
		Mesh* mesh;
	};

	class MeshManager
	{
	private:
		GraphicsDevice* graphicsDevice;
		std::map<std::string, Mesh*> meshes;
		std::vector<MeshInfo> meshInfos;
	public:
		MeshManager(GraphicsDevice* pGraphicsDevice);
		~MeshManager();
		Mesh* Add(const std::string& name);
		Mesh* GetMesh(const std::string& name);
	};
}
