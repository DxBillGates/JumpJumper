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
	meshInfos.push_back({name,mesh});
	return mesh;
}

GatesEngine::Mesh* GatesEngine::MeshManager::GetMesh(const std::string& name)
{
	////モデルが見つからなかったら四角形を返す
	//Mesh* mesh = meshes.at(name.c_str());

	Mesh* mesh = meshes[name.c_str()];
	if (!mesh)mesh = meshes["Cube"];
	//for (auto m : meshInfos)
	//{
	//	if (m.name == name)
	//	{
	//		return m.mesh;
	//	}
	//}
	//for (auto m : meshes)
	//{
	//	if (m.first == name)
	//	{
	//		mesh = m.second;
	//		return mesh;
	//	}
	//}



	return mesh;
}
