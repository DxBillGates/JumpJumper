#pragma once
#include <map>
#include <string>

namespace GatesEngine
{
	class Mesh;
	class GraphicsDevice;
	class MeshManager
	{
	private:
		GraphicsDevice* graphicsDevice;
		std::map<std::string, Mesh*> meshes;
	public:
		MeshManager(GraphicsDevice* pGraphicsDevice);
		~MeshManager();
		Mesh* Add(const std::string& name);
		Mesh* GetMesh(const std::string& name);
	};
}
