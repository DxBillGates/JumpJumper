#include "..\..\..\Header\Graphics\Manager\MeshManager.h"
#include "..\..\..\Header\Graphics\Mesh.h"

GatesEngine::MeshManager::MeshManager(GraphicsDevice* pGraphicsDevice)
	: graphicsDevice(pGraphicsDevice)
{
}

GatesEngine::MeshManager::~MeshManager()
{
	for (auto itr = meshes.begin(); itr != meshes.end(); ++itr)
	{
		delete (*itr).second;
	}
}

GatesEngine::Mesh* GatesEngine::MeshManager::Add(const std::string& name)
{
	Mesh* mesh = new Mesh();
	meshes.insert(std::make_pair(name.c_str(),mesh));
	return mesh;
}

GatesEngine::Mesh* GatesEngine::MeshManager::GetMesh(const std::string& name)
{
	Mesh* mesh = meshes.at(name.c_str());
	return mesh;
}
