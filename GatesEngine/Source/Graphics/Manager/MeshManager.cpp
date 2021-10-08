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
	////ƒ‚ƒfƒ‹‚ªŒ©‚Â‚©‚ç‚È‚©‚Á‚½‚çŽlŠpŒ`‚ð•Ô‚·
	Mesh* mesh = meshes.at(name.c_str());

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
